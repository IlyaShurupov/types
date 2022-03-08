#pragma once

#include "poolalloc.h"
#include "new.h"

#define FOREACH_STACK(type, stack, iter) \
  for (stack_node<type>* iter = stack->last; iter; iter = iter->prev)

template <typename Type>
class stack_node {
 public:
  stack_node<Type>* prev;
  Type data;

 public:
  stack_node(Type data, stack_node<Type>* prev) {
    this->prev = prev;
    this->data = data;
  }
  ~stack_node() {
  }
};

template <typename Type>
class stack {
 public:
  
  alni length;
  stack_node<Type>* last;

  poolalloc palloc;

  stack(alni palloc_chunk_size = 16) : palloc(sizeof(stack_node<Type>), palloc_chunk_size) {
    length = 0;
    last = nullptr;
  }

  ~stack() {
    free();
  }

  void push(Type data) {
    stack_node<Type>* NewNode = new(&palloc) stack_node<Type>(data, last);
    last = NewNode;
    length++;
  }

  void pop() {
    stack_node<Type>* del = last;
    last = last->prev;
    delete del;
    length--;
  }

  void free() {
    stack_node<Type>* del = last;
    for (alni i = 0; i < length; i++) {
      stack_node<Type>* prev = del->prev;
      delete del;
      del = prev;
    }
  }
};