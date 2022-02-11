
#include "implot/implot.h"
#include "imgui.h"

#include "banchmarker.h"

config glb_cfg = {
	.update_counter = 0,

	.time_per_instruction = true,
	.mem_per_instruction = true,
	.avreging = 1,

	.loading = load_type::LINEAR,
	.ordering = order_type::LINEAR,
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
	reserve_out(pattern);

	for (alni iter = 0; iter < pcfg.avreging; iter++) {
		init_allocators(pcfg);
		collect(pattern, halloc, out[0]);
		collect(pattern, palloc, out[1]);
		collect(pattern, calloc, out[2]);
		dest_allocators();
	}

	out[0]->scale_all(1.f / pcfg.avreging);
	out[1]->scale_all(1.f / pcfg.avreging);
	out[2]->scale_all(1.f / pcfg.avreging);

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
	halloc = new(&ownheap) heapalloc();
	palloc = new(&ownheap) poolalloc(pcfg.pool_bsize, pcfg.pool_blen);
	chunk_heap = new(&ownheap) heapalloc();
	calloc = new(chunk_heap) chunkalloc(chunk_heap, pcfg.chunk_bsize, pcfg.chunk_blen);
}

void banchmarker::dest_allocators() {
	mfree(&ownheap, halloc);
	mfree(&ownheap, palloc);

	calloc->finalize(chunk_heap);
	mfree(chunk_heap, calloc);

	mfree(&ownheap, chunk_heap);
}

void banchmarker::clear_out() {
	if (out[0]) { mfree(&ownheap, out[0]); out[0] = 0; };
	if (out[1]) { mfree(&ownheap, out[1]); out[1] = 0; };
	if (out[2]) { mfree(&ownheap, out[2]); out[2] = 0; };
	if (pattern_out) { mfree(&ownheap, pattern_out); pattern_out = 0; };
}

void banchmarker::reserve_out(test_pattern* pattern) {
	pattern_out = new(&ownheap) pattern_histogram(pattern);
	out[0] = new(&ownheap) allocator_histogram(pattern, "heap", cfg.time_per_instruction, cfg.mem_per_instruction);
	out[1] = new(&ownheap) allocator_histogram(pattern, "chunk", cfg.time_per_instruction, cfg.mem_per_instruction);
	out[2] = new(&ownheap) allocator_histogram(pattern, "pool", cfg.time_per_instruction, cfg.mem_per_instruction);
}


void banchmarker::draw() {
	analize(glb_cfg);

	if (ImGui::Begin("cfg")) {
		if (ImGui::Button("run")) {
			glb_cfg.update_counter++;
		}

		if (ImGui::TreeNode("General")) {
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.3f);
			ImGui::InputInt("Number of tests to be avraged", &glb_cfg.avreging, 1, 100);
			ImGui::Checkbox("Collect time per instruction", &glb_cfg.time_per_instruction);
			ImGui::Checkbox("Collect mem per instruction", &glb_cfg.mem_per_instruction);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Test Pattern")) {
			//.loading = load_type::LINEAR,
			//.ordering = order_type::LINEAR,
			ImGui::InputInt("max items", &glb_cfg.max_data_items_count, 1, 100);
			ImGui::InputInt("max item size", &glb_cfg.max_data_item_size, 1, 100);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Allocators")) {
			if (ImGui::TreeNode("Chunk")) {
				ImGui::InputInt("block size", &glb_cfg.chunk_bsize, 1, 100);
				ImGui::InputInt("num of block", &glb_cfg.chunk_blen, 1, 100);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Pool")) {
				ImGui::InputInt("block size", &glb_cfg.pool_bsize, 1, 100);
				ImGui::InputInt("num of block", &glb_cfg.pool_blen, 1, 100);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		ImGui::End();
	}

	if (ImGui::Begin("overview")) {

		if (ImGui::TreeNode("total time")) {
			for (auto& i : range(0, 3)) {
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
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("failures")) {
			if (out[0]->failed || out[1]->failed || out[2]->failed) {
				for (auto& i : range(0, 3)) {
					if (out[i]->failed)
						ImGui::Text("%s failed", out[i]->alloc_type);
				}
			}
			else {
				ImGui::Text("all succeeded");
			}
			ImGui::TreePop();
		}

		ImGui::End();
	}

	{
		ImGui::Begin("graphs");
		if (cfg.time_per_instruction && ImPlot::BeginPlot("Time (ns) vs Alloc/Free inst")) {
			ImPlot::PlotLine("halloc", x_axis, out[0]->time.GetBuff(), (int)i_count);
			ImPlot::PlotLine("pool", x_axis, out[1]->time.GetBuff(), (int)i_count);
			ImPlot::PlotLine("chunk", x_axis, out[2]->time.GetBuff(), (int)i_count);
			ImPlot::EndPlot();
		}
		ImGui::End();

		ImGui::Begin("graphs");
		if (cfg.mem_per_instruction && ImPlot::BeginPlot("Memory (bytes) at Instruction state")) {
			ImPlot::PlotLine("halloc", x_axis, out[0]->mem.GetBuff(), (int)i_count);
			ImPlot::PlotLine("pool", x_axis, out[1]->mem.GetBuff(), (int)i_count);
			ImPlot::PlotLine("chunk", x_axis, out[2]->mem.GetBuff(), (int)i_count);
			ImPlot::EndPlot();
		}
		ImGui::End();


		ImGui::Begin("graphs");
		if (ImPlot::BeginPlot("Alloc/Free instruction info vs instruction idx")) {
			ImPlot::PlotLine("allocated item size", x_axis, pattern_out->alloc_size.GetBuff(), (int)i_count);
			ImPlot::PlotLine("used data item idx", x_axis, pattern_out->data_idx.GetBuff(), (int)i_count);
			ImPlot::PlotLine("total items allocated", x_axis, pattern_out->items_loaded.GetBuff(), (int)i_count);
			ImPlot::EndPlot();
		}
		ImGui::End();
	}
}