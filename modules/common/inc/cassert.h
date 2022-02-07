
#pragma once

void assertf(const char* exp, const char* file, int line);

#undef assert
#ifdef _DEBUG
#define assert(exp) if (!(exp)) { assertf(#exp, __FILE__, __LINE__); }
#else
#define assert(exp) ((void)0)
#endif