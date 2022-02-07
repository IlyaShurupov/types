# pragma once

#include "heapalloc.h"

#include "new.h"

template <typename Type>
struct array_iterator;

 extern heapalloc array_buuf_allocator;

template <typename Type>
class array {
  
  Type* buff;
  alni length;


 public:

  array() { 
    length = 0;
    buff = nullptr;
  }

  array(alni p_length) { 
    length = p_length;
    Reserve(length);
  }
  
  alni Len() {
    return length;
  }

  void Reserve(alni p_bufflen) {
    length = p_bufflen;
    buff = (Type*)array_buuf_allocator.alloc(sizeof(Type) *length);
  }

  void Free() {
    length = 0;
    mfree(&array_buuf_allocator, buff);
    buff = nullptr;
  }

  void Insert(Type& p_block, alni idx) {
    Type* current = buff;
    Reserve(length + 1);

    for (alni befor = 0; befor < idx; befor++) {
      buff[befor] = current[befor];
    }
    for (alni after = idx; after < length - 1; after++) {
      buff[after + 1] = current[after];
    }

    buff[idx] = p_block;

    if (current) {
      mfree(&array_buuf_allocator, current);
    }
  }

  void Remove(alni p_idx) {
    Type* current = buff;
    Reserve(length - 1);

    for (alni befor = 0; befor < p_idx; befor++) {
      buff[befor] = current[befor];
    }
    for (alni after = p_idx; after < length + 1; after++) {
      buff[after - 1] = current[after];
    }

    mfree(&array_buuf_allocator, current);
  }

  void Remove(Type val) {
    for (int i = 0; i < Len(); i++) {
      if (buff[i] == val) {
        Remove(i);
      }
    }
  }

  Type& operator[](alni idx) {
    return buff[idx];
  }

  void PushBack(Type block) {
    Insert(block, length);
  }

  array(Type b1) { 
    Reserve(1); buff[0] = b1;
  }

  Type* GetBuff() {
    return buff;
  }

  array(const array& array) {
    Reserve(array.length);
    for (int i = 0; i < array.length; i++) {
      buff[i] = array.buff[i];
    }
  }

  array_iterator<Type> begin() { return array_iterator<Type>(this); }
  alni end() { return Len(); }

  ~array() {
    Free();
  }

};

template <typename Type>
struct array_iterator {
  alni idx = 0;
  array<Type>* array_p;
  
  array_iterator(array<Type>* array) : array(array) {}

  Type* operator->() { return &(*array_p)[idx]; }

  Type& data() { return (*array_p)[idx]; }

  inline void operator++() {
    idx++;
  }

  bool operator==(alni p_idx) { return idx == p_idx; }
  bool operator!=(alni p_idx) { return idx != p_idx; }
  bool operator>(alni p_idx) { return idx > p_idx; }
  bool operator<(alni p_idx) { return idx < p_idx; }
  bool operator>=(alni p_idx) { return idx >= p_idx; }
  bool operator<=(alni p_idx) { return idx <= p_idx; }

  const array_iterator& operator*() { return *this; }
};