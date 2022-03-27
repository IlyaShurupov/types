
#pragma once

#include "alloc_cfg.h"

#include "heapalloc.h"
#include "chunkalloc.h"

#ifdef MEM_WRAP
#if WRAP_FILL_VAL_HEAPALLOC == WRAP_FILL_VAL_CHUNK
#error "WRAP_FILL_VAL_HEAPALLOC == WRAP_FILL_VAL_CHUNK"
#endif
#endif  // WRAP_


extern heapalloc* global_heap;

void* operator new(size_t _Size);
void* operator new(size_t _Size, allocator* alloc);
void* operator new(size_t _Size, allocator& alloc);
void* operator new[](size_t _Size);
void* operator new[](size_t _Size, heapalloc& alloc);
void* operator new[](size_t _Size, heapalloc* alloc);

void  operator delete(void* _Block) noexcept;
void  operator delete(void* _Block, allocator* alloc) noexcept;
void  operator delete(void* _Block, allocator& alloc) noexcept;
void  operator delete[](void* _Block) noexcept;
void  operator delete[](void* _Block, heapalloc* alloc) noexcept;
void  operator delete[](void* _Block, heapalloc& alloc) noexcept;

void  operator delete(void* _Block, size_t _Size) noexcept;
void  operator delete[](void* _Block, size_t _Size) noexcept;

/*
inline void* operator new(size_t _Size, void* _Where) noexcept { return _Where; }
inline void* operator new[](size_t _Size, void* _Where) noexcept { return _Where; }
*/

