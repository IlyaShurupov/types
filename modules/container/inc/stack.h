#pragma once

#include "allocators.h"

namespace tp {

	template <typename Type>
	class Stack {
		public:

		template <typename Type>
		class Node {
			public:
			Node<Type>* below;
			Type data;

			public:
			Node(Type data, Node<Type>* below) {
				this->below = below;
				this->data = data;
			}
		};

		template <typename Type>
		struct Iterator {
			Node<Type>* mIter;
			Type& operator->() { return mIter->data; }
			const Iterator& operator*() { return *this; }
			inline void operator++() { mIter = mIter->below; }
			bool operator==(alni end) { return mIter == NULL; }
		};

		alni length;
		Node<Type>* top;

		PoolAlloc palloc;

		Stack(alni palloc_chunk_size = 16) : palloc(sizeof(Node<Type>), palloc_chunk_size) {
			length = 0;
			top = nullptr;
		}

		~Stack() {
			free();
		}

		// pushes the node on top of the stack
		void push(Type data) {
			Node<Type>* NewNode = new (palloc) Node<Type>(data, top);
			top = NewNode;
			length++;
		}

		// pops the top node of the stack
		void pop() {
			assert(top);
			Node<Type>* del = top;
			top = top->below;
			delete del;
			length--;
		}

		// deletes all nodes
		void free() {
			Node<Type>* del = top;
			for (alni i = 0; i < length; i++) {
				Node<Type>* below = del->below;
				delete del;
				del = below;
			}
		}

		alni sizeAllocatedMem() {
			alni out = 0;
			out += sizeof(Node<Type>*);
			out += sizeof(alni);
			out += palloc.sizeAllocatedMem();
			return out;
		}

		alni sizeUsedMem() {
			alni out = 0;
			out += sizeof(Node<Type>*);
			out += sizeof(alni);
			out += palloc.sizeUsedMem();
			return out;
		}

		Iterator<Type> begin() const {
			return {top};
		}

		alni end() const {
			return NULL;
		}
	};
};