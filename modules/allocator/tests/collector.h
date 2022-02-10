
#pragma once

#include "containers.h"

struct test_pattern {
	virtual alni pick_size(alni iter) { return 0; };
	virtual alni max_size() { return 0; };
	virtual alni pick_alloc_count(alni iter) { return 0; };
	virtual alni max_iterations() { return 0; };
	virtual alni pick_idx(alni iter) { return 0; };
	virtual alni data_count() { return 0; };
};

struct allocator_histogram {
	const char* alloc_type;
	Array<alnf> time;
	Array<alnf> mem;
	Array<uint1*> data;
	allocator_histogram(test_pattern* pt, const char* alloc_type);
	~allocator_histogram();
};

void collect(test_pattern* pattern, allocator* alloc, allocator_histogram* histogram);