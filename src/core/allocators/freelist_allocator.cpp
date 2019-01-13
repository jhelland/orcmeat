// Created by jhelland (1/11/19)
//


#include <assert.h> 
#include <limits>  // limits_max
#include <algorithm>  // max
#ifdef _DEBUG
#include <iostream>
#endif

#include "freelist_allocator.h"
#include "../memory_manager.h"


namespace core {
	namespace memory {

		FreeListAllocator::FreeListAllocator(const size_t totalSize, const PlacementPolicy policy) : Allocator(totalSize) {
			placementPolicy = policy;
		}


		FreeListAllocator::~FreeListAllocator() {
			start = nullptr;
		}


		void FreeListAllocator::init() {
			if (start != nullptr)
				start = nullptr;
			
			start = programStackAllocator->alloc(totalSize, 8);
			reset();
		}


		void* FreeListAllocator::alloc(const size_t size, const size_t alignment) {
			assert(size >= sizeof(Node) && "Allocation size must be bigger");
			assert(alignment >= 8 && "Alignment must be at least 8 bytes");

			const size_t allocHeaderSize = sizeof(FreeListAllocator::AllocationHeader);
			const size_t freeHeaderSize = sizeof(FreeListAllocator::FreeHeader);

			// Search for free block w/ enough space
			size_t padding;
			Node* affectedNode;
			Node* prevNode;
			find(size, alignment, padding, prevNode, affectedNode);
			assert(affectedNode != nullptr && "Not enough memory");

			const size_t alignmentPadding = padding - allocHeaderSize;
			const size_t requiredSize = size + padding;
			const size_t rest = affectedNode->data.blockSize - requiredSize;

			if (rest > 0) {
				Node* newFreeNode = (Node*)((size_t)affectedNode + requiredSize);
				newFreeNode->data.blockSize = rest;
				freeList.insert(affectedNode, newFreeNode);
			}
			freeList.remove(prevNode, affectedNode);

			// Set up data block
			const size_t headerAddress = (size_t)affectedNode + alignmentPadding;
			const size_t dataAddress = headerAddress + allocHeaderSize;
			((FreeListAllocator::AllocationHeader*)headerAddress)->blockSize = requiredSize;
			((FreeListAllocator::AllocationHeader*)headerAddress)->padding = alignmentPadding;

			used += requiredSize;
			peak = std::max(peak, used);
#ifdef _DEBUG
			//std::cout << "ALLOC" << "\t@H " << (void*)headerAddress << "\tD@ " << (void*)dataAddress << "\tS " << ((FreeListAllocator::AllocationHeader*)headerAddress)->blockSize << "\tAP " << alignmentPadding << "\tP " << padding << "\tM " << used << "\tR " << rest << std::endl;
#endif 
			return (void*)dataAddress;
		}


		void FreeListAllocator::find(const size_t size, const size_t alignment, size_t& padding, Node*& prevNode, Node*& foundNode) {
			switch (placementPolicy) {
			case FIND_FIRST:
				find_first(size, alignment, padding, prevNode, foundNode);
				break;
			case FIND_BEST:
				find_best(size, alignment, padding, prevNode, foundNode);
				break;
			}
		}


		// Find the first block that fits the target
		void FreeListAllocator::find_first(const size_t size, const size_t alignment, size_t& padding, Node*& prevNode, Node*& foundNode) {
			Node* it = freeList.head;
			Node* itPrev = nullptr;

			while (it != nullptr) {
				padding = utils::calc_padding_header((size_t)it, alignment, sizeof(FreeListAllocator::AllocationHeader));
				const size_t requiredSpace = size + padding;
				if (it->data.blockSize >= requiredSpace)
					break;
				
				itPrev = it;
				it = it->next;
			}
			prevNode = itPrev;
			foundNode = it;
		}


		// Find block that fits the target most snugly
		void FreeListAllocator::find_best(const size_t size, const size_t alignment, size_t& padding, Node*& prevNode, Node*& foundNode) {
			size_t smallestDiff = std::numeric_limits<size_t>::max();
			Node* bestBlock = nullptr;
			Node* it = freeList.head;
			Node* itPrev = nullptr;

			while (it != nullptr) {
				padding = utils::calc_padding_header((size_t)it, alignment, sizeof(FreeListAllocator::AllocationHeader));
				const size_t requiredSpace = size + padding;

				if (it->data.blockSize >= requiredSpace && (it->data.blockSize - requiredSpace < smallestDiff))
					bestBlock = it;

				itPrev = it;
				it = it->next;
			}
			prevNode = itPrev;
			foundNode = bestBlock;
		}


		void FreeListAllocator::dealloc(void* ptr) {
			const size_t currAddr = (size_t)ptr;
			const size_t headerAddr = currAddr - sizeof(FreeListAllocator::AllocationHeader);
			const FreeListAllocator::AllocationHeader* allocHeader{ (FreeListAllocator::AllocationHeader*)headerAddr };

			Node* freeNode = (Node*)headerAddr;
			freeNode->data.blockSize = allocHeader->blockSize + allocHeader->padding;
			freeNode->next = nullptr;

			Node* it = freeList.head;
			Node* itPrev = nullptr;
			while (it != nullptr) {
				if (ptr < it) {
					freeList.insert(itPrev, freeNode);
					break;
				}
				itPrev = it;
				it = it->next;
			}
			used -= freeNode->data.blockSize;

			// Merge contiguous/adjacent blocks
			coalesce(itPrev, freeNode);
#ifdef _DEBUG
			//std::cout << "FREED" << "\t@ptr " << ptr << "\tH@ " << (void*)freeNode << "\tS " << freeNode->data.blockSize << "\tM " << used << std::endl;
#endif
		}


		// Sometimes when a block is deallocated, adjacent blocks are also free. In this case, it makes sense to
		// merge those blocks to reduce nodes in the freeList.
		void FreeListAllocator::coalesce(Node* prevNode, Node* freeNode) {
			if (freeNode->next != nullptr && (size_t)freeNode + freeNode->data.blockSize == (size_t)freeNode->next) {
				freeNode->data.blockSize += freeNode->next->data.blockSize;
				freeList.remove(freeNode, freeNode->next);
#ifdef _DEBUG
				//std::cout << "\tMerging(n) " << (void*)freeNode << " & " << (void*)freeNode->next << "\tS " << freeNode->data.blockSize << std::endl;
#endif 
			}

			if (prevNode != nullptr && (size_t)prevNode + prevNode->data.blockSize == (size_t)freeNode) {
				prevNode->data.blockSize += freeNode->data.blockSize;
				freeList.remove(prevNode, freeNode);
#ifdef _DEBUG
				//std::cout << "\tMerging(p) " << (void*)prevNode << " & " << (void*)freeNode << "\tS " << prevNode->data.blockSize << std::endl;
#endif
			}
		}


		void FreeListAllocator::reset() {
			used = 0;
			peak = 0;
			Node* firstNode = (Node*)start;
			firstNode->data.blockSize = totalSize;
			firstNode->next = nullptr;
			freeList.head = nullptr;
			freeList.insert(nullptr, firstNode);
		}

	}  // namespace core
}  // namespace memory