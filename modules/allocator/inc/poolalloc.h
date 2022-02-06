#pragma once

#include "heapalloc.h"

struct chunk_list {

	struct chunk_node* first = NULL;
	alni len = 0;

	chunk_node* addchunk(heapalloc* halloc, alni bsize, alni nblocks);
	void delchunk(chunk_node* node, heapalloc* halloc);
	void initialize(heapalloc* halloc);
	void finalize(heapalloc* halloc);
};

class poolalloc : public allocator {

	class heapalloc halloc;
	chunk_list chunks;
	chunk_node* last_used;

	alni bsize;
	alni nblocks;

public:

	poolalloc(alni pbsize, alni pnblocks);
	~poolalloc();

	bool avaliable();
	alni inuse_size();
	alni reserved_size();

#ifdef MEM_TRACE
	void* alloc(alni size, const char* file, int line);
#else
	void* alloc(alni size);
#endif 
	void free(void* p);
};