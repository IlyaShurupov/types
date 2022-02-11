
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
	alni update_counter = 0;
	bool time_per_instruction = 0;
	bool mem_per_instruction = 0;
	int avreging;

	// patterns
	load_type loading = load_type::LINEAR;
	order_type ordering = order_type::LINEAR;
	int max_data_items_count = 0;
	int max_data_item_size = 0;

	// allocators
	int chunk_bsize = 0;
	int chunk_blen = 0;

	int pool_bsize = 0;
	int pool_blen = 0;

	bool operator==(const config& in) {
		return memequal(this, (config*)(&in), sizeof(config));
	}
};


struct banchmarker {

	config cfg;
	
	heapalloc ownheap;

	// allocators
	heapalloc* halloc = NULL;
	poolalloc* palloc = NULL;
	chunkalloc* calloc = NULL;
	heapalloc* chunk_heap = NULL;

	Array<allocator_histogram*> out;
	pattern_histogram* pattern_out;

	alni i_count;
	alnf* x_axis = NULL;

	banchmarker();
	~banchmarker();

	test_pattern* get_pattern(config& cfg);

	void analize(config cfg);

	void draw();

	void init_allocators(config& cfg);

	void dest_allocators();

	void clear_out();
	void reserve_out(test_pattern* pattern);
};