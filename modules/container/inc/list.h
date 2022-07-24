#pragma once

#include "allocators.h"
#include "sort.h"

#include "poolalloc.h"

#include "file.h"

namespace tp {

	template <class Type> class ListIterator;
	template <class Type> class List;

	template <typename Type>
	class ListNode {
		public:
		Type data;
		ListNode<Type>* next = nullptr;
		ListNode<Type>* prev = nullptr;

		ListNode() {}
		ListNode(Type p_data) { data = p_data; }

		Type& operator->() { return data; }
	};

	template <typename Type>
	class List {

		ListNode<Type>* mFirst = nullptr;
		ListNode<Type>* mLast = nullptr;
		alni mLength = 0;

		PoolAlloc palloc;

		public:
		List(uint2 palloc_chuck_size = 32) : palloc(sizeof(ListNode<Type>), palloc_chuck_size) {}

		inline ListNode<Type>* first() const { return mFirst; }
		inline ListNode<Type>* last() const { return mLast; }
		inline alni length() const { return mLength; }

		void attach(ListNode<Type>* node, ListNode<Type>* node_to) {
			if (node_to) {
				if (node_to->next) {
					node->next = node_to->next;
					node->next->prev = node;
				}
				node_to->next = node;
				node->prev = node_to;
				if (node_to == mLast) {
					mLast = node;
				}
			} else {
				if (mFirst) {
					mFirst->prev = node;
					node->next = mFirst;
					mFirst = node;
				} else {
					mFirst = mLast = node;
				}
			}
			mLength++;
		}

		void detach(ListNode<Type>* node) {
			if (node->next) {
				node->next->prev = node->prev;
			}
			if (node->prev) {
				node->prev->next = node->next;
			}

			if (node == mLast) {
				mLast = mLast->prev;
			}
			if (node == mFirst) {
				mFirst = mFirst->next;
			}

			mLength--;
		}

		ListNode<Type>* find(alni idx) const {
			if (!mFirst || idx < 0 || idx > mLength - 1) {
				return nullptr;
			}
			ListNode<Type>* found = mFirst;
			for (int i = 0; i != idx; i++) {
				found = found->next;
			}
			return found;
		}

		ListNode<Type>* find(Type& data) const {
			ListNode<Type>* found = mFirst;
			for (alni i = 0; data != found->data; i++) {
				if (!found->next) {
					return nullptr;
				}
				found = found->next;
			}
			return found;
		}

		void forEach(void (*functor)(List<Type>* in, ListNode<Type>* node)) {
			ListNode<Type>* node = mFirst;
			while (node) {
				ListNode<Type>* next = node->next;
				functor(this, node);
				node = next;
			}
		}

		inline Type& operator[](ListIterator<Type>& iter) { return iter.node()->data; }
		inline Type& operator[](alni idx) { return find(idx)->data; }
		inline const Type& operator[](alni idx) const {
			assert(idx < mLength && idx > -1);
			return find(idx)->data;
		}

		void pushBack(ListNode<Type>* new_node) { attach(new_node, mLast); }
		void pushFront(ListNode<Type>* new_node) { attach(new_node, nullptr); }

		void pushBack(const Type& data) {
			ListNode<Type>* node = new (palloc) ListNode<Type>();
			node->data = data;
			pushBack(node);
		}
		void pushFront(const Type& data) {
			ListNode<Type>* node = new (palloc) ListNode<Type>();
			node->data = data;
			pushFront(node);
		}

		void insert(ListNode<Type>* node, alni idx) {
			if (idx >= mLength) {
				attach(node, mLast);
			} else if (idx < 0 || !mLength) {
				attach(node, nullptr);
			} else {
				attach(node, find(idx)->prev);
			}
		}

		void insert(Type data, alni idx) {
			ListNode<Type>* node = new (palloc) ListNode<Type>();
			node->data = data;
			insert(node, idx);
		}

		void delNode(ListNode<Type>* node) {
			detach(node);
			delete node;
		}

		void free() {
			forEach([](List<Type>* list, ListNode<Type>* node) { list->delNode(node); });
			mLength = 0;
			mFirst = mLast = nullptr;
		}

		List<Type>& operator += (const List<Type>& in) {
			for (ListNode<Type>* node = in.mFirst; node; node = node->next) {
				pushBack(node->data);
			}
			return *this;
		}

		List<Type>& operator = (const List<Type>& in) {
			free();
			*this += in;
			return *this;
		}

		template <typename compare_val>
		ListNode<Type>* find(bool (*found)(ListNode<Type>* node, compare_val val), compare_val value) const {
			for (ListNode<Type>* node = mFirst; node; node = node->next) {
				if (found(node, value)) {
					return node;
				}
			}
			return nullptr;
		}

		ListIterator<Type> begin() const {
			ListIterator<Type> out(this, 0);
			return out;
		}

		alni end() const {
			return 0;
		}

		alni saveSize() {
			alni out = sizeof(alni) + mLength * sizeof(Type);
		}

		void save(File& file) {
			file.write<alni>(&mLength);
			for (auto item : *this) {
				file.write<Type>(&item.data());
			}
		}

		void load(File& file) {
			free();
			file.read<alni>(&mLength);
			for (auto idx : Range(mLength)) {
				Type block = Type();
				pushBack(block);
				file.read<Type>(&mLast->data);
			}
		}

		~List() {
			free();
		}


		/*

		template <typename SortPolicy = SortMerge>
		void Sort(bool (*compare)(const Type& obj1, const Type& obj2)) {
			if (length < 2) {
				return;
			}

			SortPolicy SortP;

			Type* buffer = new Type[length];

			for (auto iter : *this) {
				*(buffer + iter.Idx()) = iter.Data();
			}

			SortP.Sort(buffer, length, compare);

			for (auto iter : *this) {
				iter.node()->data = *(buffer + iter.Idx());
			}

			delete[] buffer;
		}

		void Invert() {
			ListIterator<Type> i(this, 0);
			ListIterator<Type> j(this, Len() - 1);
			while (i < Len() / 2) {
				SWAP(i.node()->data, j.node()->data, Type);
				++i;
				--j;
			}
		}
		*/

		void detachAll() {
			forEach([](List<Type>* list, ListNode<Type>* node) { list->detach(node); });
		}


		alni sizeAllocatedMem() {
			alni out = 0;
			out += sizeof(ListNode<Type>*) * 2;
			out += sizeof(alni);
			out += palloc.sizeAllocatedMem();
			return out;
		}

		alni sizeUsedMem() {
			alni out = 0;
			out += sizeof(ListNode<Type>*) * 2;
			out += sizeof(alni);
			out += palloc.sizeUsedMem();
			return out;
		}
	};

	template <typename Type>
	class ListIterator {
		ListNode<Type>* mIter;
		alni mIdx;

		public:
		
		ListIterator(const List<Type>* list, alni p_idx) {
			mIdx = p_idx;
			mIter = list->find(mIdx);
		}

		ListNode<Type>* node() { return mIter; }
		Type& data() { return mIter->data; }
		alni idx() { return mIdx; }

		Type& operator->() { return mIter->data; }
		const ListIterator& operator*() { return *this; }

		inline void operator++() {
			mIter = mIter->next;
			mIdx++;
		}

		bool operator==(const ListIterator<Type>& IterNode) { return IterNode.mIter == mIter; }
		bool operator!=(const ListIterator<Type>& in) { return mIter != in.mIter; }

		bool operator==(alni const dummy_end) { return !mIter; }
		bool operator!=(alni const dummy_end) { return mIter; }
	};

};