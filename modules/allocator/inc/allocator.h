#pragma once

#include "common.h"

#include "alloc_cfg.h"

class allocator {
	
public:

	virtual bool avaliable() = 0;
	virtual alni reserved_size() = 0;
	virtual alni inuse_size() { return 0; };

	virtual bool wrap_support() { return false; };
	virtual bool wrap_corrupted() = 0;

	virtual void* alloc(alni size) = 0;
	virtual void free(void* p) = 0;

	~allocator() {
	}
};
