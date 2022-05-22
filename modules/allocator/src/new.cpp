
#include "new.h"

#include <corecrt_malloc.h>

tp::HeapAlloc* global_heap = NULL;

inline void* operator new(size_t _Size, void* _Where) noexcept {
	return _Where;
}

inline void initialize_global_heap() {
	if (!global_heap) {
		global_heap = new (malloc(sizeof(tp::HeapAlloc))) tp::HeapAlloc();
	}
}

inline void finalize_global_heap() {
	global_heap->~HeapAlloc();
	free(global_heap);
	global_heap = NULL;
}

void* operator new(size_t size) {
	initialize_global_heap();
	return global_heap->Alloc(size);
}

void* operator new(size_t size, tp::AbstractAllocator* alloc) {
	assert(alloc);
	return alloc->Alloc(size);
}

void* operator new(size_t size, tp::AbstractAllocator& alloc) { return alloc.Alloc(size); }

void* operator new[](size_t size) {
	initialize_global_heap();
	return global_heap->Alloc(size);
}

void* operator new[](size_t size, tp::HeapAlloc& halloc) {
	return halloc.Alloc(size);
}

void* operator new[](size_t size, tp::HeapAlloc* halloc) {
	return halloc->Alloc(size);
}

static inline void alloc_free(void* p) {
	#ifdef MEM_WRAP
	tp::AbstractAllocator* alloc = *((tp::AbstractAllocator**) ((tp::int1*) p - WRAP_LEN) - 1);
	#else
	allocator* alloc = *((allocator**) p - 1);
	#endif  // MEM_WRAP
	alloc->Free(p);

	if (global_heap == alloc && !global_heap->num()) {
		finalize_global_heap();
	}
}

void operator delete(void* p) noexcept { alloc_free(p); }

void operator delete(void* p, tp::AbstractAllocator* alloc) noexcept { alloc_free(p); }

void operator delete(void* p, tp::AbstractAllocator& alloc) noexcept { alloc_free(p); }

void operator delete[](void* p) noexcept { alloc_free(p); }

void operator delete[](void* p, tp::HeapAlloc* halloc) noexcept { alloc_free(p); }

void operator delete[](void* p, tp::HeapAlloc& halloc) noexcept { alloc_free(p); }

void operator delete(void* p, size_t size) noexcept { alloc_free(p); }

void operator delete[](void* p, size_t size) noexcept { alloc_free(p); }
