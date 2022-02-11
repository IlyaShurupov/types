
#pragma once

#include "alloc_cfg.h"


// raw allocations
#ifdef MEM_TRACE
	#define MDBG , __FILE__, __LINE__
#else
	#define MDBG
#endif 


void* operator new(size_t size);
//void operator delete (void* p, void* alloc);

void operator delete  (void* ptr, size_t sz) noexcept;

#ifdef MEM_TRACE
void* operator new(size_t size, class allocator* alloc, const char* file, int line);
void operator delete(void* p, class allocator* alloc, const char* file, int line);

#define new(alloc_ptr) new(alloc_ptr, __FILE__, __LINE__)

#else
void* operator new(size_t size, class allocator* alloc);
void operator delete(void* p, class allocator* alloc);
#endif 

#define mfree(alloc_ptr, ptr) { delete ptr; (alloc_ptr)->free(ptr); }