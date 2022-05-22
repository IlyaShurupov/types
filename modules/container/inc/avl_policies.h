
#pragma once

#include "poolalloc.h"
#include "new.h"

namespace tp {

	template <typename K, typename avl_policy>
	class AVLNode;

	template <typename Key>
	struct alv_policy_default {

		PoolAlloc palloc;

		alv_policy_default() : palloc(sizeof(AVLNode<Key, alv_policy_default>), 16) {}

		AVLNode<Key, alv_policy_default>* alloc_node() {
			return new (&palloc) AVLNode<Key, alv_policy_default>();
		}

		void free_node(AVLNode<Key, alv_policy_default>* node) {
			delete node;
		}

		void ValCopy(AVLNode<Key, alv_policy_default>* left, AVLNode<Key, alv_policy_default>* right) {
			left->key = right->key;
		}

		void ValDestruct(AVLNode<Key, alv_policy_default>* node) {
			// release
		}
	};

};
