#pragma once

#include "heapalloc.h"

template <typename Type>
struct array_iterator;

template <typename Type>
class Array {
  
 public:
  
  Type* buff;
  alni length;

  Array() { 
    length = 0;
    buff = nullptr;
  }

  Array(alni p_length) { 
    length = p_length;
    Reserve(length);
  }
  
  alni Len() {
    return length;
  }

  void Reserve(alni p_bufflen) {
    length = p_bufflen;
    buff = new Type[length]();
  }

  void Free() {
    length = 0;
    delete[] buff;
    buff = NULL;
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
      delete[] current;
    }
  }

  void Remove(alni p_idx) {
    Type* current = buff;
    alni current_len = length;
    Reserve(length - 1);

    for (alni befor = 0; befor < p_idx; befor++) {
      buff[befor] = current[befor];
    }
    for (alni after = p_idx + 1; after < length + 1; after++) {
      buff[after - 1] = current[after];
    }

    delete[] current;
  }

  void RemoveVal(Type val) {
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

  Type* GetBuff() {
    return buff;
  }

  Array(const Array& array) {
    Reserve(array.length);
    for (int i = 0; i < array.length; i++) {
      buff[i] = array.buff[i];
    }
  }

  array_iterator<Type> begin() { return array_iterator<Type>(this); }
  alni end() { return Len(); }

  ~Array() {
    Free();
  }

};

template <typename Type>
struct array_iterator {
  alni idx = 0;
  Array<Type>* array_p;
  
  array_iterator(Array<Type>* array) : array_p(array) {}

  Type* operator->() { return &(*array_p)[idx]; }

  Type& data() const { return (*array_p)[idx]; }

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