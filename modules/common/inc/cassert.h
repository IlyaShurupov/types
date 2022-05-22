
#pragma once

namespace tp {

	void assertf(const char* exp, const char* file, int line);

	#undef assert
	#ifdef _DEBUG
	#define assert(exp) if (!(exp)) { tp::assertf(#exp, __FILE__, __LINE__); }
	#else
	#define assert(exp) ((void)0)
	#endif

	#define rterror(def) assertf(def, __FILE__, __LINE__)

};