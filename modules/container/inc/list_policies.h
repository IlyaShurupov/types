
#pragma once

#include "poolalloc.h"
#include "new.h"

template <typename Type>
class list_node;

template <typename Type>
struct list_policy_default {

	poolalloc palloc;

	list_policy_default() : palloc(sizeof(list_node<Type>), 16) {}

	list_node<Type>* alloc_node() {
		return new (&palloc) list_node<Type>();
	}

	void free_node(list_node<Type>* node) {
		delete node;
	}

	void ValCopy(list_node<Type>* left, list_node<Type>* right) {
		left->key = right->key;
	}

	void ValDestruct(list_node<Type>* node) {
		// release
	}
};
