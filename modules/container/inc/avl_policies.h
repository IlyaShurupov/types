
#pragma once

#include "poolalloc.h"
#include "new.h"

template <typename K>
struct AVLNode;

template <typename V>
struct alv_policy_default {

	poolalloc palloc;

	map_policy_default() : palloc(sizeof(AVLNode<V>), 16) {}

	AVLNode<V>* alloc_node() {
		return new (&palloc) AVLNode<V>();
	}

	void free_node(AVLNode<V>* node) {
		delete node;
	}

	void ValCopy(AVLNode<V>* left, AVLNode<V>* right) {
		left->key = right->key;
	}

	void ValDestruct(AVLNode<V>* node) {
		// release
	}
};
