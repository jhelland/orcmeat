// Created by jhelland (1/10/19)
//


#ifndef __STACK_ALLOCATOR_H__
#define __STACK_ALLOCATOR_H__


#include "allocator.h"


namespace core {
	namespace memory {

		class StackAllocator : public Allocator {
		protected:
			void* start = nullptr;
			size_t offset;

		public:
			StackAllocator(const size_t totalSize) : Allocator(totalSize) {}
			virtual ~StackAllocator();

			virtual void* alloc(const size_t size, const size_t alignment = 0) override;
			virtual void dealloc(void* ptr) override;
			virtual void init() override {}
			void init(void* block);
			virtual void reset();

		private:
			StackAllocator(StackAllocator& stackAllocator);

			struct AllocationHeader {
				char padding;
			};
		};

	}  // namespace memory 
}  // namespace core


#endif  // __STACK_ALLOCATOR_H__