#pragma once

#include "allocators.h"
#include "sort.h"

#include "list_policies.h"

//#include <cstdarg>

template <class Type> class ListIterator;
template <class Type, typename list_pol> class list;

template <typename Type>
class list_node {
public:
	Type data;
	list_node<Type>* next = nullptr;
	list_node<Type>* prev = nullptr;

	list_node(Type p_data) { data = p_data; }
	list_node() {}

	Type operator->() { return data; }

	void FreeData() {
		delete data;
	}
};

template <typename Type, typename list_pol = list_policy_default<Type>>
class list {

	list_node<Type>* first = nullptr;
	list_node<Type>* last = nullptr;
	alni length = 0;

	list_pol listh;

public:
	bool shared_nodes = false;

public:
	list() {}

	inline list_node<Type>* First() { return first; }
	inline list_node<Type>* Last() { return last; }

	inline const list_node<Type>* First() const { return first; }
	inline const list_node<Type>* Last() const { return last; }
	
	inline alni Len() { return length; }

	void Attach(list_node<Type>* node, list_node<Type>* node_to) {
		if (node_to) {
			if (node_to->next) {
				node->next = node_to->next;
				node->next->prev = node;
			}
			node_to->next = node;
			node->prev = node_to;
			if (node_to == last) {
				last = node;
			}
		}
		else {
			if (first) {
				first->prev = node;
				node->next = first;
				first = node;
			}
			else {
				first = last = node;
			}
		}
		length++;
	}

	void Detach(list_node<Type>* node) {
		if (node->next) {
			node->next->prev = node->prev;
		}
		if (node->prev) {
			node->prev->next = node->next;
		}

		if (node == last) {
			last = last->prev;
		}
		if (node == first) {
			first = first->next;
		}

		length--;
	}

	list_node<Type>* Find(alni idx) {
		if (!First() || idx < 0 || idx > Len() - 1) {
			return nullptr;
		}
		list_node<Type>* found = First();
		for (int i = 0; i != idx; i++) {
			found = found->next;
		}
		return found;
	}

	list_node<Type>* Find(Type data) {
		list_node<Type>* found = First();
		for (alni i = 0; data != found->data; i++) {
			if (!found->next) {
				return nullptr;
			}
			found = found->next;
		}
		return found;
	}

	void ForEach(void (*functor)(list<Type>* in, list_node<Type>* node)) {
		list_node<Type>* node = First();
		while (node) {
			list_node<Type>* next = node->next;
			functor(this, node);
			node = next;
		}
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

	inline Type& operator[](ListIterator<Type>& iter) { return iter.node()->data; }
	inline Type& operator[](alni idx) { return Find(idx)->data; }

	void PushBack(list_node<Type>* new_node) { Attach(new_node, Last()); }
	void PushFront(list_node<Type>* new_node) { Attach(new_node, nullptr); }

	void PushBack(Type data) {
		list_node<Type>* node = listh.alloc_node();
		node->data = data;
		PushBack(node);
	}
	void PushFront(Type data) {
		list_node<Type>* node = listh.alloc_node();
		node->data = data;
		PushFront(node);
	}

	void Insert(list_node<Type>* node, alni idx) {
		if (idx >= Len()) {
			Attach(node, Last());
		}
		else if (idx < 0 || !Len()) {
			Attach(node, nullptr);
		}
		else {
			Attach(node, Find(idx)->prev);
		}
	}

	void Insert(Type data, alni idx) {
		list_node<Type>* node = listh.alloc_node();
		node->data = data;
		Insert(node, idx);
	}

	void DelNode(list_node<Type>* node) {
		Detach(node);
		listh.ValDestruct(node);
		listh.free_node(node);
	}

	/*
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

	void Delete() {
		ForEach([](list<Type>* list, list_node<Type>* node) { list->DelNode(node); });
		length = 0;
		first = last = nullptr;
	}

	list<Type>& operator += (const list<Type>& in) {
		for (list_node<Type>* node = in.first; node; node = node->next) {
			PushBack(node->data);
		}
		return *this;
	}

	list<Type>& operator = (const list<Type>& in) {
		Clear();
		*this += in;
		return *this;
	}

	template <typename compare_val>
	list_node<Type>* Find(bool (*found)(list_node<Type>* node, compare_val val), compare_val value) {
		for (list_node<Type>* node = First(); node; node = node->next) {
			if (found(node, value)) {
				return node;
			}
		}
		return nullptr;
	}

	void Clear() {
		Delete();
	}

	ListIterator<Type> begin() {
		return ListIterator<Type>(this, 0);
	}

	alni end() {
		return Len();
	}

	~list() {
		Clear();
	}
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

	ListIterator(list<Type>* list, alni p_idx) {
		idx = p_idx;
		iter = list->Find(idx);
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