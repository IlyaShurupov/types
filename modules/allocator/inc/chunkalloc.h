#pragma once

#include "allocator.h"

#ifdef MEM_WRAP
#define WRAP_FILL_VAL_CHUNK 4
#if WRAP_LEN % ALIGNED_SIZE_B != 0
#error "WRAP_LEN must by multiple of sizeof(alni)"
#endif
#define CHUNK_WRAP_LEN WRAP_LEN
#endif

namespace tp {
	class ChunkAlloc : public AbstractAllocator {
		alni bsize = 0;
		alni nblocks = 0;

		alni* buff = NULL;
		alni* bnext = NULL;
		alni bfreec = 0;
		alni binitc = 0;

		public:

		ChunkAlloc(alni bsize, alni nblocks);

		bool isAvaliable() override;
		alni sizeInuse() override;
		alni sizeReserved() override;
		bool isEmpty() override;
		void* Alloc(alni size) override;
		virtual void Free(void* p) override;
		bool isWrapSupport() override { return true; }
		bool isWrapCorrupted() override;

		alni blockSize();

		~ChunkAlloc();

		alni sizeAllocatedMem();
		alni sizeUsedMem();

		public:
		struct unused_slot_head { alni* bnext; };
		struct used_slot_head { ChunkAlloc* chunk_p; };
		static alni calc_bsize(alni bsize);

		private:
		inline void* get_addr(alni idx) const;
		inline alni get_idx(const void* address) const;
	};
};