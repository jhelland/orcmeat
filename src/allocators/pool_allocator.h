#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H
#include <algorithm>
class PoolAllocator {

public:
	PoolAllocator(size_t capacity, size_t unit_size);
	void *allocate();
	void free(void *block);
private:
	void** available;
	void** last;
	void **mem_start;
	size_t capacity;//Capacity in bytes
	size_t unit_size;
	
};

#endif