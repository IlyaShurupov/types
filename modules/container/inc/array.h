#pragma once

#include "allocators.h"


#include "filesystem.h"

namespace tp {

	template <typename Type>
	class ArrayIterator;

	template <typename Type>
	class Array {

		void allocate(alni p_bufflen) {
			mLength = p_bufflen;
			mBuff = new Type[mLength]();
		}

		Type* mBuff;
		alni mLength;

		public:

		Array() {
			mLength = 0;
			mBuff = nullptr;
		}

		Array(alni p_length) {
			mLength = p_length;
			reserve(mLength);
		}

		alni length() const {
			return mLength;
		}

		Type* buff() const {
			return mBuff;
		}

		void reserve(alni p_bufflen) {
			free();
			mLength = p_bufflen;
			mBuff = new Type[mLength]();
		}

		void free() {
			mLength = 0;

			if (mBuff) {
				delete[] mBuff;
			}
			mBuff = NULL;
		}

		void insert(Type& p_block, alni idx) {
			Type* current = mBuff;
			allocate(mLength + 1);

			for (alni befor = 0; befor < idx; befor++) {
				mBuff[befor] = current[befor];
			}
			for (alni after = idx; after < mLength - 1; after++) {
				mBuff[after + 1] = current[after];
			}

			mBuff[idx] = p_block;

			if (current) {
				delete[] current;
			}
		}

		void remove(alni p_idx) {
			Type* current = mBuff;
			alni current_len = mLength;
			allocate(mLength - 1);

			for (alni befor = 0; befor < p_idx; befor++) {
				mBuff[befor] = current[befor];
			}
			for (alni after = p_idx + 1; after < mLength + 1; after++) {
				mBuff[after - 1] = current[after];
			}

			delete[] current;
		}

		void removeVal(Type val) {
			for (int i = 0; i < mLength; i++) {
				if (mBuff[i] == val) {
					remove(i);
				}
			}
		}

		void operator=(const Array& array) {
			reserve(array.mLength);
			for (int i = 0; i < array.mLength; i++) {
				mBuff[i] = array.mBuff[i];
			}
		}

		void operator+=(const Array& array) {
			if (!mBuff) {
				return operator=(array);
			}

			alni new_len = array.mLength + mLength;
			Type* newbuff = new Type[new_len]();

			for (halni i = 0; i < mLength; i++) {
				newbuff[i] = mBuff[i];
			}
			for (halni i = 0; i < array.mLength; i++) {
				newbuff[mLength + i] = array.mBuff[i];
			}

			delete[] mBuff;
			mBuff = newbuff;
			mLength = new_len;
		}

		inline Type& operator[](alni idx) {
			assert(idx >= 0 && idx < mLength);
			return mBuff[idx];
		}

		void pushBack(Type block) {
			insert(block, mLength);
		}

		Array(const Array& array) {
			allocate(array.mLength);
			for (int i = 0; i < array.mLength; i++) {
				mBuff[i] = array.mBuff[i];
			}
		}

		ArrayIterator<Type> begin() { return ArrayIterator<Type>(this); }
		alni end() { return mLength; }

		alni saveSize() {
			return mLength * sizeof(Type) + sizeof(mLength);
		}

		void save(File& file) {
			file.write(mLength);
			file.write_bytes((int1*) mBuff, mLength * sizeof(Type));
		}

		void load(File& file) {
			file.read(&mLength);
			reserve(mLength);
			file.read_bytes((int1*) mBuff, mLength * sizeof(Type));
		}

		~Array() {
			free();
		}
	};

	template <typename Type>
	class ArrayIterator {
		alni mIdx = 0;
		Array<Type>* mArrayPtr;
		public:

		ArrayIterator(Array<Type>* array) : mArrayPtr(array) {}

		Type& data() const { return (*mArrayPtr)[mIdx]; }
		Type& idx() const { return mIdx; }
		operator alni() { return mIdx; }

		Type* operator->() { return &(*mArrayPtr)[mIdx]; }
		const ArrayIterator& operator*() { return *this; }

		inline void operator++() { mIdx++; }
		bool operator==(alni p_idx) { return mIdx == p_idx; }
		bool operator!=(alni p_idx) { return mIdx != p_idx; }
		bool operator>(alni p_idx) { return mIdx > p_idx; }
		bool operator<(alni p_idx) { return mIdx < p_idx; }
		bool operator>=(alni p_idx) { return mIdx >= p_idx; }
		bool operator<=(alni p_idx) { return mIdx <= p_idx; }
	};

};