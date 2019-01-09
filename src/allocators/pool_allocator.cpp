#include "pool_allocator.h"

PoolAllocator::PoolAllocator(size_t capacity, size_t unit_size)
{
	if (capacity % unit_size != 0) {
		capacity -= capacity % unit_size;
	}
	unit_size = std::max(unit_size, sizeof(void*));
	mem_start = (void**)malloc(capacity);
	char* temp = (char*)mem_start;
	for (int i = 0; i < capacity / unit_size; i++) {
		*((void**)&(temp[i*unit_size])) = (void*)(&temp[(i + 1)*unit_size]);
	}
	*((void**)&temp[capacity-unit_size]) = nullptr;
	available = (void**)mem_start;
}

void * PoolAllocator::allocate()
{
	if (available == nullptr) return nullptr;
	void *ret = (void*)available;
	available = (void**)(*available);
	return ret;
}


void PoolAllocator::free(void* block)
{
	last = (void**)block;
	*last = nullptr;
	if (available == nullptr) available = last;
}
