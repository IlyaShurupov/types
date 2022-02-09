#include "heapalloc.h"

#include <corecrt_malloc.h>

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

void* heapalloc::alloc(alni size, const char* file, int line) {

#ifdef MEM_WRAP

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

    mhptr->next = entry_ptr;
    if (entry_ptr) {
      entry_ptr->prev = mhptr;
    }
    entry_ptr = mhptr;

    num++;

    mhptr->line = line;
    mhptr->file = file;
    mhptr->size = (alni)size;
    mhptr->type = nullptr;

    
#ifdef MEM_WRAP
    void* out = (void*)((int1*)((MemHead*)mhptr + 1) + WRAP_LEN);
#else
    void* out = (void*)((MemHead*)mhptr + 1);
#endif
    #ifdef MEM_ZEROING
    memset(out, size, 0);
    #endif

    return out;
  }

  return nullptr;
}

void heapalloc::free(void* ptr) {
  if (!ptr) {
    return;
  }

#ifdef MEM_WRAP

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

  if (mhptr->next) {
    mhptr->next->prev = mhptr->prev;
  }
  if (mhptr->prev) {
    mhptr->prev->next = mhptr->next;
  }

  if (mhptr == entry_ptr) {
    entry_ptr = entry_ptr->next;
  }

  num--;
  correct_malloc_free(mhptr);
}

alni heapalloc::inuse_size() {
  alni size = 0;
  MemHead* alloc_iter = entry_ptr;
  while (alloc_iter) {
    size += alloc_iter->size;
    alloc_iter = alloc_iter->next;
  }
  return size;
}

bool heapalloc::wrap_corrupted() {

  for (MemHead* mhptr = entry_ptr; mhptr; mhptr = mhptr->next) {

    void* wrap1 = (void*)((MemHead*)mhptr + 1);
    void* wrap2 = (void*)((int1*)((MemHead*)mhptr + 1) + WRAP_LEN + mhptr->size);

    for (alni i = 0; i < WRAP_LEN; i++) {
      int1 val1 = ((int1*)wrap1)[i];
      int1 val2 = ((int1*)wrap2)[i];

      if (val1 != WRAP_FILL_VAL || val2 != WRAP_FILL_VAL) {
        return true;
      }
    }
  }
  return false;
}

#else
void* heapalloc::alloc(alni size) {
  void* out = malloc(size);
  if (!out) {
    throw typesExeption("failed allocate on heap", false);
  }

  #ifdef MEM_ZEROING
  memset(out, size, 0);
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

bool heapalloc::wrap_corrupted() {
  return false;
}