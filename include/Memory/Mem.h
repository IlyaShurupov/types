#pragma once

//BAD!  defenetly needs cleanup & optimization

#include <cstddef>

#define ENVIRONMENT64

// Types
typedef char					      int1;
typedef unsigned char			  uint1;
typedef short					      int2;
typedef unsigned short		  uint2;
typedef int						      int4;
typedef unsigned int			  uint4;
#ifdef ENVIRONMENT64
typedef long long			      int8;
typedef float								flt4;
typedef double							flt8;
typedef unsigned long long  uint8;
typedef int8                alni;
typedef flt8                alnf;
#define ALIGNED_MAX LLONG_MAX
#define ALIGNED_MIN -LLONG_MAX
#else
typedef int4                    aligned;
typedef long long				int8;
typedef unsigned long long		uint8;
#define ALIGNED_MAX LLONG_MAX
#define ALIGNED_MIN LLONG_MIN
#endif


struct MemHead {
	MemHead* next;
	MemHead* prev;
	alni size;
	const char* type;
	const char* file;
	alni line;
	alni time;
	alni reserved;
};

#ifdef MEM_DEBUG_WRAP
#define MEMH_FROM_PTR(ptr) ((MemHead*)((int1*)ptr - WRAP_LEN) - 1)
#else
#define MEMH_FROM_PTR(ptr) ((MemHead*)ptr - 1)
#endif

void* operator new (size_t size, const char* file, int line);
void operator delete (void* p);
void operator delete (void* p, const char* file, int line);
#define new new (__FILE__, __LINE__)

uint8 mem_allocated_size();

void mem_test_types();