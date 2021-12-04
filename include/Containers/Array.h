# pragma once

#include "Memory/Mem.h"
#include "Macros.h"

template <typename Type>
class Array {
  
  Type* buff;
  uint4 length;

 public:

  Array() { 
    length = 0;
    buff = nullptr;
  }

  Array(uint4 p_length) { 
    length = p_length;
    Reserve(length);
  }
  
  uint4 Len() {
    return length;
  }

  void Reserve(uint4 p_bufflen) {
    length = p_bufflen;
    buff = new Type[length]; 
  }

  void Free() {
    length = 0;
    delete buff;
    buff = nullptr;
  }

  void Insert(Type& p_block, uint4 idx) {
    Type* current = buff;
    Reserve(length + 1);

    for (uint4 befor = 0; befor < idx; befor++) {
      buff[befor] = current[befor];
    }
    for (uint4 after = idx; after < length - 1; after++) {
      buff[after + 1] = current[after];
    }

    buff[idx] = p_block;

    if (current) {
      delete current;
    }
  }

  void Remove(uint4 p_idx) {
    Type* current = buff;
    Reserve(length - 1);

    for (uint4 befor = 0; befor < p_idx; befor++) {
      buff[befor] = current[befor];
    }
    for (uint4 after = p_idx; after < length + 1; after++) {
      buff[after - 1] = current[after];
    }

    delete current;
  }

  void Remove(Type val) {
    for (int i = 0; i < Len(); i++) {
      if (buff[i] == val) {
        Remove(i);
      }
    }
  }

  Type& operator[](uint4 idx) {
    return buff[idx];
  }

  void PushBack(Type block) {
    Insert(block, length);
  }

  Array(Type b1) { 
    Reserve(1); buff[0] = b1;
  }

  Array(Type b1, Type b2) { 
    Reserve(2); buff[0] = b1; buff[1] = b2;
  }

  Array(Type b1, Type b2, Type b3) { 
    Reserve(3); buff[0] = b1; buff[1] = b2; buff[2] = b3; 
  }

  Array(Type b1, Type b2, Type b3, Type b4) { 
    Reserve(4); buff[0] = b1; buff[1] = b2; buff[2] = b3; buff[3] = b4;
  }

  Array(Type b1, Type b2, Type b3, Type b4, Type b5) { 
    Reserve(5); buff[0] = b1; buff[1] = b2; buff[2] = b3; buff[3] = b4; buff[4] = b5;
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

  ~Array() {
    Free();
  }
};