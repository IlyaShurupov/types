#include "heapalloc.h"

#include <corecrt_malloc.h>

void correct_malloc_free(void* p) {
  free(p);
}

#ifdef MEM_WRAP

#ifndef MEM_TRACE
#error "using mem_wrap without mem_trace"
#endif

#define MEMH_FROM_PTR(ptr) ((MemHead*)((int1*)ptr - WRAP_LEN) - 1)

#else
#define MEMH_FROM_PTR(ptr) ((MemHead*)ptr - 1)
#endif

void* heapalloc::alloc(alni size) {
  if (!size) {
    return nullptr;
  }

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

#ifdef MEM_TRACE
    mhptr->next = mhptr->prev = nullptr;

    mhptr->next = entry_ptr;
    if (entry_ptr) {
      entry_ptr->prev = mhptr;
    }
    entry_ptr = mhptr;
#endif

    num++;
    mhptr->alloc = this;
    mhptr->size = (alni)size;
    
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

#ifdef MEM_TRACE
  if (mhptr->next) {
    mhptr->next->prev = mhptr->prev;
  }
  if (mhptr->prev) {
    mhptr->prev->next = mhptr->next;
  }

  if (mhptr == entry_ptr) {
    entry_ptr = entry_ptr->next;
  }
#endif

#ifdef MEM_ZEROING
  memset(mhptr, mhptr->size, 0);
#endif

  num--;
  correct_malloc_free(mhptr);
}


#ifdef  MEM_TRACE
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

#ifdef MEM_WRAP
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
#endif
  return false;
}

#else
alni heapalloc::inuse_size() {
  return 0;
}

bool heapalloc::wrap_corrupted() {
  return false;
}
#endif //  MEM_TRACE

bool heapalloc::is_empty() {
  return !num;
}