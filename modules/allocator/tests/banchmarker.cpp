
#include "implot.h"
#include "imgui.h"
#include "imgui_utils.h"

#include "banchmarker.h"

config glb_cfg = {

	.heap = true,
	.pool = true,
	.chunk = true,

	.update_counter = 0,

	.time_per_instruction = true,
	.mem_per_instruction = true,
	.avreging = 1,

	.loading = 0,
	.ordering = 0,
	.sizing = 0,

	.max_data_items_count = 100,
	.max_data_item_size = 32,

	.chunk_bsize = 100,
	.chunk_blen = 100,

	.pool_bsize = 100,
	.pool_blen = 100,
};

test_pattern_basic pattern_basic;

banchmarker::banchmarker() {
	i_count = 0;
	pattern_out = NULL;
	out.Reserve(3);
}

banchmarker::~banchmarker() {
	clear_out();
}

test_pattern* banchmarker::get_pattern(config& cfg) {
	return &pattern_basic;
}

void banchmarker::analize(config pcfg) {

	if (this->cfg == pcfg) {
		return;
	}
	
	this->cfg = pcfg;

	clear_out();

	test_pattern* pattern = get_pattern(pcfg);
	pattern->init(cfg.loading, cfg.ordering, cfg.sizing, cfg.max_data_items_count, cfg.max_data_item_size);
	reserve_out(pattern);

	for (alni iter = 0; iter < pcfg.avreging; iter++) {
		init_allocators(pcfg);
		if (cfg.heap) collect(pattern, halloc, out[0]);
		if (cfg.pool) collect(pattern, palloc, out[1]);
		if (cfg.chunk) collect(pattern, calloc, out[2]);
		dest_allocators();
	}

	for (auto& i : range(0, 3)) {
		if (out[i]) out[i]->scale_all(1.f / pcfg.avreging);
	}

	i_count = pattern->max_iterations();
	if (x_axis) {
		ownheap.free(x_axis);
		x_axis = NULL;
	}
	x_axis = (alnf*)ownheap.alloc(sizeof(alnf) * i_count);
	for (alni iter = 0; iter < i_count; iter++) {
		x_axis[iter] = (alnf)iter;
	}
}

void banchmarker::init_allocators(config& pcfg) {
	halloc = NULL;
	palloc = NULL;
	chunk_heap = NULL;
	calloc = NULL;

	if (cfg.heap) halloc = new(&ownheap) heapalloc();
	if (cfg.pool) palloc = new(&ownheap) poolalloc(pcfg.pool_bsize, pcfg.pool_blen);
	if (cfg.chunk) chunk_heap = new(&ownheap) heapalloc();
	if (cfg.chunk) calloc = new(chunk_heap) chunkalloc(chunk_heap, pcfg.chunk_bsize, pcfg.chunk_blen);
}

void banchmarker::dest_allocators() {
	if (cfg.heap) mfree(&ownheap, halloc);
	if (cfg.pool) mfree(&ownheap, palloc);

	if (cfg.chunk) calloc->finalize(chunk_heap);
	if (cfg.chunk) mfree(chunk_heap, calloc);
	if (cfg.chunk) mfree(&ownheap, chunk_heap);
}

void banchmarker::clear_out() {
	if (out[0]) { mfree(&ownheap, out[0]); out[0] = 0; };
	if (out[1]) { mfree(&ownheap, out[1]); out[1] = 0; };
	if (out[2]) { mfree(&ownheap, out[2]); out[2] = 0; };
	if (pattern_out) { mfree(&ownheap, pattern_out); pattern_out = 0; };
}

void banchmarker::reserve_out(test_pattern* pattern) {
	pattern_out = new(&ownheap) pattern_histogram(pattern);
	if (cfg.heap) out[0] = new(&ownheap) allocator_histogram(pattern, "heap", cfg.time_per_instruction, cfg.mem_per_instruction);
	if (cfg.pool) out[1] = new(&ownheap) allocator_histogram(pattern, "pool", cfg.time_per_instruction, cfg.mem_per_instruction);
	if (cfg.chunk) out[2] = new(&ownheap) allocator_histogram(pattern, "chunk", cfg.time_per_instruction, cfg.mem_per_instruction);
}

void banchmarker::pattern_generator() {
	if (EditorWindow("Pattern Generator")) {
		{
			const char* unload_patterns[] = { "last-first", "first-last", "random" };
			static const char* current_unload_pattern = unload_patterns[0];
			if (ImGui::BeginCombo("data order", current_unload_pattern)) {
				for (int n = 0; n < IM_ARRAYSIZE(unload_patterns); n++) {
					bool is_selected = (current_unload_pattern == unload_patterns[n]);
					if (ImGui::Selectable(unload_patterns[n], is_selected)) {
						current_unload_pattern = unload_patterns[n];
						glb_cfg.ordering = n;
					}
				}
				ImGui::EndCombo();
			}
		}

		{
			const char* size_patterns[] = { "constant", "low-large", "large-low", "random" };
			static const char* current_size_pattern = size_patterns[0];
			if (ImGui::BeginCombo("size", current_size_pattern)) {
				for (int n = 0; n < IM_ARRAYSIZE(size_patterns); n++) {
					bool is_selected = (current_size_pattern == size_patterns[n]);
					if (ImGui::Selectable(size_patterns[n], is_selected)) {
						current_size_pattern = size_patterns[n];
						glb_cfg.sizing = n;
					}
				}
				ImGui::EndCombo();
			}
		}

		{
			const char* loading_patterns[] = { "none-full" };
			static const char* current_loading_pattern = loading_patterns[0];
			if (ImGui::BeginCombo("loading", current_loading_pattern)) {
				for (int n = 0; n < IM_ARRAYSIZE(loading_patterns); n++) {
					bool is_selected = (current_loading_pattern == loading_patterns[n]);
					if (ImGui::Selectable(loading_patterns[n], is_selected)) {
						current_loading_pattern = loading_patterns[n];
						glb_cfg.loading = n;
					}
				}
				ImGui::EndCombo();
			}
		}

		ImGui::InputInt("items", &glb_cfg.max_data_items_count, 1, 100);
		ImGui::InputInt("item size", &glb_cfg.max_data_item_size, 1, 100);
	}
	ImGui::End();
}

void banchmarker::draw() {
	analize(glb_cfg);

	ImGui::BeginGroup();

	if (EditorWindow("Properties")) {
		
		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);

		
		if (ImGui::Button("Run")) {
			glb_cfg.update_counter++;
		}

		if (SubMenuBegin("General", 1)) {
			ImGui::InputInt("Averaging", &glb_cfg.avreging, 1, 100); ToolTip("Number of tests to be averaged");
			ImGui::Checkbox("Collect time per instruction", &glb_cfg.time_per_instruction);
			ImGui::Checkbox("Collect mem per instruction", &glb_cfg.mem_per_instruction);
			SubMenuEnd(1);
		}

		if (SubMenuBegin("Allocators", 1)) {
			ImGui::Checkbox("heap", &glb_cfg.heap);
			ImGui::SameLine();
			ImGui::Checkbox("pool", &glb_cfg.pool);
			ImGui::SameLine();
			ImGui::Checkbox("chunk", &glb_cfg.chunk);

			if (glb_cfg.chunk && SubMenuBegin("Chunk", 2)) {
				ImGui::InputInt("size", &glb_cfg.chunk_bsize, 1, 100); ToolTip("Size of a slot in the chunk buffer");
				ImGui::InputInt("length", &glb_cfg.chunk_blen, 1, 100); ToolTip("Number of slots in the chunk buffer");
				SubMenuEnd(2);
			}
			if (glb_cfg.pool && SubMenuBegin("Pool", 2)) {
				ToolTip("Generalized use of chunk allocator");
				ImGui::InputInt("size", &glb_cfg.pool_bsize, 1, 100); 
				ImGui::InputInt("length", &glb_cfg.pool_blen, 1, 100); 
				SubMenuEnd(2);
			}
			SubMenuEnd(1);
		}
	}
	ImGui::End();

	if (EditorWindow("Overview")) {

		if (ImGui::TreeNode("total time")) {
			for (auto& i : range(0, 3)) {
				if (out[i]) {
					if (out[i]->total_time < 1000) {
						ImGui::Text("%s : %f 10e-9", out[i]->alloc_type, out[i]->total_time);
					}
					else if (out[i]->total_time < 1000000) {
						ImGui::Text("%s : %f 10e-6", out[i]->alloc_type, out[i]->total_time / 1000.f);
					}
					else if (out[i]->total_time < 1000000000) {
						ImGui::Text("%s : %f 10e-3", out[i]->alloc_type, out[i]->total_time / 1000000.f);
					}
					else {
						ImGui::Text("%s : %f ", out[i]->alloc_type, out[i]->total_time / 1000000000.f);
					}
				}
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("failures")) {
			if ((out[0] && out[0]->failed) || (out[1] && out[1]->failed) || (out[2] && out[2]->failed)) {
				for (auto& i : range(0, 3)) {
					if (out[i] && out[i]->failed)
						ImGui::Text("%s failed", out[i]->alloc_type);
				}
			}
			else {
				ImGui::Text("all succeeded");
			}
			ImGui::TreePop();
		}
	}
	ImGui::End();

	if (cfg.time_per_instruction) {
		if (EditorWindow("Graphs")) {
			if (ImPlot::BeginPlot("Time(ns) vs Alloc / Free inst")) {
				if (cfg.heap) ImPlot::PlotLine("halloc", x_axis, out[0]->time.GetBuff(), (int)i_count);
				if (cfg.pool) ImPlot::PlotLine("pool", x_axis, out[1]->time.GetBuff(), (int)i_count);
				if (cfg.chunk) ImPlot::PlotLine("chunk", x_axis, out[2]->time.GetBuff(), (int)i_count);
				ImPlot::EndPlot();
			}
		}
		ImGui::End();
	}

	if (cfg.mem_per_instruction) {
		if (EditorWindow("Graphs")) {
			if (ImPlot::BeginPlot("Memory (bytes) at Instruction state")) {
				if (cfg.heap) ImPlot::PlotLine("halloc", x_axis, out[0]->mem.GetBuff(), (int)i_count);
				if (cfg.pool) ImPlot::PlotLine("pool", x_axis, out[1]->mem.GetBuff(), (int)i_count);
				if (cfg.chunk) ImPlot::PlotLine("chunk", x_axis, out[2]->mem.GetBuff(), (int)i_count);
				ImPlot::EndPlot();
			}
		}
		ImGui::End();
	}

	if (EditorWindow("Pattern")) {
		if (ImPlot::BeginPlot("Alloc/Free instruction info vs instruction idx")) {
			ImPlot::PlotLine("allocated item size", x_axis, pattern_out->alloc_size.GetBuff(), (int)i_count);
			ImPlot::PlotLine("used data item idx", x_axis, pattern_out->data_idx.GetBuff(), (int)i_count);
			ImPlot::PlotLine("total items allocated", x_axis, pattern_out->items_loaded.GetBuff(), (int)i_count);
			ImPlot::EndPlot();
		}
	}
	ImGui::End();

	pattern_generator();

	ImGui::EndGroup();
}

