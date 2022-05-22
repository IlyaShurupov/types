
#include "poolalloc.h"

#include "chunkalloc.h"
#include "heapalloc.h"
#include "new.h"

namespace tp {

	PoolAlloc::chunk_list::chunk_node* PoolAlloc::chunk_list::addchunk(PoolAlloc* self, alni bsize, alni nblocks) {
		chunk_node* node = new chunk_node(self, bsize, nblocks);

		node->next = node->prev = NULL;

		if (last) {
			last->next = node;
			node->prev = last;
		}

		last = node;

		return node;
	}

	void PoolAlloc::chunk_list::delchunk(chunk_node* node) {
		if (node->next) {
			node->next->prev = node->prev;
		}
		if (node->prev) {
			node->prev->next = node->next;
		}

		if (node == last) {
			last = last->prev;
		}

		delete node;
	}

	void PoolAlloc::chunk_list::initialize() {
		if (last) {
			finalize();
		}
		last = NULL;
	}

	void PoolAlloc::chunk_list::finalize() {
		chunk_node* iter = last;
		while (iter) {
			chunk_node* del = iter;
			iter = iter->prev;
			delchunk(del);
		}
	}

	PoolAlloc::PoolAlloc(alni pbsize, alni pnblocks) {
		chunks.initialize();
		last_used = NULL;
		bsize = pbsize;
		nblocks = pnblocks;
	}

	PoolAlloc::~PoolAlloc() { chunks.finalize(); }

	bool PoolAlloc::isAvaliable() { return true; }

	alni PoolAlloc::sizeInuse() {
		alni size = 0;
		for (chunk_list::chunk_node* iter = chunks.last; iter; iter = iter->prev) {
			size += iter->sizeInuse();
		}
		return size;
	}

	alni PoolAlloc::sizeReserved() {
		alni size = 0;
		for (chunk_list::chunk_node* iter = chunks.last; iter; iter = iter->prev) {
			size += iter->sizeReserved();
		}
		return size;
	}

	bool PoolAlloc::isEmpty() { return !sizeReserved(); }

	void* PoolAlloc::Alloc(alni size) {
		chunk_list::chunk_node* avalchunk = NULL;

		if (last_used && last_used->isAvaliable()) {
			avalchunk = last_used;
		} else {
			for (chunk_list::chunk_node* iter = chunks.last; iter; iter = iter->prev) {
				if (iter->isAvaliable()) {
					avalchunk = iter;
				}
			}
		}

		if (!avalchunk) {
			chunk_list::chunk_node* node = chunks.addchunk(this, bsize, nblocks);
			avalchunk = node;
		}

		last_used = avalchunk;

		return avalchunk->Alloc(size);
	}

	void PoolAlloc::Free(void* p) {
		#ifdef MEM_WRAP
		ChunkAlloc* chunk_p = (((ChunkAlloc::used_slot_head*) ((int1*) p - CHUNK_WRAP_LEN)) - 1)->chunk_p;
		#else
		chunkalloc* chunk_p = (((used_slot_head*) p) - 1)->chunk_p;
		#endif  // MEM_WRAP

		chunk_p->ChunkAlloc::Free(p);

		if (!chunk_p->sizeInuse()) {
			chunks.delchunk((chunk_list::chunk_node*) chunk_p);
			if (last_used == chunk_p) {
				last_used = NULL;
			}
		}
	}

	bool PoolAlloc::isWrapCorrupted() {
		for (chunk_list::chunk_node* iter = chunks.last; iter; iter = iter->prev) {
			if (iter->isWrapCorrupted()) {
				return true;
			}
		}
		return false;
	}
};