#pragma once

#include "allocators.h"


#define FOREACH_STACK(type, stack, iter) \
  for (stack_node<type>* iter = stack->last; iter; iter = iter->prev)

namespace tp {

	template <typename Type>
	class Stack {
		public:

		template <typename Type>
		class Node {
			public:
			Node<Type>* prev;
			Type data;

			public:
			Node(Type data, Node<Type>* prev) {
				this->prev = prev;
				this->data = data;
			}
		};

		alni length;
		Node<Type>* last;

		PoolAlloc palloc;

		Stack(alni palloc_chunk_size = 16) : palloc(sizeof(Node<Type>), palloc_chunk_size) {
			length = 0;
			last = nullptr;
		}

		~Stack() {
			free();
		}

		void push(Type data) {
			Node<Type>* NewNode = new (palloc) Node<Type>(data, last);
			last = NewNode;
			length++;
		}

		void pop() {
			Node<Type>* del = last;
			last = last->prev;
			delete del;
			length--;
		}

		void free() {
			Node<Type>* del = last;
			for (alni i = 0; i < length; i++) {
				Node<Type>* prev = del->prev;
				delete del;
				del = prev;
			}
		}
	};
};