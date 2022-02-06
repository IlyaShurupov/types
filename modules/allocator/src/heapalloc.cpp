#include "heapalloc.h"

#include "heapalloc.h"

#include <string.h>
#include <corecrt_malloc.h>
#include <cassert>

void correct_malloc_free(void* p) {
  free(p);
}

struct MemHead {
  MemHead* next;
  MemHead* prev;
  alni size;
  const char* type;
  const char* file;
  alni line;
  alni reserved;
};

#ifdef MEM_WRAP

#ifndef MEM_TRACE
#error "using mem_wrap without mem_trace"
#endif

#define MEMH_FROM_PTR(ptr) ((MemHead*)((int1*)ptr - WRAP_LEN) - 1)

#else
#define MEMH_FROM_PTR(ptr) ((MemHead*)ptr - 1)
#endif

#ifdef MEM_TRACE

void* heap_alloc::alloc(alni size, const char* file, int line) {

#ifdef MEM_DEBUG_WRAP

  alni total = (alni)size + sizeof(MemHead) + WRAP_LEN * 2;
  MemHead* mhptr = (MemHead*)malloc(total);

  if (mhptr) {

    for (alni i = 0; i < total; i++) {
      ((int1*)mhptr)[i] = WRAP_FILL_VAL;
    }

#else

  MemHead* mhptr = (MemHead*)malloc(size + sizeof(MemHead));

  if (mhptr) {
#endif

    mhptr->next = mhptr->prev = nullptr;

    if (entry_ptr) {
      entry_ptr->next = mhptr;
    }

    mhptr->prev = entry_ptr;
    entry_ptr = mhptr;

    num++;

    mhptr->line = line;
    mhptr->file = file;
    mhptr->size = (alni)size;
    mhptr->type = nullptr;

    
#ifdef MEM_DEBUG_WRAP
    void* out = (void*)((int1*)((MemHead*)mhptr + 1) + WRAP_LEN);
#else
    void* out = (void*)((MemHead*)mhptr + 1);
#endif
    #ifdef MEM_ZEROING
    memset(out, 0, size);
    #endif

    return out;
  }

  return nullptr;
}

void heap_alloc::free(void* ptr) {
  if (!ptr) {
    return;
  }

#ifdef MEM_DEBUG_WRAP

  MemHead* mhptr = ((MemHead*)((int1*)ptr - WRAP_LEN) - 1);

  void* wrap1 = (void*)((MemHead*)mhptr + 1);
  void* wrap2 = (void*)((int1*)((MemHead*)mhptr + 1) + WRAP_LEN + mhptr->size);

  for (alni i = 0; i < WRAP_LEN; i++) {
    int1 val1 = ((int1*)wrap1)[i];
    int1 val2 = ((int1*)wrap2)[i];
    assert((val1 == WRAP_FILL_VAL && val2 == WRAP_FILL_VAL));
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
      entry_ptr = mhptr->prev;
    }
  }

  num--;
  correct_malloc_free(mhptr);
}

alni heap_alloc::use_size() {
  alni size = 0;
  MemHead* alloc_iter = entry_ptr;
  while (alloc_iter) {
    size += alloc_iter->size;
    alloc_iter = alloc_iter->prev;
  }
  return size;
}

#else
void* heapalloc::alloc(alni size) {
  void* out = malloc(size);
  if (!out) {
    throw "failed alloc";
  }

  #ifdef MEM_ZEROING
  memset(out, 0, size);
  #endif

  return out;
}

void heapalloc::free(void* ptr) {
  correct_malloc_free(ptr);
}

alni heapalloc::inuse_size() {
  return 0;
}
#endif