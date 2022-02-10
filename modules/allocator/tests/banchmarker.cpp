
#include "implot/implot.h"
#include "imgui.h"

#include "banchmarker.h"

config glb_cfg;

banchmarker::banchmarker() {
	out.Reserve(3);
}

test_pattern banchmarker::get_pattern(config& cfg) {
	return test_pattern_basic();
}

void banchmarker::analize(config pcfg) {

	/*
	if (this->cfg == pcfg) {
		return;
	}
	*/

	init_allocators(pcfg);

	if (out[0]) { mfree(&ownheap, out[0]); out[0] = 0; };
	if (out[1]) { mfree(&ownheap, out[1]); out[1] = 0; };
	if (out[2]) { mfree(&ownheap, out[2]); out[2] = 0; };

	test_pattern pattern = get_pattern(pcfg);

	allocator_histogram* heap_hist = new(&ownheap) allocator_histogram(&pattern, "heap");
	allocator_histogram* chunk_hist = new(&ownheap) allocator_histogram(&pattern, "chunk");
	allocator_histogram* pool_hist = new(&ownheap) allocator_histogram(&pattern, "pool");

	for (alni iter = 0; iter < pcfg.avreging; iter++) {
		collect(&pattern, halloc, heap_hist);
		collect(&pattern, palloc, pool_hist);
		collect(&pattern, calloc, chunk_hist);
	}

	out[0] = heap_hist;
	out[1] = pool_hist;
	out[2] = chunk_hist;

	dest_allocators();
	this->cfg = pcfg;
}

void banchmarker::draw() {
	analize(glb_cfg);

	ImGui::Begin("cfg");
	ImGui::SliderInt("Collector Iterations", &glb_cfg.avreging, 1, 100);
	ImGui::End();
}

void banchmarker::init_allocators(config& pcfg) {
	halloc = new(&ownheap) heapalloc();
	palloc = new(&ownheap) poolalloc(pcfg.pool_bsize, pcfg.pool_blen);
	chunk_heap = new(&ownheap) heapalloc();
	calloc = new(chunk_heap) chunkalloc(chunk_heap, pcfg.pool_bsize, pcfg.pool_blen);
}

void banchmarker::dest_allocators() {
	mfree(&ownheap, halloc);
	mfree(&ownheap, palloc);

	calloc->finalize(chunk_heap);
	mfree(chunk_heap, calloc);

	mfree(&ownheap, chunk_heap);
}