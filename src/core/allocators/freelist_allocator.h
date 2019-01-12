// Created by jhelland (1/10/19)
//


#ifndef __FREELIST_ALLOCATOR_H__
#define __FREELIST_ALLOCATOR_H__


#include "allocator.h"
#include "../../utils/data_structures/singly_linked_list.h"


namespace core {
	namespace memory {

		class FreeListAllocator : public Allocator {
		public:
			enum PlacementPolicy {
				FIND_FIRST,
				FIND_BEST
			};

		private:
			struct FreeHeader { 
				size_t blockSize; 

				inline friend bool operator<(const FreeHeader& l, const FreeHeader& r) { return l.blockSize < r.blockSize; }
			};
			struct AllocationHeader { 
				size_t blockSize; 
				char padding; 
			};

			typedef SinglyLinkedList<FreeHeader>::Node Node;

			void* start = nullptr;
			PlacementPolicy placementPolicy;
			SinglyLinkedList<FreeHeader> freeList;

		public:
			FreeListAllocator(const size_t totalSize, const PlacementPolicy policy);
			virtual ~FreeListAllocator();

			virtual void* alloc(const size_t size, const size_t alignment = 16) override;
			virtual void dealloc(void* ptr) override;
			virtual void init() override;
			virtual void reset();

		private:
			FreeListAllocator(FreeListAllocator& freelistAllocator);

			void coalesce(Node* prevBlock, Node* freeBlock);

			void find(const size_t size, const size_t alignment, size_t& padding, Node*& prevNode, Node*& foundNode);
			void find_best(const size_t size, const size_t alignment, size_t& padding, Node*& prevNode, Node*& foundNode);
			void find_first(const size_t size, const size_t alignment, size_t& padding, Node*& prevNode, Node*& foundNode);
		};

	}  // namespace memory
}  // namespace core


#endif  // __FREELIST_ALLOCATOR_H__
