
#pragma once

#include "env.h"
#include "cassert.h"
#include "macros.h"
#include "exeptions.h"
#include "timer.h"
#include "random.h"


#include <limits>

void memset(void* p, alni bytesize, uint1 val);
void memcp(void* left, const void* right, alni len);
bool memequal(const void* left, const void* right, alni len);

template <typename TypeIn, typename TypeOut>
TypeOut i2f2i(TypeIn in) {
	union { TypeIn in; TypeOut out; } un;
	un.in = in;
	return un.out;
}

uint8 next_pow_of_2(uint8 v);

template <typename Type = alni>
struct bits {

	Type flags;

	bits() {
#ifdef ENVIRONMENT_DEBUG
		flags = 0;
#endif // DEBUGs
	}

	bits(Type val) {
		flags = val;
	}

	bits(bool val) {
		for (int bit = 0; bit < sizeof(Type); bit++) {
			set(bit, val);
		}
	}

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

struct range_iter {
	alni idx;

	range_iter(alni st) : idx(st) {}

	inline void operator++() {
		idx++;
	}

	operator alni() const { return idx; }

	bool operator==(alni p_idx) { return idx == p_idx; }
	bool operator!=(alni p_idx) { return idx != p_idx; }
	const range_iter& operator*() { return *this; }
};

struct range {

	range() : st(0), nd(0) {}
	range(alni st, alni nd) : st(st), nd(nd) {}

	alni st;
	alni nd;

	bool valid() {
		return st < nd;
	}

	range_iter begin() {
		return range_iter(st);
	}

	alni end() {
		return nd;
	}
};

alni hash(const char* bytes);
alni hash(alni bytes);

#ifdef PLATFORM_WINDOWS
const char* working_dir();
#endif

void char2wide(const char* c, wchar_t* out, alni len);
void wide2char(const wchar_t* c, char* out);

