
#include "common.h"

#include <stdio.h>

namespace tp {

	void assertf(const char* exp, const char* file, int line) {
		if (!exp) {
			exp = "no info";
		}
		printf(" assertion failure - %s \n in file %s %i\n", exp, file, line);
		throw Exeption(exp, true);
	}

};