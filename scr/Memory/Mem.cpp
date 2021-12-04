
#include "Memory/Mem.h"


#include "Time/Timer.h"
#include "Strings/Strings.h"

#include <corecrt_malloc.h>
#include <stdlib.h>

#ifdef MEM_DEBUG_WRAP
#include <cassert>
typedef char int1;
#define WRAP_LEN 8  // bytes
#define WRAP_FILL_VAL 1  // bytes
#endif

MemHead* mem_debug_entry_ptr;
uint8 num = 0;

#undef new

void* operator new(size_t size, const char* file, int line) {

#ifdef MEM_DEBUG_WRAP

  int total = size + sizeof(MemHead) + WRAP_LEN * 2;
  MemHead* mhptr = (MemHead*)malloc(total);

  if (mhptr) {

    for (alloc_size i = 0; i < total; i++) {
      ((int1*)mhptr)[i] = WRAP_FILL_VAL;
    }

#else

  MemHead* mhptr = (MemHead*)malloc(size + sizeof(MemHead));

  if (mhptr) {
#endif

    mhptr->next = mhptr->prev = nullptr;

    if (mem_debug_entry_ptr) {
      mem_debug_entry_ptr->next = mhptr;
    }

    mhptr->prev = mem_debug_entry_ptr;
    mem_debug_entry_ptr = mhptr;

    num++;

    mhptr->line = line;
    //mhptr->file = file;
    mhptr->size = (uint8)size;
    mhptr->type = nullptr;
    mhptr->time = (uint8)get_time();

#ifdef MEM_DEBUG_WRAP
    return (void*)((int1*)((MemHead*)mhptr + 1) + WRAP_LEN);
#else
    return (void*)((MemHead*)mhptr + 1);
#endif
  }

  return nullptr;
}

void operator delete (void* ptr) {
  if (!ptr) {
    return;
  }

#ifdef MEM_DEBUG_WRAP

  MemHead* mhptr = ((MemHead*)((int1*)ptr - WRAP_LEN) - 1);

  void* wrap1 = (void*)((MemHead*)mhptr + 1);
  void* wrap2 = (void*)((int1*)((MemHead*)mhptr + 1) + WRAP_LEN + mhptr->size);

  for (alloc_size i = 0; i < WRAP_LEN; i++) {
    int1 val1 = ((int1*)wrap1)[i];
    int1 val2 = ((int1*)wrap2)[i];
    assert(val1 == WRAP_FILL_VAL && val2 == WRAP_FILL_VAL);
  }

#else
  MemHead* mhptr = ((MemHead*)ptr - 1);
#endif

  if (mhptr->prev && mhptr->next) {
    mhptr->next->prev = mhptr->prev;
    mhptr->prev->next = mhptr->next;

  }
  else if (mhptr->prev || mhptr->next) {

    if (mhptr->next) {
      mhptr->next->prev = nullptr;
    }
    else {
      mhptr->prev->next = nullptr;
      mem_debug_entry_ptr = mhptr->prev;
    }
  }

  //delete mhptr->file;
  //delete mhptr->type;

  num--;
  free(mhptr);
}

void operator delete(void* p, const char* file, int line) {
  delete p;
}

uint8 mem_allocated_size() {
  uint8 size = 0;
  MemHead* alloc_iter = mem_debug_entry_ptr;
  while (alloc_iter) {
    size += alloc_iter->size;
    alloc_iter = alloc_iter->prev;
  }
  return size;
}


bool init_mem_debug() {
  return true;
}


void mem_test_types() {
  int size = 0;
  size = sizeof(int1);
  size = sizeof(int2);
  size = sizeof(int4);
  size = sizeof(int8);
  size = sizeof(alnf);
  size = sizeof(alni);
}