
#include "common.h"

void memset(void* p, alni bytesize, uint1 val) {

	alni alignedval = 0;
	for (alni idx = 0; idx < sizeof(alni); idx++) {
		((uint1*)&alignedval)[idx] = val;
	}

	alni alignedlen = bytesize / sizeof(alni);
	for (alni idx = 0; idx < alignedlen; idx++) {
		((alni*)p)[idx] = val;
	}

	alni unalignedlen = bytesize - (alignedlen * sizeof(alni));
	for (alni idx = 0; idx < unalignedlen; idx++) {
		((uint1*)p)[bytesize - idx] = val;
	}
}

uint8 next_pow_of_2(uint8 v) {
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v |= v >> 32;
	return v + 1;
}

alni hash(const char* bytes) {
	unsigned long hash = 5381;
	int c;
	while ((c = *bytes++)) {
		hash = ((hash << 5) + hash) + c;
	}
	return hash;
}

alni hash(alni bytes) {
	return bytes;
}