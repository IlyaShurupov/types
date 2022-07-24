#pragma once

#include "chunkalloc.h"

namespace tp {
	class PoolAlloc : public AbstractAllocator {

		struct chunk_list {

			struct chunk_node : public ChunkAlloc {
				PoolAlloc* self;
				chunk_node* next = NULL;
				chunk_node* prev = NULL;

				chunk_node(PoolAlloc* self, alni bsize, alni nblocks) : self(self), ChunkAlloc(bsize, nblocks) {}
				alni sizeAllocatedMem();
				alni sizeUsedMem();

				virtual void Free(void* p) override {
					self->Free(p);
				}
			};

			struct chunk_node* last = NULL;

			chunk_node* addchunk(PoolAlloc* self, alni bsize, alni nblocks);
			void delchunk(chunk_node* node);
			void initialize();
			void finalize();
			alni sizeAllocatedMem();
			alni sizeUsedMem();
		};
		
		chunk_list chunks;
		chunk_list::chunk_node* last_used;
		alni bsize;
		alni nblocks;

		public:

		PoolAlloc(alni pbsize, alni pnblocks);
		
		bool isAvaliable() override;
		alni sizeInuse() override;
		alni sizeReserved() override;
		bool isEmpty() override;

		void* Alloc(alni size) override;
		void Free(void* p) override;

		bool isWrapSupport() override { return true; }
		bool isWrapCorrupted() override;

		alni sizeAllocatedMem();
		alni sizeUsedMem();
		
		~PoolAlloc();
	};
};