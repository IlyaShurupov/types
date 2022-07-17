
#pragma once

#include "alloc_cfg.h"

#include "heapalloc.h"
#include "chunkalloc.h"

#ifdef MEM_WRAP
#if WRAP_FILL_VAL_HEAPALLOC == WRAP_FILL_VAL_CHUNK
#error "WRAP_FILL_VAL_HEAPALLOC == WRAP_FILL_VAL_CHUNK"
#endif
#endif  // WRAP_

#define ALLOCATOR_PLACEMENT_NEW

extern tp::HeapAlloc* global_heap;

void* operator new(size_t _Size);
void* operator new(size_t _Size, tp::AbstractAllocator* alloc);
void* operator new(size_t _Size, tp::AbstractAllocator& alloc);
void* operator new[](size_t _Size);
void* operator new[](size_t _Size, tp::HeapAlloc& alloc);
void* operator new[](size_t _Size, tp::HeapAlloc* alloc);

void  operator delete(void* _Block) noexcept;
void  operator delete(void* _Block, tp::AbstractAllocator* alloc) noexcept;
void  operator delete(void* _Block, tp::AbstractAllocator& alloc) noexcept;
void  operator delete[](void* _Block) noexcept;
void  operator delete[](void* _Block, tp::HeapAlloc* alloc) noexcept;
void  operator delete[](void* _Block, tp::HeapAlloc& alloc) noexcept;

void  operator delete(void* _Block, size_t _Size) noexcept;
void  operator delete[](void* _Block, size_t _Size) noexcept;

#ifdef ALLOCATOR_PLACEMENT_NEW
inline void* operator new(size_t _Size, void* _Where) noexcept;
//inline void* operator new[](size_t _Size, void* _Where) noexcept;
#endif

