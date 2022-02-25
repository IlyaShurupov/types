
#include "new.h"

heapalloc main_heap;

void* operator new(size_t size) {
	return main_heap.alloc(size);
}

void* operator new(size_t size, allocator* alloc) {
	assert(alloc);
	return alloc->alloc(size);
}

void* operator new(size_t size, allocator& alloc) {
	return alloc.alloc(size);
}

void* operator new[](size_t size) {
	return main_heap.alloc(size);
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