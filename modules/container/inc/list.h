#pragma once

#include "allocators.h"
#include "sort.h"

#include "list_policies.h"

#include "file.h"

namespace tp {

	template <class Type> class ListIterator;
	template <class Type, typename list_pol> class List;

	template <typename Type>
	class list_node {
		public:
		Type data;
		list_node<Type>* next = nullptr;
		list_node<Type>* prev = nullptr;

		list_node(Type p_data) { data = p_data; }
		list_node() {}

		Type operator->() { return data; }
	};

	template <typename Type, typename list_pol = list_policy_default<Type>>
	class List {

		list_node<Type>* mFirst = nullptr;
		list_node<Type>* mLast = nullptr;
		alni mLength = 0;

		list_pol listh;

		public:
		bool shared_nodes = false;

		public:
		List() {}

		inline list_node<Type>* first() const { return mFirst; }
		inline list_node<Type>* last() const { return mLast; }
		inline alni length() const { return mLength; }

		void attach(list_node<Type>* node, list_node<Type>* node_to) {
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

		void detach(list_node<Type>* node) {
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

		list_node<Type>* find(alni idx) const {
			if (!mFirst || idx < 0 || idx > mLength - 1) {
				return nullptr;
			}
			list_node<Type>* found = mFirst;
			for (int i = 0; i != idx; i++) {
				found = found->next;
			}
			return found;
		}

		list_node<Type>* find(Type& data) const {
			list_node<Type>* found = mFirst;
			for (alni i = 0; data != found->data; i++) {
				if (!found->next) {
					return nullptr;
				}
				found = found->next;
			}
			return found;
		}

		void forEach(void (*functor)(List<Type>* in, list_node<Type>* node)) {
			list_node<Type>* node = mFirst;
			while (node) {
				list_node<Type>* next = node->next;
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

		void pushBack(list_node<Type>* new_node) { attach(new_node, mLast); }
		void pushFront(list_node<Type>* new_node) { attach(new_node, nullptr); }

		void pushBack(const Type& data) {
			list_node<Type>* node = listh.alloc_node();
			node->data = data;
			pushBack(node);
		}
		void pushFront(const Type& data) {
			list_node<Type>* node = listh.alloc_node();
			node->data = data;
			pushFront(node);
		}

		void insert(list_node<Type>* node, alni idx) {
			if (idx >= mLength) {
				attach(node, mLast);
			} else if (idx < 0 || !mLength) {
				attach(node, nullptr);
			} else {
				attach(node, find(idx)->prev);
			}
		}

		void insert(Type data, alni idx) {
			list_node<Type>* node = listh.alloc_node();
			node->data = data;
			insert(node, idx);
		}

		void delNode(list_node<Type>* node) {
			detach(node);
			listh.ValDestruct(node);
			listh.free_node(node);
		}

		void free() {
			forEach([](List<Type>* list, list_node<Type>* node) { list->delNode(node); });
			mLength = 0;
			mFirst = mLast = nullptr;
		}

		List<Type>& operator += (const List<Type>& in) {
			for (list_node<Type>* node = in.mFirst; node; node = node->next) {
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
		list_node<Type>* find(bool (*found)(list_node<Type>* node, compare_val val), compare_val value) const {
			for (list_node<Type>* node = mFirst; node; node = node->next) {
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
			return mLength;
		}

		alni saveSize() {
			alni out = sizeof(alni) + length * sizeof(Type);
		}

		void save(File& file) {
			file.write<alni>(&length);
			for (auto item : *this) {
				file.write<Type>(&item.Data());
			}
		}

		void load(File& file) {
			free();
			file.read<alni>(&length);
			for (auto idx : Range(length)) {
				Type block = Type();
				PushBack(block);
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
		void Release() {
			if (shared_nodes) {
				ForEach([](list<Type>* list, list_node<Type>* node) { list->Detach(node); });
			}
			else {
				ForEach([](list<Type>* list, list_node<Type>* node) { list_node<Type>* del_node = node;  list->Detach(node); delete del_node; });
			}
			length = 0;
			first = last = nullptr;
		}
		*/
	};

	template <typename Type>
	class ListIterator {

		list_node<Type>* iter;
		alni idx;

		public:
		alni Idx() { return idx; }
		Type& operator->() { return iter->data; }
		Type& Data() { return iter->data; }
		list_node<Type>* node() { return iter; }

		ListIterator(const List<Type>* list, alni p_idx) {
			idx = p_idx;
			iter = list->find(idx);
		}

		inline void operator++() {
			iter = iter->next;
			idx++;
		}

		bool operator==(const ListIterator<Type>& IterNode) { return IterNode.iter == iter; }

		bool operator==(alni p_idx) { return idx == p_idx; }
		bool operator!=(alni p_idx) { return idx != p_idx; }
		bool operator!=(const ListIterator<Type>& in) { return iter != in.iter; }
		bool operator>(alni p_idx) { return idx > p_idx; }
		bool operator<(alni p_idx) { return idx < p_idx; }
		bool operator>=(alni p_idx) { return idx >= p_idx; }
		bool operator<=(alni p_idx) { return idx <= p_idx; }

		const ListIterator& operator*() { return *this; }
	};

};