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

	chunkalloc(allocator* alloc, alni bsize, alni nblocks);

	void finalize(allocator* alloc);

	bool avaliable();
	alni inuse_size();
	alni reserved_size();

	inline void* get_addr(alni idx) const;
	inline alni get_idx(const void* address) const;
	alni get_bsize();

#ifdef MEM_TRACE
	void* alloc(alni size, const char* file, int line);
#else
	void* alloc(alni size);
#endif 
	void free(void* p);
};