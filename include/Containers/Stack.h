#pragma once

#include "Memory/Mem.h"

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
class Stack {
 public:
  
  alni length;
  StackNode<Type>* last;

  Stack() {
    length = 0;
    last = nullptr;
  }

  ~Stack() {
    free();
  }

  void push(Type data) {
    StackNode<Type>* NewNode = new StackNode<Type>(data, last);
    last = NewNode;
    length++;
  }

  void pop() {
    StackNode<Type>* del = last;
    last = last->prev;
    delete del;
    length--;
  }

  void free() {
    StackNode<Type>* del = last;
    for (alni i = 0; i < length; i++) {
      StackNode<Type>* prev = del->prev;
      delete del;
      del = prev;
    }
  }
};