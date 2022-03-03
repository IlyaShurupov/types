
#include "new.h"

#include <corecrt_malloc.h>

heapalloc* global_heap = NULL;
bool global_heap_ninitialized = true;

inline void* operator new(size_t _Size, void* _Where) noexcept { return _Where; }

// initializes global_heap
// I intentionally do not free global heap due to undefined order of destruction of global variables (undefined for me)
// yep, consistent at-exit 18b memory leak, os shall take care of that
inline void init_global_heap() {
	if (global_heap_ninitialized) {
		global_heap = new (malloc(sizeof(heapalloc))) heapalloc();
		global_heap_ninitialized = false;
	}
}

void* operator new(size_t size) {
	init_global_heap();
	return global_heap->alloc(size);
}

void* operator new(size_t size, allocator* alloc) {
	assert(alloc);
	return alloc->alloc(size);
}

void* operator new(size_t size, allocator& alloc) {
	return alloc.alloc(size);
}

void* operator new[](size_t size) {
	init_global_heap();
	return global_heap->alloc(size);
}

void* operator new[](size_t size, heapalloc& halloc) {
	return halloc.alloc(size);
}

void* operator new[](size_t size, heapalloc* halloc) {
	return halloc->alloc(size);
}

static inline void alloc_free(void* p) {
#ifdef MEM_WRAP
	allocator** alloc = ((allocator**)((int1*)p - WRAP_LEN) - 1);
#else
	allocator** alloc = ((allocator**)p - 1);
#endif // MEM_WRAP
	(*alloc)->free(p);
}

void  operator delete(void* p) noexcept {
	alloc_free(p);
}

void  operator delete(void* p, allocator* alloc) noexcept {
	alloc_free(p);
}

void  operator delete(void* p, allocator& alloc) noexcept {
	alloc_free(p);
}

void  operator delete[](void* p) noexcept {
	alloc_free(p);
}

void  operator delete[](void* p, heapalloc* halloc) noexcept {
	alloc_free(p);
}

void  operator delete[](void* p, heapalloc& halloc) noexcept {
	alloc_free(p);
}

void  operator delete(void* p, size_t size) noexcept {
	alloc_free(p);
}

void  operator delete[](void* p, size_t size) noexcept {
	alloc_free(p);
}