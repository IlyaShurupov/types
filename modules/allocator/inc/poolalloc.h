#pragma once

#include "heapalloc.h"

struct chunk_list {
  struct chunk_node* last = NULL;

  chunk_node* addchunk(class poolalloc* self, alni bsize, alni nblocks);
  void delchunk(chunk_node* node);
  void initialize();
  void finalize();
};

class poolalloc : public allocator {
  chunk_list chunks;
  chunk_node* last_used;

  alni bsize;
  alni nblocks;

 public:
  poolalloc(alni pbsize, alni pnblocks);
  ~poolalloc();

  bool avaliable() override;
  alni inuse_size() override;
  alni reserved_size() override;
  bool is_empty() override;

  void* alloc(alni size) override;

  void free(void* p) override;

  bool wrap_support() override { return true; }
  bool wrap_corrupted() override;
};