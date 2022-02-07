#pragma once

#include "poolalloc.h"
#include "new.h"

#define FOREACH_STACK(type, stack, iter) \
  for (StackNode<type>* iter = stack->last; iter; iter = iter->prev)

template <typename Type>
class StackNode {
 public:
  StackNode<Type>* prev;
  Type data;

 public:
  StackNode(Type data, StackNode<Type>* prev) {
    this->prev = prev;
    this->data = data;
  }
  ~StackNode() {
  }
};

template <typename Type>
class stack {
 public:
  
  alni length;
  StackNode<Type>* last;

  poolalloc palloc;

  stack(alni palloc_chunk_size = 16) : palloc(sizeof(StackNode<Type>), palloc_chunk_size) {
    length = 0;
    last = nullptr;
  }

  ~stack() {
    free();
  }

  void push(Type data) {
    StackNode<Type>* NewNode = new(&palloc) StackNode<Type>(data, last);
    last = NewNode;
    length++;
  }

  void pop() {
    StackNode<Type>* del = last;
    last = last->prev;
    mfree(&palloc, del);
    length--;
  }

  void free() {
    StackNode<Type>* del = last;
    for (alni i = 0; i < length; i++) {
      StackNode<Type>* prev = del->prev;
      mfree(&palloc, del);
      del = prev;
    }
  }
};