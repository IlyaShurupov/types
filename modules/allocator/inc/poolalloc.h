#pragma once

#include "heapalloc.h"

struct chunk_list {

	struct chunk_node* first = NULL;

	chunk_node* addchunk(heapalloc* halloc, alni bsize, alni nblocks);
	void delchunk(chunk_node* node, heapalloc* halloc);
	void initialize(heapalloc* halloc);
	void finalize(heapalloc* halloc);
};

extern heapalloc pool_halloc;

class poolalloc : public allocator {

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

	void* alloc(alni size, const char* file, int line);
	void* alloc(alni size);

	void free(void* p);

	bool wrap_corrupted() { return true; }
};