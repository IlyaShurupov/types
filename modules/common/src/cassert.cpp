
#include "common.h"

#include <stdio.h>

void assertf(const char* exp, const char* file, int line) {
	if (!exp) {
		exp = "no info";
	}
	printf(" assertion failure - %s \n in file %s %i\n", exp, file, line);
	throw typesExeption(exp, true);
}