#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>

#define GROW_CAPACITY(capacity) \
	((capacity) < 8 ? 8 : (capacity) * 2)

#define SHRINK_CAPACITY(capacity) \
	((capacity) < 16 ? 8 : (capacity / 2))

#define GROW_ARRAY(type, pointer, oldCount, newCount) \
	(type*)reallocate(pointer, sizeof(type) * (oldCount), sizeof(type) * (newCount))

#define GROW_ARRAY_SIZE(size, pointer, oldCount, newCount) \
	(void*)reallocate(pointer, size * (oldCount), size * (newCount))

#define FREE_ARRAY(type, pointer, oldCount) \
	reallocate(pointer, sizeof(type) * oldCount, 0)

#define FREE_ARRAY_SIZE(size, pointer, oldCount) \
	reallocate(pointer, size * oldCount, 0)

void* reallocate(void* pointer, size_t oldSize, size_t newSize);


#endif
