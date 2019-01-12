// Created by jratzlaff (1/8/19)
//
// Edited by jhelland (1/9/19)
//
// DESCRIPTION:
//	Custom memory pool allocator to avoid heap allocations for objects
//
// TODO:
//	- If attempt to allocate beyond current available memory, automatically add more? 
//


#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H


#include "allocator.h"


namespace core {
	namespace memory {

		template<typename T>
		class StackLinkedList {
		private:
			StackLinkedList(StackLinkedList& stackLinkedList);

		public:
			struct Node {
				T data;
				Node* next;
			};
			Node* head;

		public:
			StackLinkedList() {}

			void push(Node* node) {
				node->next = head;
				head = node;
			}

			Node* pop() {
				Node* top = head;
				head = head->next;
				return top;
			}
		};


		class PoolAllocator : public Allocator {
		private:
			struct FreeHeader {};
			typedef StackLinkedList<FreeHeader>::Node Node;
			StackLinkedList<FreeHeader> freeList;

			void* start = nullptr;
			size_t chunkSize;

		public:
			PoolAllocator(const size_t totalSize, const size_t chunkSize);
			virtual ~PoolAllocator();

			virtual void* alloc(const size_t size, const size_t alignment = 0) override;
			virtual void dealloc(void* ptr) override;
			virtual void init() override;
			virtual void reset();

		private:
			PoolAllocator(PoolAllocator& poolAllocator);
		};

	}  // namespace memory
}  // namespace core


#endif


/*
		class PoolAllocator {
		private:
			void** available;
			void** last;
			void** memStart;
			size_t capacity;  //Capacity in bytes
			size_t unitSize;

		public:
			PoolAllocator(size_t numObjs, size_t unitSize);

			void* allocate();
			void free(void* block);
		};  // class PoolAllocator
		*/
