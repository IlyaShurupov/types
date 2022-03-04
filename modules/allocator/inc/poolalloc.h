#pragma once

#include "heapalloc.h"

struct chunk_list {

	struct chunk_node* last = NULL;

	chunk_node* addchunk(class poolalloc* self, alni bsize, alni nblocks);
	void delchunk(chunk_node* node);
	void initialize();
	void finalize();
};

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
	bool is_empty();

	void* alloc(alni size);

	void free(void* p);

	bool wrap_corrupted() { return false; }
};