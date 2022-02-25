#pragma once

#include "allocator.h"

struct MemHead {
#ifdef MEM_TRACE
	MemHead* next;
	MemHead* prev;
#endif
	alni size;
	alni reserved;
	allocator* alloc;
};


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

	void* alloc(alni size);
	void free(void* p);

	alni inuse_size();
};
