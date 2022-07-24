#include "allocator.h"

tp::AbstractAllocator** tp::get_allocator_from_ptr(void* ptr) {
	#ifdef MEM_WRAP
	tp::AbstractAllocator** alloc = ((tp::AbstractAllocator**) ((tp::int1*) ptr - WRAP_LEN) - 1);
	#else
	allocator* alloc = ((allocator**) ptr - 1);
	#endif  // MEM_WRAP
	return alloc;
}
