
#include "allocators.h"

#include <stdio.h>

#define FAILED 1
#define SUCCEED 0

bool test_heapalloc();
bool test_chunkalloc();
bool test_poolalloc();

typedef bool (testf)();

testf* tests[] = {
	test_heapalloc,
	test_chunkalloc,
	test_poolalloc,
	nullptr
};

int main() {

	for (int i = 0; tests[i]; i++) {
		bool res;
		try {
			res = tests[i]();
		}
		catch (...) {
			res = FAILED;
		}

		if (res == SUCCEED) {
			printf("test %i - passed\n", i);
		}
		else {
			printf("test %i - failed\n", i);
		}
	}
}

// -------------------- tests ----------------------------- //

struct test_struct {
	alni val;
	test_struct() { val = 0; }
	~test_struct() { val = 0; }
};

bool test_heapalloc() {

	heapalloc halloc;

	test_struct* arr = new(&halloc) test_struct();
	mfree(&halloc, arr);

	return halloc.inuse_size() ? FAILED : SUCCEED;
}

bool test_chunkalloc() {
	return FAILED;
}

bool test_poolalloc() {
	return FAILED;
}