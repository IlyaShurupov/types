
#pragma once

#include "Macros.h"

/*
template <int S_SLOT, int N_ENTRIES>
struct MemChunkAlloc {

  alni buff[N_ENTRIES];
  void* first = &buff;
  alni capacity = N_ENTRIES;

  MemChunkAlloc() {
    for (alni* block_iter = &buff; i < N_ENTRIES; p += S_SLOT) {
      *block_iter = &block_iter[1];
    }
  }

  ~MemChunkAlloc() {
  }

  void* alloc(uint1 size) { 
    if (!freeblocks) {
      return 0;
    }
    uint1* pResult = buff + (first * blockSize);
    first = *pResult;
    freeblocks--;
    return pResult;
  }

  void free(void* ptr, int blockSize) {
    uint1* delptr = (uint1*)(ptr);
    *delptr = first;
    first = (uint1)((delptr - buff) / blockSize);
    ++freeblocks; 
  }
};
*/