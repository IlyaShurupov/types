
#pragma once

#include "allocators.h"
#include "vec.h"
#include "rect.h"

namespace tp {

	template <typename Type>
	class Array2D {

		vec2i mSize;
		Type* mBuff = nullptr;

		public:

		Array2D() {}
		Array2D(vec2i size) { resize(size); }
		~Array2D() {
			delete buff;
		}

		vec2i size() const { return mSize; }
		Type* buff() const { return mBuff; }

		inline Type& Get(alni x, alni y) {
			assert(x < size.x&& y < size.y&& x >= 0 && y >= 0);
			return mBuff + mSize.x * y + x;
		}

		inline void Set(alni x, alni y, const Type& value) {
			assert(x < size.x&& y < size.y&& x >= 0 && y >= 0);
			*(mBuff + mSize.x * y + x) = value;
		}

		void resize(vec2i newsize) {
			if (mSize.x != newsize.x || mSize.y != newsize.y) {
				if (mBuff)
					delete mBuff;
				mBuff = new Type[newsize.x * newsize.y];
				mSize = newsize;
			}
		}

		void project(Array2D<Type>* in, vec2i pos) {

			recti unclamped_fromrect(pos, in->mBuff);
			recti fromrect;
			recti torect(vec2i(), mBuff);

			torect.intersection(unclamped_fromrect, fromrect);

			vec2i clampsize(unclamped_fromrect.size - fromrect.size);

			vec2<bool> mns(unclamped_fromrect.pos.x < 0, unclamped_fromrect.pos.y < 0);

			if (mns.x && mns.y) {
				for (int i = 0; i < fromrect.size.x; i++) {
					for (int j = 0; j < fromrect.size.y; j++) {
						int to = mSize.x * (fromrect.pos.y + j) + (fromrect.pos.x + i);
						mBuff[to] = in->mBuff[in->mSize.x * (j + clampsize.y) + (i + clampsize.x)];
					}
				}
			} else if (!mns.x && !mns.y) {
				for (int i = 0; i < fromrect.size.x; i++) {
					for (int j = 0; j < fromrect.size.y; j++) {
						int to = mSize.x * (fromrect.pos.y + j) + (fromrect.pos.x + i);
						mBuff[to] = in->mBuff[in->mSize.x * j + i];
					}
				}
			} else if (!mns.x && mns.y) {
				for (int i = 0; i < fromrect.size.x; i++) {
					for (int j = 0; j < fromrect.size.y; j++) {
						int to = mSize.x * (fromrect.pos.y + j) + (fromrect.pos.x + i);
						mBuff[to] = in->mBuff[in->mSize.x * (j + clampsize.y) + i];
					}
				}
			} else {
				for (int i = 0; i < fromrect.size.x; i++) {
					for (int j = 0; j < fromrect.size.y; j++) {
						int to = mSize.x * (fromrect.pos.y + j) + (fromrect.pos.x + i);
						mBuff[to] = in->mBuff[in->mSize.x * j + (i + clampsize.x)];
					}
				}
			}
		}

		void assign(const Type& value) {
			halni len = mSize.x * mSize.y;
			for (halni i = 0; i < len; i++) {
				mBuff[i] = value;
			}
		}


		alni sizeAllocatedMem() {
			alni out = 0;
			out += mSize.sizeAllocatedMem();
			out += sizeof(Type*);
			if (mBuff) {
				out += sizeof(Type) * mSize.x * mSize.y;
			}
			return out;
		}

		alni sizeUsedMem() {
			alni out = 0;
			out += mSize.sizeAllocatedMem();
			out += sizeof(Type*);
			if (mBuff) {
				out += sizeof(Type) * mSize.x * mSize.y;
			}
			return out;
		}
	};

};