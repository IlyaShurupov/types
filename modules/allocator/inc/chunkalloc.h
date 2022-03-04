#pragma once

#include "allocator.h"

struct unused_slot_head {
	alni* bnext;
};

struct used_slot_head {
	class chunkalloc* chunk_p;
};

alni calc_bsize(alni bsize);

class chunkalloc : public allocator {
	
	alni bsize = 0;
	alni nblocks = 0;

	alni* buff = NULL;
	alni* bnext = NULL;
	alni bfreec = 0;
	alni binitc = 0;

public:

	chunkalloc(alni bsize, alni nblocks);
	~chunkalloc();

	bool avaliable();
	alni inuse_size();
	alni reserved_size();
	bool is_empty();

	inline void* get_addr(alni idx) const;
	inline alni get_idx(const void* address) const;
	alni get_bsize();

	void* alloc(alni size);
	virtual void free(void* p);

	bool wrap_corrupted() { return false; }
};