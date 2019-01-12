// Created by jhelland (1/11/19)
//


#include "memory_manager.h"


namespace core {
	namespace memory {

		StackAllocator* programStackAllocator;  // global allocator, only one declaration

		namespace {
			static void* programMemory;
		}

		void alloc_program_memory(const size_t size) {
			programMemory = malloc(size);
			programStackAllocator = new StackAllocator(size);
			programStackAllocator->init(programMemory);
		}

		void free_program_memory() {
			free(programMemory);
			delete programStackAllocator;
		}

	}  // namespace memory
}  // namespace core
