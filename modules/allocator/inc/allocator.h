#pragma once

#include "alloc_cfg.h"
#include "common.h"

namespace tp {
	class AbstractAllocator {
		public:
		virtual bool isAvaliable() = 0;
		virtual alni sizeReserved() = 0;
		virtual alni sizeInuse() = 0;

		virtual bool isEmpty() = 0;

		virtual bool isWrapSupport() = 0;
		virtual bool isWrapCorrupted() = 0;

		virtual void* Alloc(alni size) = 0;
		virtual void Free(void* p) = 0;

		void checkErrorOnExit() {
			assert(isEmpty() && "destruction of unfreed allocator was detected");
			#ifdef MEM_WRAP
			if (isWrapSupport()) {
				assert(!isWrapCorrupted() && "memory corruption detected by mem guards");
			}
			#endif
		}

		~AbstractAllocator() {}
	};
};