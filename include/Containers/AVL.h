#pragma once

#include "Macros.h"

template <typename Key> class AVL;

template <typename Key>
class AVLNode {

	friend class AVL<Key>;

	AVLNode* left = nullptr;
	AVLNode* right = nullptr;
	alni height = 0;

	Key key;

	AVLNode(const Key& key) {
		this->key = key;
	}
};

template <typename Key>
class AVL {

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

		pivot->height = 1 + max(node_height(pivot->left), node_height(pivot->right));
		new_head->height = 1 + max(node_height(new_head->left), node_height(new_head->right));

		return new_head;
	}

	AVLNode<Key>* node_rotate_right(AVLNode<Key>* pivot) {
		AVLNode<Key>* new_head = pivot->left;
		pivot->left = pivot->left->right;
		new_head->right = pivot;

		pivot->height = 1 + max(node_height(pivot->left), node_height(pivot->right));
		new_head->height = 1 + max(node_height(new_head->left), node_height(new_head->right));

		return new_head;
	}

	// recursevly returns valid left or right child or root
	AVLNode<Key>* insert_util(AVLNode<Key>* head, Key key) {

		AVLNode<Key>* inserted_node = nullptr;

		if (head == nullptr) {
			entries++;
			return new AVLNode<Key>(key);
		}
		else if (key == head->key) {
			return head;
		}
		else if (key > head->key) {
			inserted_node = insert_util(head->right, key);
			head->right = inserted_node;
		}
		else {
			inserted_node = insert_util(head->left, key);
			head->left = inserted_node;
		}


		// update height
		head->height = 1 + max(node_height(head->right), node_height(head->left));

		alni balance = node_height(head->right) - node_height(head->left);

		if (balance > 1) {
			if (key > head->right->key) {
				return node_rotate_left(head);
			}
			else {
				head->right = node_rotate_right(head->right);
				return node_rotate_left(head);
			}
		}
		else if (balance < -1) {
			if (key < head->left->key) {
				return node_rotate_right(head);
			}
			else {
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
		}
		else if (key > head->key) {
			head->right = remove_util(head->right, key);
		}
		else {

			AVLNode<Key>* tmp;

			if (head->right == nullptr) {
				tmp = head->left;
				entries--;
				delete head;
				head = tmp;
			}
			else if (head->left == nullptr) {
				tmp = head->right;
				entries--;
				delete head;
				head = tmp;
			}
			else {
				
				AVLNode<Key>* min = min_node(head->right);
				
				head->key = min->key;

				head->right = remove_util(head->right, min->key);
			}
		}

		if (head == NULL) { 
			return head; 
		}

		head->height = 1 + max(node_height(head->right), node_height(head->left));
		alni balance = node_height(head->right) - node_height(head->left);

		if (balance < -1) {
			if (node_height(head->left->left) >= node_height(head->left->right)) {
				return node_rotate_right(head);
			}
			else {
				head->left = node_rotate_left(head->left);
				return node_rotate_right(head);
			}
		}
		else if (balance > 1) {
			if (node_height(head->right->right) >= node_height(head->right->left)) {
				return node_rotate_left(head);
			}
			else {
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

	AVLNode<Key>* head() {
		return this->root;
	}

	void insert(Key key) {
		root = insert_util(root, key);
	}

	void remove(Key key) {
		root = remove_util(root, key);
	}

	AVLNode<Key>* max_node(AVLNode<Key>* head) {
		if (head) {
			while (head->right != nullptr) {
				head = head->right;
			}
		}
		return head;
	}

	AVLNode<Key>* min_node(AVLNode<Key>* head) {
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
	AVLNode<Key>* invalid_tree(AVLNode<Key>* head) {

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

		AVLNode<Key>* ret = invalid_tree(head->right);
		if (ret) {
			return ret;
		}

		return invalid_tree(head->left);
	}

};


