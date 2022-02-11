
#include "new.h"

#include "allocator.h"

#include "heapalloc.h"

#undef new
#undef delete
#undef alloc
#undef free

allocator* alloc;

void setalloc(class allocator* p_alloc) {
	alloc = p_alloc;
}

#ifdef MEM_TRACE

void* operator new(size_t size, class allocator* alloc, const char* file, int line) {
	return alloc->alloc(size, file, line);
}

void operator delete(void* p, class allocator* alloc, const char* file, int line) {}


#else

void* operator new(size_t size, class allocator* alloc) {
	return alloc->alloc(size);
}

void operator delete(void* p, class allocator* alloc) {}
#endif 

void operator delete(void* p, void* alloc) {
	((allocator*)alloc)->free(p);
}

void operator delete  (void* ptr, size_t sz) noexcept {
}