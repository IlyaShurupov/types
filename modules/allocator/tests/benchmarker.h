
#pragma once

#include "patterns.h"

#include "allocators.h"
#include "array.h"

#include "ImGuiClass.h"

#include "gl.h"
#include "glcommon.h"
#include "window.h"

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
	bool live_update = false;

	bool heap = true;
	bool pool = true;
	bool chunk = true;

	// pattern
	pattern* current_sizing_pattern = 0;
	pattern* current_loading_pattern = 0;
	pattern* current_ordering_pattern = 0;
	pattern_scale pt_scale;

	tp::alni update = 0;
	bool time_per_instruction = 0;
	bool mem_per_instruction = 0;
	int avreging;

	// allocators
	int chunk_bsize = 0;
	int chunk_blen = 0;

	int pool_bsize = 0;
	int pool_blen = 0;

	bool operator==(const config& in) {
		return tp::memequal(this, (config*)(&in), sizeof(config));
	}
};

struct benchmarker : public ImGui::CompleteApp {

	config cfg;
	bool is_output = false;

	// allocators
	tp::HeapAlloc* halloc = NULL;
	tp::PoolAlloc* palloc = NULL;
	tp::ChunkAlloc* calloc = NULL;


	tp::Array<allocator_histogram*> out;
	pattern_histogram* pattern_out;

	tp::alni i_count;
	tp::alnf* x_axis = NULL;

	tp::HashMap<pattern*, tp::string> patterns;

	pattern_reader pattern_analizer;

	const char* pattern_generator_active = NULL;

	benchmarker();
	~benchmarker();

	test_pattern* get_pattern(config& cfg);

	void pattern_combo(const char*&);
	void analize(config cfg);
	void select_pattern();
	void output_draw();
	
	void MainDrawTick();

	void pattern_generator();

	void init_allocators(config& cfg);

	void dest_allocators();

	void clear_out();
	void reserve_out(test_pattern* pattern);
};