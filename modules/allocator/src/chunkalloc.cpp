
#include "chunkalloc.h"

namespace tp {
	
	#ifdef MEM_WRAP
	alni align(alni out) {
		if (out < sizeof(alni)) {
			out = sizeof(alni);
		}
		if (out % sizeof(alni) != 0) {
			out += sizeof(alni);
		}
		return out;
	}
	#endif

	alni ChunkAlloc::calc_bsize(alni bsize) {

		#ifdef MEM_WRAP
		bsize += CHUNK_WRAP_LEN * 2;
		#endif // MEM_WRAP

		if (bsize < sizeof(alni)) {
			bsize = sizeof(alni);
		}
		if (bsize % sizeof(alni) != 0) {
			bsize += sizeof(alni) - (bsize % sizeof(alni));
		}

		bsize += sizeof(used_slot_head);
		return bsize;
	}

	ChunkAlloc::ChunkAlloc(alni pbsize, alni pnblocks) {
		nblocks = pnblocks;

		if (nblocks < 8) {
			nblocks = 8;
		}
		bsize = calc_bsize(pbsize);

		buff = new alni[alni(bsize / (alnf) sizeof(alni)) * nblocks];

		bnext = buff;
		bfreec = nblocks;
		binitc = 0;
	}

	bool ChunkAlloc::isAvaliable() { return bfreec; }

	alni ChunkAlloc::sizeInuse() {
		#ifdef MEM_WRAP
		return (nblocks - bfreec) * (bsize - sizeof(used_slot_head) - CHUNK_WRAP_LEN * 2);
		#else
		return (nblocks - bfreec) * (bsize - sizeof(used_slot_head));
		#endif
	}

	alni ChunkAlloc::sizeReserved() { return  bsize * nblocks; }

	bool ChunkAlloc::isEmpty() {
		return !sizeReserved();
	}

	inline void* ChunkAlloc::get_addr(alni idx) const {
		return ((uint1*) buff) + (idx * bsize);
	}
	inline alni ChunkAlloc::get_idx(const void* address) const {
		return (((alni) ((alni*) address - buff)) / bsize);
	}

	alni ChunkAlloc::blockSize() {
		return bsize;
	}

	void* ChunkAlloc::Alloc(alni size) {

		if (size > alni(bsize - sizeof(used_slot_head) - CHUNK_WRAP_LEN * 2)) {
			throw Exeption("faled allocate on chunk - size exeedes block size", false);
		}

		if (binitc < nblocks) {
			((unused_slot_head*) get_addr(binitc))->bnext = (alni*) get_addr(binitc + 1);
			binitc++;
		}

		alni* out = NULL;
		if (bfreec) {

			out = bnext;
			--bfreec;

			if (bfreec != 0) {
				bnext = ((unused_slot_head*) bnext)->bnext;
			} else {
				bnext = NULL;
			}

			((used_slot_head*) out)->chunk_p = this;
			out = (alni*) (((used_slot_head*) out) + 1);

			#ifdef MEM_WRAP
			int1* wrap1 = ((int1*) out);
			int1* wrap2 = ((int1*) ((used_slot_head*) out - 1)) + bsize - CHUNK_WRAP_LEN;

			for (alni i = 0; i < CHUNK_WRAP_LEN; i++) {
				wrap1[i] = WRAP_FILL_VAL_CHUNK;
				wrap2[i] = WRAP_FILL_VAL_CHUNK;
			}

			out = (alni*) ((int1*) out + CHUNK_WRAP_LEN);
			#endif // MEM_WRAP
			#ifdef MEM_ZEROING
			memsetv(out, size, 0);
			#endif
		} else {
			throw Exeption("faled allocate on chunk - chunk is full", false);
		}

		return out;
	}

	void ChunkAlloc::Free(void* p) {

		alni* bdel = (alni*) p;

		#ifdef MEM_WRAP
		int1* wrap1 = ((int1*) p) - CHUNK_WRAP_LEN;
		int1* wrap2 = ((int1*) ((used_slot_head*) p - 1)) + bsize - CHUNK_WRAP_LEN * 2;

		for (alni i = 0; i < CHUNK_WRAP_LEN; i++) {
			int1 val1 = ((int1*) wrap1)[i];
			int1 val2 = ((int1*) wrap2)[i];
			assert((val1 == WRAP_FILL_VAL_CHUNK && val2 == WRAP_FILL_VAL_CHUNK));
		}

		bdel = (alni*) ((int1*) bdel - CHUNK_WRAP_LEN);
		#endif


		#ifdef MEM_ZEROING
		#ifdef MEM_WRAP
		memsetv((int1*) bdel + CHUNK_WRAP_LEN, bsize - sizeof(used_slot_head) - CHUNK_WRAP_LEN * 2, 0);
		#else
		memsetv(bdel, bsize - sizeof(used_slot_head), 0);
		#endif
		#endif

		bdel = (alni*) ((used_slot_head*) bdel - 1);

		((unused_slot_head*) bdel)->bnext = bnext;
		bnext = bdel;

		++bfreec;
	}


	bool ChunkAlloc::isWrapCorrupted() {

		#ifdef MEM_WRAP

		assert(sizeof(unused_slot_head) == sizeof(used_slot_head) && "chunk corruption detection will fail in full buff scan");

		for (alni bidx = 0; bidx < binitc; bidx++) {

			alni* bp = (alni*) get_addr(bidx);
			alni* bp_next = (alni*) get_addr(bidx + 1);

			int1* wrap1 = ((int1*) (bp + 1));
			int1* wrap2 = ((int1*) (bp_next)) - CHUNK_WRAP_LEN;

			for (alni i = 0; i < CHUNK_WRAP_LEN; i++) {
				int1 val1 = ((int1*) wrap1)[i];
				int1 val2 = ((int1*) wrap2)[i];
				if (val1 != WRAP_FILL_VAL_CHUNK || val2 != WRAP_FILL_VAL_CHUNK) {
					return true;
				}
			}
		}
		#endif  // MEM_WRAP
		return false;
	}

	ChunkAlloc::~ChunkAlloc() {
		if (buff) {
			delete[] buff;
		}
	}

	alni ChunkAlloc::sizeAllocatedMem() {
		alni out = 0;

		out += sizeof(alni); // bsize
		out += sizeof(alni); // nblocks
		out += sizeof(alni*); // buff
		out += sizeof(alni*); // mNbnextum
		out += sizeof(alni); // bfreec
		out += sizeof(alni); // binitc

		out += sizeof(alni); // binitc

		out += (alni(bsize / (alnf) sizeof(alni)) * nblocks) * sizeof(alni);

		return out;
	}

	alni ChunkAlloc::sizeUsedMem() {
		alni out = 0;

		out += sizeof(alni); // bsize
		out += sizeof(alni); // nblocks
		out += sizeof(alni*); // buff
		out += sizeof(alni*); // mNbnextum
		out += sizeof(alni); // bfreec
		out += sizeof(alni); // binitc

		out += sizeof(alni); // binitc

		out += (alni(bsize / (alnf) sizeof(alni)) * (nblocks - bfreec)) * sizeof(alni);

		return out;
	}

};

