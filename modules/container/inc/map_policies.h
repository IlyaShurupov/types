
#pragma once

#include "heapalloc.h"
#include "poolalloc.h"

#include "new.h"

template <typename V, typename K>
struct HashNode;

template <typename V, typename K>
struct map_policy_default {

	poolalloc palloc;

	map_policy_default() : palloc(sizeof(HashNode<V, K>), 16) {}

	HashNode<V, K>** alloc_table(alni nslots) {
		HashNode<V, K>** out = new HashNode<V, K>*[nslots];
#ifndef MEM_ZEROING
		for (auto& i : range(0, nslots)) {
			out[i] = NULL;
		}
#endif // !MEM_ZEROING
		return out;
	}

	HashNode<V, K>* alloc_node() {
		return new (&palloc) HashNode<V, K>();
	}

	void free_node(HashNode<V, K>* node) {
		delete node;
	}

	void free_table(HashNode<V, K>** table) {
		return delete[] table;
	}

	alni KeyHash(const K& key) {
		return hash(key);
	}

	void ValCopy(HashNode < V, K >* left, HashNode < V, K >* right) {
		left->val = right->val;
	}

	void ValDestruct(HashNode < V, K >* node) {
		// release
	}
};