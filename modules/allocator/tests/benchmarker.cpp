
#include "benchmarker.h"

#include "implot.h"

tp::alni hash(const tp::string& val) {
	return tp::hash(val.cstr());
}

config glb_cfg = {

	.live_update = false,

	.heap = true,
	.pool = true,
	.chunk = true,

	.current_sizing_pattern = 0,
	.current_loading_pattern = 0,
	.current_ordering_pattern = 0,

	.update = 0,

	.time_per_instruction = true,
	.mem_per_instruction = true,
	.avreging = 1,

	.chunk_bsize = 100,
	.chunk_blen = 100,

	.pool_bsize = 100,
	.pool_blen = 100,
};

const_pattern bipattern_const;
linear_pattern bipattern_line;
random_pattern bipattern_rand;

benchmarker::benchmarker() /*: ImGui::CompleteApp()*/ {
	i_count = 0;
	pattern_out = NULL;
	out.reserve(3);

	patterns.put("constant", &bipattern_const);
	patterns.put("linear", &bipattern_line);
	patterns.put("random", &bipattern_rand);
}

void benchmarker::output_draw() {
	if (ImGui::WindowEditor("Overview")) {

		if (is_output) {
			if (ImGui::TreeNode("total time")) {
				for (auto& i : tp::Range(0, 3)) {
					if (out[i]) {
						if (out[i]->total_time < 1000) {
							ImGui::Text("%s : %f 10e-9", out[i]->alloc_type, out[i]->total_time);
						} else if (out[i]->total_time < 1000000) {
							ImGui::Text("%s : %f 10e-6", out[i]->alloc_type, out[i]->total_time / 1000.f);
						} else if (out[i]->total_time < 1000000000) {
							ImGui::Text("%s : %f 10e-3", out[i]->alloc_type, out[i]->total_time / 1000000.f);
						} else {
							ImGui::Text("%s : %f ", out[i]->alloc_type, out[i]->total_time / 1000000000.f);
						}
					}
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("failures")) {
				if ((out[0] && out[0]->failed) || (out[1] && out[1]->failed) || (out[2] && out[2]->failed)) {
					for (auto& i : tp::Range(0, 3)) {
						if (out[i] && out[i]->failed)
							ImGui::Text("%s failed", out[i]->alloc_type);
					}
				} else {
					ImGui::Text("all succeeded");
				}
				ImGui::TreePop();
			}
		}
	}
	ImGui::End();


	if (cfg.time_per_instruction) {
		if (ImGui::WindowEditor("Graphs")) {
			if (is_output) {
				if (ImPlot::BeginPlot("Time(ns) vs Alloc / Free inst")) {
					if (cfg.heap) ImPlot::PlotLine("halloc", x_axis, out[0]->time.buff(), (int) i_count);
					if (cfg.pool) ImPlot::PlotLine("pool", x_axis, out[1]->time.buff(), (int) i_count);
					if (cfg.chunk) ImPlot::PlotLine("chunk", x_axis, out[2]->time.buff(), (int) i_count);
					ImPlot::EndPlot();
				}
			}
		}
		ImGui::End();
	}

	if (cfg.mem_per_instruction) {
		if (ImGui::WindowEditor("Graphs")) {
			if (is_output) {
				if (ImPlot::BeginPlot("Memory (bytes) at Instruction state")) {
					if (cfg.heap) ImPlot::PlotLine("halloc", x_axis, out[0]->mem.buff(), (int) i_count);
					if (cfg.pool) ImPlot::PlotLine("pool", x_axis, out[1]->mem.buff(), (int) i_count);
					if (cfg.chunk) ImPlot::PlotLine("chunk", x_axis, out[2]->mem.buff(), (int) i_count);
					ImPlot::EndPlot();
				}
			}
		}
		ImGui::End();
	}

	if (ImGui::WindowEditor("Pattern")) {
		if (is_output) {
			if (ImPlot::BeginPlot("Alloc/Free instruction info vs instruction idx")) {
				if (cfg.current_sizing_pattern) ImPlot::PlotLine("allocated item size", x_axis, pattern_out->alloc_size.buff(), (int) i_count);
				if (cfg.current_ordering_pattern) ImPlot::PlotLine("used data item idx", x_axis, pattern_out->data_idx.buff(), (int) i_count);
				if (cfg.current_loading_pattern) ImPlot::PlotLine("total items allocated", x_axis, pattern_out->items_loaded.buff(), (int) i_count);
				ImPlot::EndPlot();
			}
		}
	}
	ImGui::End();
}

benchmarker::~benchmarker() {
	for (auto iter : patterns) {
		if (!iter->val->build_in) {
			delete iter->val;
		}
	}
	clear_out();
}

void benchmarker::select_pattern() {
	if (patterns.size()) {
		const char** pattern_names = new const char* [patterns.size()];

		const char* current_opattern_name = NULL;
		const char* current_lpattern_name = NULL;
		const char* current_spattern_name = NULL;

		for (auto pattern_name : patterns) {
			pattern_names[pattern_name.entry_idx] = pattern_name->key.cstr();
			if (pattern_name->val == glb_cfg.current_ordering_pattern)
				current_opattern_name = pattern_name->key.cstr();
			if (pattern_name->val == glb_cfg.current_sizing_pattern)
				current_spattern_name = pattern_name->key.cstr();
			if (pattern_name->val == glb_cfg.current_loading_pattern)
				current_lpattern_name = pattern_name->key.cstr();
		}

		if (ImGui::BeginCombo("Ordering", current_opattern_name)) {
			for (int n = 0; n < patterns.size(); n++) {
				bool is_selected = (current_opattern_name == pattern_names[n]);
				if (ImGui::Selectable(pattern_names[n], is_selected)) {
					current_opattern_name = pattern_names[n];
					glb_cfg.current_ordering_pattern = patterns.get(pattern_names[n]);
				}
			}
			ImGui::EndCombo();
		}

		if (ImGui::BeginCombo("Loading", current_lpattern_name)) {
			for (int n = 0; n < patterns.size(); n++) {
				bool is_selected = (current_lpattern_name == pattern_names[n]);
				if (ImGui::Selectable(pattern_names[n], is_selected)) {
					current_lpattern_name = pattern_names[n];
					glb_cfg.current_loading_pattern = patterns.get(pattern_names[n]);
				}
			}
			ImGui::EndCombo();
		}

		if (ImGui::BeginCombo("Sizing", current_spattern_name)) {
			for (int n = 0; n < patterns.size(); n++) {
				bool is_selected = (current_spattern_name == pattern_names[n]);
				if (ImGui::Selectable(pattern_names[n], is_selected)) {
					current_spattern_name = pattern_names[n];
					glb_cfg.current_sizing_pattern = patterns.get(pattern_names[n]);
				}
			}
			ImGui::EndCombo();
		}
		delete pattern_names;
	}
}

void benchmarker::pattern_combo(const char*& current) {
	if (patterns.size()) {
		const char** pattern_names = new const char* [patterns.size()];

		for (auto pattern_name : patterns) {
			pattern_names[pattern_name.entry_idx] = pattern_name->key.cstr();
		}

		if (ImGui::BeginCombo("Patterns", current)) {
			for (int n = 0; n < patterns.size(); n++) {
				bool is_selected = (current == pattern_names[n]);
				if (ImGui::Selectable(pattern_names[n], is_selected)) {
					current = pattern_names[n];
				}
			}
			ImGui::EndCombo();
		}
		delete pattern_names;
	}
}

void benchmarker::pattern_generator() {

	ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.1f);

	pattern* pattern_edit = NULL;
	if (ImGui::WindowEditor("Pattern Generator")) {

		if (ImGui::SubMenuBegin("Selector", 1)) {

			pattern_combo(pattern_generator_active);

			if (pattern_generator_active) {
				tp::alni idx = patterns.presents(pattern_generator_active);
				if (!MAP_VALID_IDX(idx)) {
					pattern_edit = 0;
					pattern_generator_active = 0;
				} else {
					pattern_edit = patterns[idx];
				}
			}

			//ImGui::Separator();

			static char create_name[100] = {"new pattern name"};
			ImGui::InputText("pattern name", create_name, 100);

			if (ImGui::Button("Create")) {
				if (patterns.presents(create_name)) {
					pattern* new_pt = new pattern();
					new_pt->build_in = false;
					new_pt->pattern_name = create_name;
					patterns.put(create_name, new_pt);
				} else {
					ImGui::Notify("Such Pattern Already Exists", 3);
				}
			}

			if (pattern_edit) {
				ImGui::SameLine();
				if (ImGui::Button("Delete")) {
					if (pattern_edit->build_in) {
						ImGui::Notify("Cant Remove Built-in Patterns", 3);
					} else {
						patterns.remove(pattern_generator_active);
						delete pattern_edit;
						pattern_edit = NULL;
						pattern_generator_active = NULL;
					}
				}

				ImGui::SameLine();
				if (ImGui::Button("Rename")) {
					if (pattern_edit->build_in) {
						ImGui::Notify("Cant Rename Built-in Patterns", 3);
					} else {
						patterns.remove(pattern_generator_active);
						patterns.put(create_name, pattern_edit);
					}
				}
			}
			ImGui::SubMenuEnd(1);
		}

		if (pattern_generator_active) {
			tp::alni idx = patterns.presents(pattern_generator_active);
			if (!MAP_VALID_IDX(idx)) {
				pattern_edit = 0;
				pattern_generator_active = 0;
			} else {
				pattern_edit = patterns[idx];
			}
		}

		if (!pattern_edit) {
			ImGui::Text("Select pattern to edit or create one");
			ImGui::End();
			return;
		}

		if (ImGui::SubMenuBegin("Preview", 1)) {
			static float preview_res_scale = 0.05f;
			int resolution = (int) (1000 * preview_res_scale);
			CLAMP(resolution, 3, 10000);
			float* x_axis = new float[resolution];
			float* y_axis = new float[resolution];

			float x = 0;
			float step = 1.f / (resolution - 1);
			for (tp::alni idx = 0; idx < resolution; idx++) {
				x_axis[idx] = x;
				y_axis[idx] = (tp::flt4) pattern_edit->get_y(&patterns, x);
				x += step;
			}


			if (ImPlot::BeginPlot("Pattern")) {
				ImPlot::PlotLine("toggle", x_axis, y_axis, resolution);
				ImPlot::EndPlot();
			}

			delete x_axis;
			delete y_axis;
			ImGui::SliderFloat("graph resolution", &preview_res_scale, 0.f, 1.f);
			ImGui::SubMenuEnd(1);
		}

		if (ImGui::SubMenuBegin("Compositor", 1)) {

			if (!pattern_edit->build_in) {

				static tp::alni selected_idx = -1;
				static pattern* child_pattern_active = NULL;

				if (ImGui::SubMenuBegin("Child Patterns", 2)) {
					ImGui::BeginListBox("");
					for (tp::alni idx = 0; idx < pattern_edit->regions.length(); idx++) {
						ImGui::PushID((int) idx);
						if (ImGui::Button(pattern_edit->regions[idx].name.cstr())) {
							child_pattern_active = patterns.get(pattern_edit->regions[idx].name);
							selected_idx = idx;
						}
						if (selected_idx == idx) {
							ImGui::SameLine(); ImGui::Text(" - Active");
						}
						ImGui::PopID();
					}
					ImGui::EndListBox();

					if (selected_idx >= pattern_edit->regions.length()) {
						selected_idx = -1;
					}

					static const char* append_pattern = NULL;
					bool add = ImGui::Button(" + ");
					ImGui::SameLine();
					pattern_combo(append_pattern);
					if (add) {
						if (append_pattern) {
							pattern_edit->regions.pushBack(child_pattern(append_pattern));
						} else {
							ImGui::Notify("Select a Pattern to Append");
						}
					}

					if (child_pattern_active && selected_idx != -1) {

						if (ImGui::Button(" Up ")) {
							if (selected_idx > 0) {
								tp::string tmp = pattern_edit->regions[selected_idx].name;
								pattern_edit->regions[selected_idx] = pattern_edit->regions[selected_idx - 1];
								pattern_edit->regions[selected_idx - 1] = tmp;
								selected_idx--;
							}
						}
						ImGui::SameLine();
						if (ImGui::Button("Down")) {
							if (selected_idx < pattern_edit->regions.length() - 1) {
								tp::string tmp = pattern_edit->regions[selected_idx].name;
								pattern_edit->regions[selected_idx] = pattern_edit->regions[selected_idx + 1];
								pattern_edit->regions[selected_idx + 1] = tmp;
								selected_idx++;
							}
						}
						ImGui::SameLine();
						if (ImGui::Button("Remove")) {
							pattern_edit->regions.remove(selected_idx);
							selected_idx = pattern_edit->regions.length() - 1;
						}
					} else {
						ImGui::Text("Select Child Pattern");
					}
					ImGui::SubMenuEnd(2);
				}

				if (child_pattern_active && selected_idx != -1) {
					if (ImGui::SubMenuBegin("child Pattern Properties", 2)) {
						ImGui::SliderFloat("Point", &pattern_edit->regions[selected_idx].point, 0.f, 1.f);
						ImGui::SliderFloat("Lower lim", &pattern_edit->regions[selected_idx].lowerlim, 0.f, 1.f);
						ImGui::SliderFloat("Upper lim", &pattern_edit->regions[selected_idx].uppernlim, 0.f, 1.f);
						if (child_pattern_active->build_in) {
						}
						ImGui::SubMenuEnd(2);
					}
				}
			} else {
				ImGui::Text("Can't Edit Built-In Patterns");
			}
			ImGui::SubMenuEnd(1);
		}

		ImGui::Separator();

	}
	ImGui::End();
}

void benchmarker::MainDrawTick() {

	analize(glb_cfg);


	ImGui::BeginGroup();

	if (ImGui::WindowEditor("Properties")) {

		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);

		{
			if (ImGui::Button("Run")) {
				glb_cfg.update = true;
			}

			bool prev_val = glb_cfg.live_update;
			ImGui::SameLine(); ImGui::Checkbox("Live Update", &glb_cfg.live_update);
			if (prev_val != glb_cfg.live_update) {
				glb_cfg.update = true;
			}
		}

		if (ImGui::SubMenuBegin("General", 1)) {
			ImGui::InputInt("Averaging", &glb_cfg.avreging, 1, 100); ImGui::ToolTip("Number of tests to be averaged");
			ImGui::Checkbox("Collect time per instruction", &glb_cfg.time_per_instruction);
			ImGui::Checkbox("Collect mem per instruction", &glb_cfg.mem_per_instruction);

			ImGui::SubMenuEnd(1);
		}

		if (ImGui::SubMenuBegin("Testing Pattern", 1)) {

			select_pattern();

			ImGui::InputInt("size", &glb_cfg.pt_scale.size, 1, 100); ImGui::ToolTip("Y Scale of sizing pattern");
			ImGui::InputInt("items", &glb_cfg.pt_scale.items, 1, 100); ImGui::ToolTip("Y Scale of ordering and loading patterns");
			ImGui::InputInt("iterations", &glb_cfg.pt_scale.iterations, 1, 100); ImGui::ToolTip("X Scale of all patterns");

			ImGui::SubMenuEnd(1);
		}

		if (ImGui::SubMenuBegin("Allocators", 1)) {
			ImGui::Checkbox("heap", &glb_cfg.heap); ImGui::SameLine(); ImGui::Checkbox("pool", &glb_cfg.pool); ImGui::SameLine(); ImGui::Checkbox("chunk", &glb_cfg.chunk);

			if (glb_cfg.chunk && ImGui::SubMenuBegin("Chunk", 2)) {
				ImGui::InputInt("size", &glb_cfg.chunk_bsize, 1, 100); ImGui::ToolTip("Size of a slot in the chunk buffer");
				ImGui::InputInt("length", &glb_cfg.chunk_blen, 1, 100); ImGui::ToolTip("Number of slots in the chunk buffer");
				ImGui::SubMenuEnd(2);
			}
			if (glb_cfg.pool && ImGui::SubMenuBegin("Pool", 2)) {
				ImGui::ToolTip("Generalized use of chunk allocator");
				ImGui::InputInt("size", &glb_cfg.pool_bsize, 1, 100);
				ImGui::InputInt("length", &glb_cfg.pool_blen, 1, 100);
				ImGui::SubMenuEnd(2);
			}
			ImGui::SubMenuEnd(1);
		}

		//ImGui::PopItemWidth();
	}
	ImGui::End();

	output_draw();

	pattern_generator();

	ImGui::EndGroup();
}


void benchmarker::analize(config pcfg) {

	if (!((pcfg.update) || (!(this->cfg == pcfg) && cfg.live_update))) {
		return;
	}

	this->cfg = pcfg;

	clear_out();

	if (!pattern_analizer.init(&patterns, cfg.current_loading_pattern, cfg.current_ordering_pattern, cfg.current_sizing_pattern, &cfg.pt_scale)) {
		if (pcfg.update) ImGui::Notify("invalid pattern configuration", 3);
		glb_cfg.update = false;
		cfg.update = false;
		is_output = false;
		return;
	}

	reserve_out(&pattern_analizer);

	for (tp::alni iter = 0; iter < pcfg.avreging; iter++) {
		init_allocators(pcfg);
		if (cfg.heap) collect(&pattern_analizer, halloc, out[0]);
		if (cfg.pool) collect(&pattern_analizer, palloc, out[1]);
		if (cfg.chunk) collect(&pattern_analizer, calloc, out[2]);
		dest_allocators();
	}

	for (auto& i : tp::Range(0, 3)) {
		if (out[i]) out[i]->scale_all(1.f / pcfg.avreging);
	}

	i_count = pattern_analizer.max_iterations();
	if (x_axis) {
		delete x_axis;
		x_axis = NULL;
	}
	x_axis = new tp::alnf[i_count];
	for (tp::alni iter = 0; iter < i_count; iter++) {
		x_axis[iter] = (tp::alnf) iter;
	}

	glb_cfg.update = false;
	cfg.update = false;
	is_output = true;
}

void benchmarker::init_allocators(config& pcfg) {
	if (cfg.heap) halloc = new tp::HeapAlloc();
	if (cfg.pool) palloc = new tp::PoolAlloc(pcfg.pool_bsize, pcfg.pool_blen);
	if (cfg.chunk) calloc = new tp::ChunkAlloc(pcfg.chunk_bsize, pcfg.chunk_blen);
}

void benchmarker::dest_allocators() {
	if (cfg.heap) delete halloc;
	if (cfg.pool) delete palloc;
	if (cfg.chunk) delete calloc;

	halloc = NULL;
	palloc = NULL;
	calloc = NULL;
}

void benchmarker::clear_out() {
	for (auto i : tp::Range(0, 3)) {
		if (out[i]) {
			delete out[i];
			out[i] = NULL;
		}
	}

	if (pattern_out) delete pattern_out;
	pattern_out = NULL;
}

void benchmarker::reserve_out(test_pattern* pattern) {
	pattern_out = new pattern_histogram(pattern);
	if (cfg.heap) out[0] = new allocator_histogram(pattern, "heap", cfg.time_per_instruction, cfg.mem_per_instruction);
	if (cfg.pool) out[1] = new allocator_histogram(pattern, "pool", cfg.time_per_instruction, cfg.mem_per_instruction);
	if (cfg.chunk) out[2] = new allocator_histogram(pattern, "chunk", cfg.time_per_instruction, cfg.mem_per_instruction);
}