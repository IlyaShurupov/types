
#pragma once

#include "containers.h"

class test_pattern {
public:
	virtual alni pick_size(alni iter) { return 0; };
	virtual alni max_size() { return 0; };
	virtual alni pick_alloc_count(alni iter) { return 0; };
	virtual alni max_iterations() { return 0; };
	virtual alni pick_idx(alni iter) { return 0; };
	virtual alni data_count() { return 0; };
	
	virtual void init(alni ploading, alni pordering, alni psizing, alni pitems_count, alni pmax_item_size) {};
};

struct pattern_histogram {
	Array<alnf> alloc_size;
	Array<alnf> data_idx;
	Array<alnf> items_loaded;

	pattern_histogram(test_pattern* pt);
};

struct allocator_histogram {
	const char* alloc_type;
	
	alnf total_time;
	Array<alnf> time;
	Array<alnf> mem;
	bool failed;

	Array<uint1*> data;
	bool time_per_inst;
	bool mem_per_inst;
	
	allocator_histogram(test_pattern* pt, const char* alloc_type, bool time_per_inst, bool mem_per_inst);
	~allocator_histogram();

	void mark_resourses_usage(alni idx, alnf time, alni mem, bool add);
	void scale_all(alnf fac);
};

void collect(test_pattern* pattern, allocator* alloc, allocator_histogram* histogram);