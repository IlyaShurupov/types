#pragma once

#include "common.h"
#include "allocators.h"


namespace tp {

	template <typename Key> class AvlTree;

	template <typename Key>
	class AVLNode {

		friend AvlTree<Key>;

		public:

		AVLNode* left = nullptr;
		AVLNode* right = nullptr;
		alni height = 0;

		Key key;

	};

	template <typename Key>
	class AvlTree {

		PoolAlloc palloc;

		AVLNode<Key>* root = nullptr;
		alni entries = 0;

		alni node_height(AVLNode<Key>* node) {
			if (!node) {
				return -1;
			}
			return node->height;
		}

		// returns new head
		AVLNode<Key>* node_rotate_left(AVLNode<Key>* pivot) {
			AVLNode<Key>* new_head = pivot->right;
			pivot->right = pivot->right->left;
			new_head->left = pivot;

			pivot->height = 1 + MAX(node_height(pivot->left), node_height(pivot->right));
			new_head->height = 1 + MAX(node_height(new_head->left), node_height(new_head->right));

			return new_head;
		}

		AVLNode<Key>* node_rotate_right(AVLNode<Key>* pivot) {
			AVLNode<Key>* new_head = pivot->left;
			pivot->left = pivot->left->right;
			new_head->right = pivot;

			pivot->height = 1 + MAX(node_height(pivot->left), node_height(pivot->right));
			new_head->height = 1 + MAX(node_height(new_head->left), node_height(new_head->right));

			return new_head;
		}

		// recursevly returns valid left or right child or root
		AVLNode<Key>* insert_util(AVLNode<Key>* head, Key key) {

			AVLNode<Key>* inserted_node = nullptr;

			if (head == nullptr) {
				entries++;
				AVLNode<Key>* out = new (palloc) AVLNode<Key>();
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

		AVLNode<Key>* remove_util(AVLNode<Key>* head, Key key) {

			if (head == NULL) {
				return head;
			}
			if (key < head->key) {
				head->left = remove_util(head->left, key);
			} else if (key > head->key) {
				head->right = remove_util(head->right, key);
			} else {

				AVLNode<Key>* tmp;

				if (head->right == nullptr) {
					tmp = head->left;
					entries--;
					delete head;
					head = tmp;
				} else if (head->left == nullptr) {
					tmp = head->right;
					entries--;
					delete head;
					head = tmp;
				} else {

					AVLNode<Key>* min = minNode(head->right);

					head->key = min->key;

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

		AvlTree(uint2 palloc_chunk_size = 30) : palloc(sizeof(AVLNode<Key>), palloc_chunk_size) {}

		alni size() {
			return entries;
		}

		AVLNode<Key>* head() {
			return this->root;
		}

		void insert(Key key) {
			root = insert_util(root, key);
		}

		void remove(Key key) {
			root = remove_util(root, key);
		}

		AVLNode<Key>* maxNode(AVLNode<Key>* head) {
			if (head) {
				while (head->right != nullptr) {
					head = head->right;
				}
			}
			return head;
		}

		AVLNode<Key>* minNode(AVLNode<Key>* head) {
			if (head) {
				while (head->left != nullptr) {
					head = head->left;
				}
			}
			return head;
		}

		AVLNode<Key>* find(Key key) {
			AVLNode<Key>* iter = root;

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
		AVLNode<Key>* isInvalid(AVLNode<Key>* head) {

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

			AVLNode<Key>* ret = isInvalid(head->right);
			if (ret) {
				return ret;
			}

			return isInvalid(head->left);
		}


		alni sizeAllocatedMem() {
			alni out = 0;
			out += sizeof(AVLNode<Key>*);
			out += sizeof(alni);
			out += palloc.sizeAllocatedMem();
			return out;
		}

		alni sizeUsedMem() {
			alni out = 0;
			out += sizeof(AVLNode<Key>*);
			out += sizeof(alni);
			out += palloc.sizeUsedMem();
			return out;
		}
	};
};