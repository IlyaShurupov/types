
#pragma once

#include "Macros.h"

/*
struct AllocatorChunck {

  uint1* buff;
  uint1 first;
  uint1 freeblocks;
  uint1 blockSize;

  AllocatorChunck(uint1 p_blockSize, uint1 blocks) { 
    buff = (uint1*)malloc(p_blockSize * blocks);
    first = 0;
    freeblocks = blocks;
    blockSize = p_blockSize;

    uint1 i = 0;
    uint1* p = buff;
    for (; i != blocks; p += blockSize) {
      *p = ++i;
    }
  }

  ~AllocatorChunck() {
    free(buff); 
  }

  void* Get(uint1 size) { 
    if (!freeblocks) {
      return 0;
    }
    uint1* pResult = buff + (first * blockSize);
    first = *pResult;
    freeblocks--;
    return pResult;
  }

  void Deallocate(void* ptr, int blockSize) {
    uint1* delptr = (uint1*)(ptr);
    *delptr = first;
    first = (uint1)((delptr - buff) / blockSize);
    ++freeblocks; 
  }
};
*/