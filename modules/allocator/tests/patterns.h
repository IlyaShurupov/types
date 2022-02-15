
#pragma once

#include "collector.h"

#include <string.h>
#include <iostream>

using string = std::string;
alni hash(const string& val);

#include "map.h"

#include "imgui_utils.h"

enum class leav_pattern_type {
	LINEAR,
	RANDOM,
	SINE,
	CONST,
};

struct child_pattern {
	child_pattern() {
	}

	child_pattern(string _name) {
		name = _name;
	}

	float uppernlim = 1.f;
	float lowerlim = 0.f;
	float point = 1.f;
	string name;
};

struct pattern {
	leav_pattern_type type = leav_pattern_type::CONST;
	string pattern_name = "";

	Array<child_pattern> regions;
	bool build_in = true;

	alnf get_y(HashMap<pattern*, string>* patterns, alnf x) {
		
		assert(x <= 1.0001f && x >= -0.00001);

		if (!regions.Len()) {
			return pure_get_y(x);
		}
		
		float offset = 0.f;
		for (alni i = 0; i < regions.Len(); i++) {
			alni idx = patterns->Presents(regions[i].name);
			if (!MAP_VALID_IDX(idx)) {
				return 0.f;
			}
			pattern* child = patterns->table[idx]->val;
			assert(child);

			float range = regions[i].point * (1.f - offset);
			if (offset + range > x) {
				return regions[i].lowerlim + (child->get_y(patterns, (x - offset) / range) * (regions[i].uppernlim - regions[i].lowerlim));
			}
			offset += range;
		}
	}

	virtual float pure_get_y(float x) {
		return 0;
	}

	~pattern() {}
};


// -------------------- build-in patterns ---------------------------- //

struct const_pattern : pattern {
	float val = 0.f;
	const_pattern() {
		type = leav_pattern_type::CONST;
		pattern_name = "const";
		build_in = true;
	}
	
	float pure_get_y(float x) override {
		return val;
	}
};

struct linear_pattern : pattern {
	
	bool reversed = false;
	linear_pattern() {
		type = leav_pattern_type::LINEAR;
		pattern_name = "linear";
		build_in = true;
	}

	float pure_get_y(float x) override {
		return reversed ? 1.f - x : x;
	}
};

struct random_pattern : pattern {

	random_pattern() {
		type = leav_pattern_type::RANDOM;
		build_in = true;
	}

	float pure_get_y(float x) override {
		return randf();
	}
};

// -------------------- build-in patterns end ---------------------------- //

struct pattern_scale {
	int items = 0;
	int size = 0;
	int iterations = 0;
};

class pattern_reader : public test_pattern {

public:

	bool init(HashMap<pattern*, string>* p_patterns, pattern* p_lpattern, pattern* p_opattern, pattern* p_spattern, pattern_scale* p_scale) {
		lpattern = p_lpattern;
		opattern = p_opattern;
		spattern = p_spattern;

		scale = p_scale;

		patterns = p_patterns;
		return verify_rulles();
	}

	bool verify_rulles() {
		if (!lpattern || !opattern || !spattern) {
			return false;
		}

		bool out = true;
		out &= scale->items > 0;
		out &= scale->size > 0;
		out &= scale->iterations > 0;
		return out;
	}

	HashMap<pattern*, string>* patterns;

	pattern* lpattern = NULL;
	pattern* opattern = NULL;
	pattern* spattern = NULL;

	pattern_scale* scale = 0;

	float get_x_val(alni iter) {
		float out = iter / (float)scale->iterations;
		return out > 1 ? 1.f : out;
	}

	alni pick_size(alni iter) override {
		return scale->size * spattern->get_y(patterns, get_x_val(iter));
	}

	alni pick_alloc_count(alni iter) override {
		return scale->items * lpattern->get_y(patterns, get_x_val(iter));
	}

	alni pick_idx(alni iter) override {
		return scale->items *opattern->get_y(patterns, get_x_val(iter));
	}

	alni max_size() override {
		return scale->size;
	}

	alni max_iterations() override {
		return scale->iterations;
	}

	alni data_count() override {
		return scale->iterations;
	}
};
