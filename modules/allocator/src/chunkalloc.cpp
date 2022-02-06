
#include "chunkalloc.h"

alni calc_bsize(alni bsize) {
	if (bsize < sizeof(alni)) {
		bsize = sizeof(alni);
	}
	if (bsize % sizeof(alni) != 0) {
		bsize += sizeof(alni);
	}

	bsize += sizeof(used_slot_head);
	return bsize;
}

chunkalloc::chunkalloc(allocator* alloc, alni bsize, alni nblocks) {
	if (nblocks < 32) {
		nblocks = 32;
	}
	bsize = calc_bsize(bsize);

	buff = (alni*)alloc->alloc(bsize * nblocks);
	bnext = NULL;
	bfreec = binitc = 0;
}


void chunkalloc::finalize(allocator* alloc) {
	alloc->free(buff);
}

bool chunkalloc::avaliable() { return bfreec; }
alni chunkalloc::inuse_size() { return (nblocks - bfreec) * (bsize - sizeof(used_slot_head)); }
alni chunkalloc::reserved_size() { return  bsize * nblocks; }

inline void* chunkalloc::get_addr(alni idx) const { return &buff[idx * bsize]; }
inline alni chunkalloc::get_idx(const void* address) const { return (((alni)((alni*)address - buff)) / bsize); }

alni chunkalloc::get_bsize() {
	return bsize;
}

#ifdef MEM_TRACE
void* alloc(alni size, const char* file, int line) {
	alloc(alni size);
}
#else
void* chunkalloc::alloc(alni size) {

	if (size > alni(bsize - sizeof(used_slot_head))) {
		return NULL;
	}

	if (binitc < nblocks) {
		binitc++;
		((unused_slot_head*)get_addr(binitc))->bnext = (alni*)get_addr(binitc);
	}

	alni* out = NULL;
	if (bfreec) {

		out = bnext;
		--bfreec;

		if (bfreec != 0) {
			bnext = ((unused_slot_head*)bnext)->bnext;
		}
		else {
			bnext = NULL;
		}

		((used_slot_head*)out)->chunk_p = this;
		out = (alni*)(((used_slot_head*)out) + 1);
	}

	return out;
}
#endif 

void chunkalloc::free(void* p) {
	alni* bdel = (alni*)((used_slot_head*)p + 1);
	if (bnext != NULL) {
		((unused_slot_head*)bdel)->bnext = bnext;
		bnext = bdel;
	}
	else {
		((unused_slot_head*)bdel)->bnext = (alni*)get_addr(nblocks);
		bnext = bdel;
	}
	++bfreec;
}
