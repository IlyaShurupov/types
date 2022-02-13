
#pragma once

#include "collector.h"

class test_pattern_basic : public test_pattern {

public:

	void init(alni ploading, alni pordering, alni psizing, alni pitems_count, alni pmax_item_size) {
		loading = ploading;
		ordering = pordering;
		sizing = psizing;
		items_count = pitems_count;
		max_item_size = pmax_item_size;
	}

	alni loading = 0;
	alni ordering = 0;
	alni sizing = 0;
	alni items_count = 100;
	alni max_item_size = 60;

	alni pick_size(alni iter) override {
		if (sizing == 0) {
			return max_item_size;
		}
		if (sizing == 1) {
			CLAMP(iter, 1, max_item_size);
			return iter;
		}
		if (sizing == 2) {
			iter = max_item_size - iter;
			CLAMP(iter, 1, max_item_size);
			return iter;
		}
		iter = (alni)(randf() * max_item_size);
		CLAMP(iter, 1, max_item_size);
		return iter;
	}

	alni pick_alloc_count(alni iter) override {
		if (iter < items_count)
			return iter;
		return items_count - iter;
	}

	alni pick_idx(alni iter) override {
		if (iter >= items_count)
			return iter -= items_count;

		if (ordering == 0) {
			CLAMP(iter, 0, items_count);
			return iter;
		}
		if (ordering == 1) {
			iter = max_item_size - iter;
			CLAMP(iter, 0, items_count);
			return iter;
		}
		iter = (alni)(randf() * items_count);
		CLAMP(iter, 0, items_count);
		return iter;
	}

	alni max_size() override {
		return max_item_size;
	}

	alni max_iterations() override {
		return items_count;
	}

	alni data_count() override {
		return items_count;
	}
};
