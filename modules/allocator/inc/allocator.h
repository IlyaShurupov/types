#pragma once

#include "common.h"

#include "alloc_cfg.h"

class allocator {
	
public:

	virtual bool avaliable() = 0;
	virtual alni reserved_size() = 0;
	virtual alni inuse_size() { return 0; };

#ifdef MEM_TRACE
	virtual void* alloc(alni size, const char* file, int line) = 0;
#else
	virtual void* alloc(alni size) = 0;
#endif 
	virtual void free(void* p) = 0;

	~allocator() {
		assert(!inuse_size() && "memory leaks detected");
	}
};
