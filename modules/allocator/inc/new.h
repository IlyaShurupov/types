
#pragma once

#include "alloc_cfg.h"

#include "heapalloc.h"

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

