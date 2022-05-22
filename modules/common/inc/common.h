
#pragma once

#include "env.h"
#include "cassert.h"
#include "macros.h"
#include "exeptions.h"
#include "timer.h"
#include "random.h"

namespace tp {

	void memsetv(void* p, alni bytesize, uint1 val);
	void memcp(void* left, const void* right, alni len);
	bool memequal(const void* left, const void* right, alni len);

	template <typename Type>
	constexpr Type abs(const Type& val) {
		return ABS(val);
	}

	template <typename TypeIn, typename TypeOut>
	TypeOut i2f2i(TypeIn in) {
		union { TypeIn in; TypeOut out; } un;
		un.in = in;
		return un.out;
	}

	uint8 next_pow_of_2(uint8 v);

	template <typename T>
	inline void swap(T& t1, T& t2) {
		T tmp = t1;
		t1 = t2;
		t2 = tmp;
	}

	template <typename T>
	inline T clamp(T t1, const T t2, const T t3) {
		CLAMP(t1, t2, t3)
			return t1;
	}

	template <typename Type = alni>
	class Bits {

		Type mFlags;

		public:

		Bits() {
			#ifdef ENVIRONMENT_DEBUG
			mFlags = 0;
			#endif // DEBUGs
		}

		Bits(Type val) {
			mFlags = val;
		}

		Bits(bool val) {
			for (int bit = 0; bit < sizeof(Type); bit++) {
				set(bit, val);
			}
		}

		bool get(int1 idx) {
			return mFlags & (1l << idx);
		}

		void set(int1 idx, bool val) {
			if (val) {
				mFlags |= (1l << idx);
			} else {
				mFlags &= ~(1l << idx);
			}
		}
	};

	class RangeIterator {
		alni mIndex;
		public:

		RangeIterator(alni pStartIndex) : mIndex(pStartIndex) {}
		alni index() const { return mIndex; }
		inline void operator++() { mIndex++; }
		inline operator alni() const { return mIndex; }
		inline bool operator==(alni pIndex) { return mIndex == pIndex; }
		inline bool operator!=(alni pIndex) { return mIndex != pIndex; }
		inline const RangeIterator& operator*() { return *this; }
	};

	class Range {
		alni mBeginIdx;
		alni mEndIdx;
		public:

		Range() : mBeginIdx(0), mEndIdx(0) {}
		Range(alni pEndIndex) : mBeginIdx(0), mEndIdx(pEndIndex) {}
		Range(alni pStartIndex, alni pEndIndex) : mBeginIdx(pStartIndex), mEndIdx(pEndIndex) {}

		bool valid() { return mBeginIdx < mEndIdx; }
		alni idxBegin() const { return mBeginIdx; }
		alni idxEnd() const { return mEndIdx; }
		RangeIterator begin() { return RangeIterator(mBeginIdx); }
		alni end() { return mEndIdx; }
	};

	alni hash(const char* bytes);
	alni hash(alni bytes);

	#ifdef PLATFORM_WINDOWS
	const char* working_dir();
	#endif

	void char2wide(const char* c, wchar_t* out, alni len);
	void wide2char(const wchar_t* c, char* out);

};
