
#include "common.h"

void assertf(const char* exp, const char* file, int line) {
	throw typesExeption(exp, true);
}