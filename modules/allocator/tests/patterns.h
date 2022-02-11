
#pragma once

#include "collector.h"

class test_pattern_basic : public test_pattern {

public:
	alni items_count = 100;
	alni item_size = 60;

	alni pick_size(alni iter) override {
		return (alni)(randf() * item_size);
	}

	alni pick_alloc_count(alni iter) override {
		return iter;
	}

	alni pick_idx(alni iter) override {
		return iter;
	}

	alni max_size() override {
		return item_size;
	}

	alni max_iterations() override {
		return items_count;
	}

	alni data_count() override {
		return items_count;
	}
};
