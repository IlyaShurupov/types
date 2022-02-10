
#include "collector.h"

#include <chrono>

allocator_histogram::allocator_histogram(test_pattern* pt, const char* alloc_type) {
	this->alloc_type = alloc_type;
	time.Reserve(pt->max_iterations());
	mem.Reserve(pt->max_iterations());
	data.Reserve(pt->data_count());
}

allocator_histogram::~allocator_histogram() {
}

bool execute_instruction(allocator* alloc, bool load, alni size, uint1*& data) {
	bool failed = false;
	try {
		if (load) {
			data = (uint1*)alloc->alloc(size);
			if (!data) {
				failed = true;
			}
		}
		else {
			alloc->free(data);
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

	for (iter_idx = 0; iter_idx < pattern->max_iterations(); iter_idx++) {

		alni target_nimtems_loaded = pattern->pick_alloc_count(iter_idx);
		alni data_idx = pattern->pick_idx(iter_idx);
		alni load_size = pattern->pick_size(iter_idx);
		bool load = nimtems_loaded < target_nimtems_loaded;

		if (nimtems_loaded == target_nimtems_loaded) {
			if (iter_idx > 0) {
				histogram->time[iter_idx] += histogram->time[iter_idx - 1];
				histogram->mem[iter_idx] += histogram->mem[iter_idx - 1];
			}
			else {
				histogram->time[iter_idx] = 0;
				histogram->mem[iter_idx] = 0;
			}
		}
		else {

			auto iter_st = std::chrono::high_resolution_clock::now();

			if (execute_instruction(alloc, load, load_size, histogram->data[data_idx])) {

				auto iter_nd = std::chrono::high_resolution_clock::now();
				alni dur = std::chrono::duration_cast<std::chrono::nanoseconds>(iter_nd - iter_st).count();

				histogram->time[iter_idx] += alnf(dur);
				histogram->mem[iter_idx] += alloc->reserved_size();
			}
			else {
				histogram->time[iter_idx] = 0;
				histogram->mem[iter_idx] = 0;
			}
		}

		nimtems_loaded += (alni)load + (-1 * (alni)(!load));
	}
}
