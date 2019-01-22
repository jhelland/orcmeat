// Created by jratzlaff (1/8/19)
//		Edited by jhelland (1/9/19)
//


#include <stdlib.h>  // malloc, free
#include <assert.h>
#include <algorithm>
#ifdef _DEBUG
#include <iostream>
#endif

#include "pool_allocator.h"
#include "../memory_manager.h"


namespace core {
	namespace memory {

		PoolAllocator::PoolAllocator(const size_t totalSize, const size_t chunkSize) : Allocator(totalSize) {
			assert(chunkSize >= 8 && "Chunk size must be >= 8");
			assert(totalSize % chunkSize == 0 && "Total size must be a multiple of chunk size");

			this->chunkSize = chunkSize;
		}


		PoolAllocator::~PoolAllocator() {
			
		}


		void PoolAllocator::init() {
			start = programStackAllocator->alloc(totalSize, sizeof(size_t));
			reset();
		}


		void* PoolAllocator::alloc(const size_t allocSize, const size_t alignment) {
			assert(allocSize <= chunkSize && "Allocation size must be <= chunk size");

			Node* freePosition = freeList.pop();

			assert(freePosition != nullptr && "The pool allocator is full");

			used += chunkSize;
			peak = std::max(peak, used);
#ifdef _DEBUG
			std::cout << "ALLOC" << "\t@S " << start << "\t@R " << (void*)freePosition << "\tM " << used << std::endl;
#endif
			return (void*)freePosition;
		}


		void PoolAllocator::dealloc(void* ptr) {
			used -= chunkSize;
			freeList.push((Node*)ptr);
#ifdef _DEBUG
			std::cout << "FREED" << "\t@S " << start << "\t@F " << ptr << "\tM " << used << std::endl;
#endif
		}


		void PoolAllocator::reset() {
			used = 0;
			peak = 0;
			
			// Create linked list w/ all free blocks
			const int numChunks = totalSize / chunkSize;
			for (int i = 0; i < numChunks; ++i) {
				size_t addr = (size_t)start + i * chunkSize;
				freeList.push((Node*)addr);
			}
		}

	}  // namespace memory 
}  // namespace core


/*
		PoolAllocator::PoolAllocator(size_t numObjs, size_t unitSize) : unitSize(unitSize) {
			capacity = numObjs * unitSize;
			if (capacity % unitSize != 0) {
				capacity -= capacity % unitSize;
			}

			memStart = (void**)malloc(capacity);
			char* temp = (char*)memStart;
			for (int i = 0; i < capacity / unitSize; i++) {
				*((void**)&(temp[i*unitSize])) = (void*)(&temp[(i + 1)*unitSize]);
			}
			*((void**)&temp[capacity - unitSize]) = nullptr;
			available = (void**)memStart;
		}  // constructor


		void* PoolAllocator::allocate() {
			if (available == nullptr)
				return nullptr;

			void *ret = (void*)available;
			available = (void**)(*available);
			return ret;
		}  // void allocate


		void PoolAllocator::free(void* block) {
			last = (void**)block;
			*last = nullptr;
			if (available == nullptr) available = last;
		}
		*/
