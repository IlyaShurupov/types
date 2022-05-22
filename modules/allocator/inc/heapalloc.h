#pragma once

#include "allocator.h"

#ifdef MEM_WRAP
#define WRAP_FILL_VAL_HEAPALLOC 2
#endif

namespace tp {

	class HeapAlloc : public AbstractAllocator {
		
		public:

		struct MemHead {
			#ifdef MEM_TRACE
			MemHead* next;
			MemHead* prev;
			#endif
			alni size;
			alni reserved;
			AbstractAllocator* alloc;
		};

		private:

		alni mNum = 0;
		#ifdef MEM_TRACE
		MemHead* mEntryPtr = nullptr;
		#endif
		
		public:

		alni num() const { return mNum; }
		MemHead* entry() const { return mEntryPtr; }

		alni sizeReserved() override { return sizeInuse(); }
		bool isAvaliable() override { return true; }

		bool isWrapSupport() override { return true; };
		bool isWrapCorrupted() override;

		void* Alloc(alni size) override;
		void Free(void* p) override;

		bool isEmpty() override;
		alni sizeInuse() override;

		~HeapAlloc() { checkErrorOnExit(); }


	};

};