// Created by jhelland (1/10/19)
//


#include <stdlib.h>  // malloc, free
#include <algorithm>  // max
#ifdef _DEBUG
#include <iostream>
#endif

#include "stack_allocator.h"


namespace core {
	namespace memory {

		StackAllocator::~StackAllocator() {
			//free(start);
			start = nullptr;
		}


		void StackAllocator::init(void* block) {
			//if (start != nullptr)
			//	free(start);

			//start = malloc(totalSize);
			start = block;
			offset = 0;
		}


		void* StackAllocator::alloc(const size_t size, const size_t alignment) {
			const size_t currentAddr = (size_t)start + offset;
			size_t padding = utils::calc_padding_header(currentAddr, alignment, sizeof(AllocationHeader));

			if (offset + padding + size > totalSize)
				return nullptr;
			offset += padding;

			const size_t nextAddr = currentAddr + padding;
			const size_t headerAddr = nextAddr - sizeof(AllocationHeader);
			AllocationHeader allocationHeader{ static_cast<char>(padding) };
			AllocationHeader* headerPtr = (AllocationHeader*)headerAddr;
			headerPtr = &allocationHeader;

			offset += size;
#ifdef _DEBUG
			std::cout << "F" << "\t@C " << (void*)currentAddr << "\t@F " << (void*)nextAddr << "\tO " << offset << "\tP " << padding << std::endl;
#endif
			used = offset;
			peak = std::max(peak, used);

			return (void*)nextAddr;
		}


		void StackAllocator::dealloc(void* ptr) {
			const size_t currentAddr = (size_t)ptr;
			const size_t headerAddr = currentAddr - sizeof(AllocationHeader);
			const AllocationHeader* allocationHeader{ (AllocationHeader*)headerAddr };

			offset = currentAddr - allocationHeader->padding - (size_t)start;
			used = offset;
#ifdef _DEBUG
			std::cout << "F" << "\t@C " << (void*)currentAddr << "\t@F " << (void*)((char*)start + offset) << "\tO " << offset << std::endl;
#endif
		}


		void StackAllocator::reset() {
			offset = 0;
			used = 0;
			peak = 0;
		}

	}  // namespace memory
}  // namespace core
