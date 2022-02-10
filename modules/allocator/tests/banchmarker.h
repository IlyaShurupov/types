
#pragma once

#include "patterns.h"

#include "allocators.h"

enum class load_type {
	LINEAR,
	SINE,
	STEPS,
	RANDOM
};

enum class order_type {
	LINEAR,
	LINEAR_REVERSED,
	RANDOM,
};

struct config {
	// general
	int avreging;

	// patterns
	load_type loading;
	order_type ordering;
	int max_data_items_count;
	int max_data_item_size;

	// allocators
	int chunk_bsize;
	int chunk_blen;

	int pool_bsize;
	int pool_blen;

	bool operator==(const config& in) {
		return memequal(this, (config*)(&in), sizeof(config));
	}
};


struct banchmarker {

	config cfg;
	
	heapalloc ownheap;

	// allocators
	heapalloc* halloc;
	poolalloc* palloc;
	chunkalloc* calloc;
	heapalloc* chunk_heap;

	Array<allocator_histogram*> out;

	banchmarker();

	test_pattern get_pattern(config& cfg);

	void analize(config cfg);

	void draw();

	void init_allocators(config& cfg);

	void dest_allocators();
};