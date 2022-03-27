
#include "new.h"

#include <corecrt_malloc.h>

heapalloc* global_heap = NULL;

inline void* operator new(size_t _Size, void* _Where) noexcept {
  return _Where;
}

inline void initialize_global_heap() {
  if (!global_heap) {
    global_heap = new (malloc(sizeof(heapalloc))) heapalloc();
  }
}

inline void finalize_global_heap() {
  global_heap->~heapalloc();
  free(global_heap);
  global_heap = NULL;
}

void* operator new(size_t size) {
  initialize_global_heap();
  return global_heap->alloc(size);
}

void* operator new(size_t size, allocator* alloc) {
  assert(alloc);
  return alloc->alloc(size);
}

void* operator new(size_t size, allocator& alloc) { return alloc.alloc(size); }

void* operator new[](size_t size) {
  initialize_global_heap();
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
  allocator* alloc = *((allocator**)((int1*)p - WRAP_LEN) - 1);
#else
  allocator* alloc = *((allocator**)p - 1);
#endif  // MEM_WRAP
  alloc->free(p);

  if (global_heap == alloc && !global_heap->num) {
    finalize_global_heap();
  }
}

void operator delete(void* p) noexcept { alloc_free(p); }

void operator delete(void* p, allocator* alloc) noexcept { alloc_free(p); }

void operator delete(void* p, allocator& alloc) noexcept { alloc_free(p); }

void operator delete[](void* p) noexcept { alloc_free(p); }

void operator delete[](void* p, heapalloc* halloc) noexcept { alloc_free(p); }

void operator delete[](void* p, heapalloc& halloc) noexcept { alloc_free(p); }

void operator delete(void* p, size_t size) noexcept { alloc_free(p); }

void operator delete[](void* p, size_t size) noexcept { alloc_free(p); }