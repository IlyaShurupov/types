
#include "poolalloc.h"

#include "heapalloc.h"
#include "chunkalloc.h"

#include "new.h"

heapalloc pool_halloc;

struct chunk_node : public chunkalloc {
	chunk_node(allocator* alloc, alni bsize, alni nblocks) : chunkalloc(alloc, bsize, nblocks) {}
	chunk_node* next = NULL;
	chunk_node* prev = NULL;
};


chunk_node* chunk_list::addchunk(heapalloc* halloc, alni bsize, alni nblocks) {
	chunk_node* node = new(halloc) chunk_node(halloc, bsize, nblocks);
	
	node->next = node->prev = NULL;

	if (first) {
		first->next = node;
		node->prev = first;
	}

	first = node;

	return node;
}

void chunk_list::delchunk(chunk_node* node, heapalloc* halloc) {

	if (node->next) {
		node->next->prev = node->prev;
	}
	if (node->prev) {
		node->prev->next = node->next;
	}

	if (node == first) {
		first = first->next;
	}

	node->finalize(halloc);
	mfree(halloc, node);
}


void chunk_list::initialize(heapalloc* halloc) {
	if (first) {
		finalize(halloc);
	}
	first = NULL;
}

void chunk_list::finalize(heapalloc* halloc) {
	for (chunk_node* iter = first; iter; iter = iter->next) {
		iter->finalize(halloc);
		mfree(halloc, iter);
	}
}



poolalloc::poolalloc(alni pbsize, alni pnblocks) {
	chunks.initialize(&pool_halloc);
	last_used = NULL;
	bsize = pbsize;
	nblocks = pnblocks;
}

poolalloc::~poolalloc() {
	chunks.finalize(&pool_halloc);
}


bool poolalloc::avaliable() {
	return true;
}

alni poolalloc::inuse_size() {
	alni size = 0;
	for (chunk_node* iter = chunks.first; iter; iter = iter->next) {
		size += iter->inuse_size();
	}
	return size;
}

alni poolalloc::reserved_size() {
	alni size = 0;
	for (chunk_node* iter = chunks.first; iter; iter = iter->next) {
		size += iter->reserved_size();
	}
	return size;
}

void* poolalloc::alloc(alni size, const char* file, int line) {
	return alloc(size);
}

void* poolalloc::alloc(alni size) {

	chunk_node* avalchunk = NULL;

	// first allocation ever
	if (!last_used) {
		chunk_node* node = chunks.addchunk(&pool_halloc, bsize, nblocks);
		avalchunk = node;

	}
	else if (last_used->avaliable()) {
		avalchunk = last_used;
	}
	else {
		for (chunk_node* iter = chunks.first; iter; iter = iter->next) {
			if (iter->avaliable()) {
				avalchunk = iter;
			}
		}
	}

	last_used = avalchunk;

	return avalchunk->alloc(size);
}

void poolalloc::free(void* p) {
	chunkalloc* chunk_p = (((used_slot_head*)p) - 1)->chunk_p;
	chunk_p->free(p);

	alni chunks_len = 0;
	for (chunk_node* iter = chunks.first; iter; iter = iter->next) {
		chunks_len++;
	}

	if (!chunk_p->inuse_size() && chunks_len > 2) {
		chunks.delchunk((chunk_node*)chunk_p, &pool_halloc);
	}
}
