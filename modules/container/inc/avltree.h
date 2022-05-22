#pragma once

#include "common.h"

#include "avl_policies.h"

namespace tp {

	template <typename Key, typename avl_policy> class AvlTree;

	template <typename Key, typename avl_policy>
	class AVLNode {

		friend class AvlTree<Key, avl_policy>;

		AVLNode* left = nullptr;
		AVLNode* right = nullptr;
		alni height = 0;

		Key key;

	};

	template <typename Key, typename avl_policy = alv_policy_default<Key> >
	class AvlTree {

		AVLNode<Key, avl_policy>* root = nullptr;
		alni entries = 0;
		avl_policy avlp;

		alni node_height(AVLNode<Key, avl_policy>* node) {
			if (!node) {
				return -1;
			}
			return node->height;
		}

		// returns new head
		AVLNode<Key, avl_policy>* node_rotate_left(AVLNode<Key, avl_policy>* pivot) {
			AVLNode<Key, avl_policy>* new_head = pivot->right;
			pivot->right = pivot->right->left;
			new_head->left = pivot;

			pivot->height = 1 + MAX(node_height(pivot->left), node_height(pivot->right));
			new_head->height = 1 + MAX(node_height(new_head->left), node_height(new_head->right));

			return new_head;
		}

		AVLNode<Key, avl_policy>* node_rotate_right(AVLNode<Key, avl_policy>* pivot) {
			AVLNode<Key, avl_policy>* new_head = pivot->left;
			pivot->left = pivot->left->right;
			new_head->right = pivot;

			pivot->height = 1 + MAX(node_height(pivot->left), node_height(pivot->right));
			new_head->height = 1 + MAX(node_height(new_head->left), node_height(new_head->right));

			return new_head;
		}

		// recursevly returns valid left or right child or root
		AVLNode<Key, avl_policy>* insert_util(AVLNode<Key, avl_policy>* head, Key key) {

			AVLNode<Key, avl_policy>* inserted_node = nullptr;

			if (head == nullptr) {
				entries++;
				AVLNode<Key, avl_policy>* out = avlp.alloc_node();
				out->key = key;
				return out;
			} else if (key == head->key) {
				return head;
			} else if (key > head->key) {
				inserted_node = insert_util(head->right, key);
				head->right = inserted_node;
			} else {
				inserted_node = insert_util(head->left, key);
				head->left = inserted_node;
			}


			// update height
			head->height = 1 + MAX(node_height(head->right), node_height(head->left));

			alni balance = node_height(head->right) - node_height(head->left);

			if (balance > 1) {
				if (key > head->right->key) {
					return node_rotate_left(head);
				} else {
					head->right = node_rotate_right(head->right);
					return node_rotate_left(head);
				}
			} else if (balance < -1) {
				if (!(key > head->left->key)) {
					return node_rotate_right(head);
				} else {
					head->left = node_rotate_left(head->left);
					return node_rotate_right(head);
				}
			}

			return head;
		}

		AVLNode<Key, avl_policy>* remove_util(AVLNode<Key, avl_policy>* head, Key key) {

			if (head == NULL) {
				return head;
			}
			if (key < head->key) {
				head->left = remove_util(head->left, key);
			} else if (key > head->key) {
				head->right = remove_util(head->right, key);
			} else {

				AVLNode<Key, avl_policy>* tmp;

				if (head->right == nullptr) {
					tmp = head->left;
					entries--;
					avlp.free_node(head);
					head = tmp;
				} else if (head->left == nullptr) {
					tmp = head->right;
					entries--;
					avlp.free_node(head);
					head = tmp;
				} else {

					AVLNode<Key, avl_policy>* min = min_node(head->right);

					avlp.ValCopy(head->key, min->key);

					head->right = remove_util(head->right, min->key);
				}
			}

			if (head == NULL) {
				return head;
			}

			head->height = 1 + MAX(node_height(head->right), node_height(head->left));
			alni balance = node_height(head->right) - node_height(head->left);

			if (balance < -1) {
				if (node_height(head->left->left) >= node_height(head->left->right)) {
					return node_rotate_right(head);
				} else {
					head->left = node_rotate_left(head->left);
					return node_rotate_right(head);
				}
			} else if (balance > 1) {
				if (node_height(head->right->right) >= node_height(head->right->left)) {
					return node_rotate_left(head);
				} else {
					head->right = node_rotate_right(head->right);
					return node_rotate_left(head);
				}
			}

			return head;
		}

		public:

		alni size() {
			return entries;
		}

		AVLNode<Key, avl_policy>* head() {
			return this->root;
		}

		void insert(Key key) {
			root = insert_util(root, key);
		}

		void remove(Key key) {
			root = remove_util(root, key);
		}

		AVLNode<Key, avl_policy>* maxNode(AVLNode<Key, avl_policy>* head) {
			if (head) {
				while (head->right != nullptr) {
					head = head->right;
				}
			}
			return head;
		}

		AVLNode<Key, avl_policy>* minNode(AVLNode<Key, avl_policy>* head) {
			if (head) {
				while (head->left != nullptr) {
					head = head->left;
				}
			}
			return head;
		}

		AVLNode<Key, avl_policy>* find(Key key) {
			AVLNode<Key, avl_policy>* iter = root;

			while (true) {
				if (!iter) {
					return nullptr;
				}
				if (iter->key == key) {
					return iter;
				}
				if (iter->key < key) {
					iter = iter->right;
					continue;
				}

				iter = iter->left;
			}

			return iter;
		}

		// returns first invalid node
		AVLNode<Key, avl_policy>* isInvalid(AVLNode<Key, avl_policy>* head) {

			if (head == nullptr) {
				return nullptr;
			}

			if (head->left) {
				if (head->left->key > head->key) {
					return head;
				}
			}
			if (head->right) {
				if (head->right->key < head->key) {
					return head;
				}
			}

			int balance = node_height(head->right) - node_height(head->left);

			if (balance > 1 || balance < -1) {
				return head;
			}

			AVLNode<Key, avl_policy>* ret = invalid_tree(head->right);
			if (ret) {
				return ret;
			}

			return invalid_tree(head->left);
		}
	};
};