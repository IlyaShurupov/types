
#pragma once

#include "array.h"

class test_pattern {
public:
	virtual tp::alni pick_size(tp::alni iter) { return 0; };
	virtual tp::alni max_size() { return 0; };
	virtual tp::alni pick_alloc_count(tp::alni iter) { return 0; };
	virtual tp::alni max_iterations() { return 0; };
	virtual tp::alni pick_idx(tp::alni iter) { return 0; };
	virtual tp::alni data_count() { return 0; };
};

struct pattern_histogram {
	tp::Array<tp::alnf> alloc_size;
	tp::Array<tp::alnf> data_idx;
	tp::Array<tp::alnf> items_loaded;

	pattern_histogram(test_pattern* pt);
};

struct allocator_histogram {
	const char* alloc_type;
	
	tp::alnf total_time;
	tp::Array<tp::alnf> time;
	tp::Array<tp::alnf> mem;
	bool failed;

	tp::Array<tp::uint1*> data;
	bool time_per_inst;
	bool mem_per_inst;
	
	allocator_histogram(test_pattern* pt, const char* alloc_type, bool time_per_inst, bool mem_per_inst);
	~allocator_histogram();

	void mark_resourses_usage(tp::alni idx, tp::alnf time, tp::alni mem, bool add);
	void scale_all(tp::alnf fac);
};

void collect(test_pattern* pattern, tp::AbstractAllocator* alloc, allocator_histogram* histogram);