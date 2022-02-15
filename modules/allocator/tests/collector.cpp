
#include "collector.h"

#include <chrono>

allocator_histogram::allocator_histogram(test_pattern* pt, const char* alloc_type, bool p_time_per_inst, bool p_mem_per_inst) {
	this->alloc_type = alloc_type;
	time_per_inst = p_time_per_inst;
	mem_per_inst = p_mem_per_inst;

	if (time_per_inst) {
		time.Reserve(pt->max_iterations());
	}
	if (mem_per_inst) {
		mem.Reserve(pt->max_iterations()); 
	}
	data.Reserve(pt->data_count());

	total_time = 0;
	failed = false;
}

void allocator_histogram::scale_all(alnf fac) {
	for (auto& iter : time) {
		iter.data() = fac * iter.data();
	}
	for (auto& iter : mem) {
		iter.data() = fac * iter.data();
	}
	total_time = fac * total_time;
}

allocator_histogram::~allocator_histogram() {
}

void allocator_histogram::mark_resourses_usage(alni idx, alnf p_time, alni p_mem, bool add) {
	if (mem.Len() > idx)
		add ? mem[idx] += (alnf)p_mem : mem[idx] = (alnf)p_mem;
	if (time.Len() > idx)
		add ? time[idx] += p_time : time[idx] = p_time;
}

bool execute_instruction(allocator* alloc, bool load, alni size, uint1*& data) {
	bool failed = false;
	try {
		if (load) {
			if (!data) {
				data = (uint1*)alloc->alloc(size);
				if (!data) {
					failed = true;
				}
				else {
					data = 0;
				}
			}
		}
		else {
			if (data) {
				alloc->free(data);
				data = NULL;
			}
		}
	}
	catch (...) {
		failed = true;
	}
	return !failed;
}



void collect(test_pattern* pattern, allocator* alloc, allocator_histogram* histogram) {

	alni iter_idx = 0;
	alni nimtems_loaded = 0;

	auto total_st = std::chrono::high_resolution_clock::now();

	for (iter_idx = 0; iter_idx < pattern->max_iterations(); iter_idx++) {

		alni target_nimtems_loaded = pattern->pick_alloc_count(iter_idx);
		alni data_idx = pattern->pick_idx(iter_idx);
		alni load_size = pattern->pick_size(iter_idx);
		
		auto iter_st = std::chrono::high_resolution_clock::now();
		while (nimtems_loaded != target_nimtems_loaded) {
			bool load = nimtems_loaded < target_nimtems_loaded;
			if (!execute_instruction(alloc, load, load_size, histogram->data[data_idx])) {
				histogram->mark_resourses_usage(iter_idx, 0, 0, 0);
				histogram->failed = true;
				break;
			}
			nimtems_loaded += (alni)load + (-1 * (alni)(!load));
		}
		auto iter_nd = std::chrono::high_resolution_clock::now();
		alni dur = std::chrono::duration_cast<std::chrono::nanoseconds>(iter_nd - iter_st).count();
		histogram->mark_resourses_usage(iter_idx, alnf(dur), alloc->reserved_size(), 1);
	}

	// clear all out
	for (iter_idx = 0; iter_idx < pattern->max_iterations(); iter_idx++) {
		if (histogram->data[iter_idx]) {
			execute_instruction(alloc, false, 0, histogram->data[iter_idx]);
		}
	}

	auto total_nd = std::chrono::high_resolution_clock::now();
	alni dur = std::chrono::duration_cast<std::chrono::nanoseconds>(total_nd - total_st).count();
	histogram->total_time += dur;
}

pattern_histogram::pattern_histogram(test_pattern* pt) {
	alloc_size.Reserve(pt->max_iterations());
	data_idx.Reserve(pt->max_iterations());
	items_loaded.Reserve(pt->max_iterations());

	for (auto& i : range(0, pt->max_iterations())) {
		alloc_size[i.idx] = (alnf)pt->pick_size(i);
		data_idx[i.idx] = (alnf)pt->pick_idx(i);
		items_loaded[i.idx] = (alnf)pt->pick_alloc_count(i);
	}
}
