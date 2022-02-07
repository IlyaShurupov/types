
#pragma once

#include "env.h"
#include "cassert.h"
#include "macros.h"
#include "exeptions.h"

void memset(void* p, alni bytesize, uint1 val);

template <typename TypeIn, typename TypeOut>
TypeOut i2f2i(TypeIn in) {
	union { TypeIn in; TypeOut out; } un;
	un.in = in;
	return un.out;
}

uint8 next_pow_of_2(uint8 v);

template <typename Type = alni>
struct bits {

	Type flags = 0;

	bool get(int1 idx) {
		return flags & (1l << idx);
	}

	void set(int1 idx, bool val) {
		if (val) {
			flags |= (1l << idx);
		}
		else {
			flags &= ~(1l << idx);
		}
	}
};

template <typename T1, typename T2>
struct tuple {

	T1 t1;
	T2 t2;

	tuple() {}
	tuple(T1 t1, T2 t2) {
		this->t1 = t1;
		this->t2 = t2;
	}
};

alni hash(const char* bytes);
alni hash(alni bytes);