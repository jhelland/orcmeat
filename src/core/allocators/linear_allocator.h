// Created by jhelland (1/10/19)
//


#ifndef __LINEAR_ALLOCATOR_H__
#define __LINEAR_ALLOCATOR_H__


#include "allocator.h"


namespace core {
	namespace memory {	

		class LinearAllocator : public Allocator {
		protected:
			void* start = nullptr;
			size_t offset;

		public:
			LinearAllocator(const size_t totalSize) : Allocator(totalSize) {};
			virtual ~LinearAllocator();

			virtual void* alloc(const size_t size, const size_t alignment = 0) override;
			virtual void dealloc(void* ptr) override;
			virtual void init() override;
			virtual void reset();

		private:
			LinearAllocator(LinearAllocator& linearAllocator);
		};

	}  // namespace memory
}  // namespace core


#endif