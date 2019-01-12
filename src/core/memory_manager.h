// Created by jhelland (1/11/19)
//


#ifndef __MEMORY_MANAGER_H__
#define __MEMORY_MANAGER_H__


#include <stdlib.h>

#include "allocators/stack_allocator.h"


namespace core {
	namespace memory {

		extern StackAllocator* programStackAllocator;

		void alloc_program_memory(const size_t size);

		void free_program_memory();

	}  // namespace memory
}  // namespace core


#endif