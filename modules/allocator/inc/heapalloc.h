#pragma once

#include "allocator.h"

#ifdef MEM_WRAP
#define WRAP_FILL_VAL_HEAPALLOC 2
#endif

struct MemHead {
#ifdef MEM_TRACE
  MemHead* next;
  MemHead* prev;
#endif
  alni size;
  alni reserved;
  allocator* alloc;
};

class heapalloc : public allocator {
 public:
  alni num = 0;
#ifdef MEM_TRACE
  struct MemHead* entry_ptr = nullptr;
#endif

  alni reserved_size() override { return inuse_size(); }
  bool avaliable() override { return true; }

  bool wrap_support() override { return true; };
  bool wrap_corrupted() override;

  void* alloc(alni size) override;
  void free(void* p) override;

  bool is_empty() override;
  alni inuse_size() override;

  ~heapalloc() { check_err_onexit(); }
};
