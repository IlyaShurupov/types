
#pragma once

#define TYPES_V2

#include <limits.h>
#include <float.h>

#ifdef _DEBUG
#define ENVIRONMENT_DEBUG
#else  
#define ENVIRONMENT_RELEASE
#endif // _DEBUG


#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif

#define PLATFORM_WINDOWS
#else
#error "can not determind target architecture"
#endif

namespace tp {

	typedef char										int1;
	typedef unsigned char						uint1;
	typedef short										int2;
	typedef unsigned short					uint2;
	typedef int											int4;
	typedef unsigned int						uint4;
	typedef long long								int8;
	typedef unsigned long long			uint8;
	typedef float										flt4;
	typedef double									flt8;

	#ifdef ENVIRONMENT64
	typedef uint4										uhalni;
	typedef int4										halni;
	typedef flt4										halnf;

	typedef int8										alni;
	typedef flt8										alnf;

	#define ALIGNED_MAX LLONG_MAX
	#define ALIGNED_MIN -LLONG_MAX
	#define ALIGNED_SIZE 64
	#define ALIGNED_SIZE_B 8

	#else
	typedef uint2										uhalni;
	typedef int2										halni;
	typedef flt2										halnf;
	typedef flt4										alnf;
	typedef int4                    alni;

	#define ALIGNED_MAX INT_MAX
	#define ALIGNED_MIN -INT_MAX
	#define ALIGNED_SIZE 32
	#define ALIGNED_SIZE_B 4
	#endif


};