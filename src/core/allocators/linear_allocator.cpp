// Created by jhelland (1/10/19)
//


#include <stdlib.h>  // malloc, free
#include <assert.h>  // assert
#include <algorithm>  // max
#ifdef _DEBUG
#include <iostream>
#endif

#include "linear_allocator.h"
#include "../memory_manager.h"


namespace core {
	namespace memory {

		LinearAllocator::~LinearAllocator() {
			//free(start);
			start = nullptr;
		}


		void LinearAllocator::init() {
			//if (start != nullptr)
				//free(start);

			//start = malloc(totalSize);
			start = programStackAllocator->alloc(totalSize, sizeof(size_t));
			offset = 0;
		}


		void* LinearAllocator::alloc(const size_t size, const size_t alignment) {
			size_t padding = 0;
			size_t paddedAddr = 0;
			const size_t currentAddr = (size_t)start + offset;

			if (alignment != 0 && offset % alignment != 0)
				padding = utils::calc_padding(currentAddr, alignment);

			if (offset + padding + size > totalSize)
				return nullptr;

			offset += padding;
			const size_t nextAddr = currentAddr + padding;
			offset += size;
#ifdef _DEBUG
			std::cout << "A" << "\t@C " << (void*)currentAddr << "\t@R " << (void*)nextAddr << "\tO " << offset << "\tP " << padding << std::endl;
#endif
			used = offset;
			peak = std::max(peak, used);

			return (void*)nextAddr;
		}


		void LinearAllocator::dealloc(void* ptr) {
			assert(false && "Use reset() method");
		}


		void LinearAllocator::reset() {
			offset = 0;
			used = 0;
			peak = 0;
		}

	}  // namespace memory
}  // namespace core
