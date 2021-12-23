#pragma once

#include "Memory/Allocators.h"
#include "Sort.h"
#include "Macros.h"


#include <cstdarg>

template <class Type> class ListIterator;
template <class Type> class List;

template <typename Type>
class Node {
public:
	Type data;
	Node<Type>* next = nullptr;
	Node<Type>* prev = nullptr;

	Node(Type p_data) { data = p_data; }

	Type operator->() { return data; }

	void FreeData() {
		delete data;
	}
};

template <typename Type>
class List {

	Node<Type>* first = nullptr;
	Node<Type>* last = nullptr;
	alni length = 0;

public:
	bool recursive_free_on_destruction = true;
	bool shared_nodes = false;

public:
	List() {}
	List(bool recursive_free_on_destruction) : recursive_free_on_destruction(recursive_free_on_destruction) {}

	inline Node<Type>* First() { return first; }
	inline Node<Type>* Last() { return last; }

	inline const Node<Type>* First() const { return first; }
	inline const Node<Type>* Last() const { return last; }
	
	inline alni Len() { return length; }

	void Attach(Node<Type>* node, Node<Type>* node_to) {
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

	void Detach(Node<Type>* node) {
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

	Node<Type>* Find(alni idx) {
		if (!First() || idx < 0 || idx > Len() - 1) {
			return nullptr;
		}
		Node<Type>* found = First();
		for (int i = 0; i != idx; i++) {
			found = found->next;
		}
		return found;
	}

	Node<Type>* Find(Type data) {
		Node<Type>* found = First();
		for (alni i = 0; data != found->data; i++) {
			if (!found->next) {
				return nullptr;
			}
			found = found->next;
		}
		return found;
	}

	void ForEach(void (*functor)(List<Type>* list, Node<Type>* node)) {
		Node<Type>* node = First();
		while (node) {
			Node<Type>* next = node->next;
			functor(this, node);
			node = next;
		}
	}

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

	inline Type& operator[](ListIterator<Type>& iter) { return iter.node()->data; }
	inline Type& operator[](alni idx) { return Find(idx)->data; }

	void PushBack(Node<Type>* new_node) { Attach(new_node, Last()); }
	void PushFront(Node<Type>* new_node) { Attach(new_node, nullptr); }

	void PushBack(Type data) {
		PushBack(new Node<Type>(data));
	}
	void PushFront(Type data) {
		PushFront(new Node<Type>(data));
	}

	void Insert(Node<Type>* node, alni idx) {
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
		Insert(new Node<Type>(data), idx);
	}

	void DelNode(Node<Type>* node) {
		Detach(node);
		node->FreeData();
		delete node;
	}

	void Release() {
		if (shared_nodes) {
			ForEach([](List<Type>* list, Node<Type>* node) { list->Detach(node); });
		}
		else {
			ForEach([](List<Type>* list, Node<Type>* node) { Node<Type>* del_node = node;  list->Detach(node); delete del_node; });
		}
		length = 0;
		first = last = nullptr;
	}

	void Delete() {
		ForEach([](List<Type>* list, Node<Type>* node) { list->DelNode(node); });
		length = 0;
		first = last = nullptr;
	}

	List<Type>& operator += (const List<Type>& in) {
		for (Node<Type>* node = in.first; node; node = node->next) {
			PushBack(node->data);
		}
		return *this;
	}

	List<Type>& operator = (const List<Type>& in) {

		Clear();

		*this += in;

		recursive_free_on_destruction = in.recursive_free_on_destruction;

		return *this;
	}

	template <typename compare_val>
	Node<Type>* Find(bool (*found)(Node<Type>* node, compare_val val), compare_val value) {
		for (Node<Type>* node = First(); node; node = node->next) {
			if (found(node, value)) {
				return node;
			}
		}
		return nullptr;
	}

	void Clear() {
		if (recursive_free_on_destruction) {
			Delete();
		}
		else {
			Release();
		}
	}

	ListIterator<Type> begin() {
		return ListIterator<Type>(this, 0);
	}

	alni end() {
		return Len();
	}

	~List() {
		Clear();
	}
};

template <typename Type>
class ListIterator {

	Node<Type>* iter;
	alni idx;

public:
	alni Idx() { return idx; }
	Type& operator->() { return iter->data; }
	Type& Data() { return iter->data; }
	Node<Type>* node() { return iter; }

	ListIterator(List<Type>* list, alni p_idx) {
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