#pragma once

#include "allocator.h"

#ifdef MEM_WRAP
#define WRAP_FILL_VAL_CHUNK 4
#if WRAP_LEN % ALIGNED_SIZE != 0
#error "WRAP_LEN must by multiple of sizeof(alni)"
#endif
#define CHUNK_WRAP_LEN WRAP_LEN
#endif


struct unused_slot_head {
  alni* bnext;
};

struct used_slot_head {
  class chunkalloc* chunk_p;
};

alni calc_bsize(alni bsize);

class chunkalloc : public allocator {
  alni bsize = 0;
  alni nblocks = 0;

  alni* buff = NULL;
  alni* bnext = NULL;
  alni bfreec = 0;
  alni binitc = 0;

  inline void* get_addr(alni idx) const;
  inline alni get_idx(const void* address) const;

 public:
  chunkalloc(alni bsize, alni nblocks);
  ~chunkalloc();

  bool avaliable() override;
  alni inuse_size() override;
  alni reserved_size() override;
  bool is_empty() override;

  alni get_bsize();

  void* alloc(alni size) override;
  virtual void free(void* p) override;

  bool wrap_support() override { return true; }
  bool wrap_corrupted() override;
};