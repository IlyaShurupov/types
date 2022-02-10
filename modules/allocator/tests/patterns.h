
#pragma once

#include "collector.h"

struct test_pattern_basic : test_pattern {

	alni items_count = 100;
	alni item_size = 100;

	alni pick_size(alni iter) {
		return item_size;
	}

	alni pick_alloc_count(alni iter) {
		return iter;
	}

	alni pick_idx(alni iter) {
		return iter;
	}

	alni max_size() {
		return item_size;
	}

	alni iterations_length() {
		return items_count;
	}

	alni data_count() {
		return items_count;
	}
};
