#pragma once

#include "allocator.h"

class heapalloc : public allocator {

	alni num = 0;

#ifdef MEM_TRACE
	struct MemHead* entry_ptr = nullptr;
#endif

public:

	alni reserved_size() { return inuse_size(); }
	bool avaliable() { return true; }
	
	bool wrap_support() { return true; };
	bool wrap_corrupted();

#ifdef MEM_TRACE
	void* alloc(alni size, const char* file, int line);
	void* alloc(alni size) {
		return alloc(size, NULL, 0);
	}
#else
	void* alloc(alni size);
#endif 

	void free(void* p);

	alni inuse_size();
};
