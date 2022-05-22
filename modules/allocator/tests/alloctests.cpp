
#include <math.h>
#include <stdio.h>

#include "allocators.h"

struct test_struct {
  tp::alni val = 0;

  test_struct() { val = 1; }
  ~test_struct() { val = -1; }

  bool operator==(const test_struct& in) { return in.val == val; }
};

template <tp::alni size>
struct allocator_test {
  test_struct data[size];
  bool is_allocated[size];
  tp::alni n_loaded = 0;

  test_struct* allocations[size];

  tp::AbstractAllocator* alloc;
  tp::AbstractAllocator* parent_alloc;
  const char* allocator_name = NULL;

  tp::alni rand_idx(bool state) {
  RAND:

    tp::alni idx = (tp::alni)(tp::randf() * (size + 1));
    CLAMP(idx, 0, size - 1);

    if (state == is_allocated[idx]) {
      goto RAND;
    }
    return idx;
  }

  allocator_test(tp::AbstractAllocator* palloc, const char* pallocator_name,
                 tp::AbstractAllocator* p_parent_alloc) {
    allocator_name = pallocator_name;
    parent_alloc = p_parent_alloc;
    alloc = palloc;
    for (tp::alni i = 0; i < size; i++) {
    RAND:
      tp::alni val = tp::alni(tp::randf() * (size + 100.f));
      for (tp::alni check_idx = 0; check_idx < size; check_idx++) {
        if (data[check_idx].val == val) {
          goto RAND;
        }
      }

      data[i].val = val;
      is_allocated[i] = false;
      allocations[i] = 0;
    }
  }

  void verify_integrity() {
    // verify data integrity
    for (tp::alni i = 0; i < size; i++) {
      if (is_allocated[i]) {
        assert(*allocations[i] == data[i] && "data is currupted\n");
      }
    }

    if (alloc->isWrapSupport()) assert(!alloc->isWrapCorrupted());
    if (parent_alloc && parent_alloc->isWrapSupport())
      assert(!parent_alloc->isWrapCorrupted());

    verify_sizes();
  }

  void verify_sizes() {
#ifdef MEM_TRACE
    assert(alloc->sizeInuse() == n_loaded * sizeof(test_struct) &&
           "invalid inuse size\n");
    assert(alloc->sizeReserved() >= n_loaded * (tp::alni)sizeof(test_struct) &&
           "invalid reserved size\n");
#endif
  }

  void load_item(tp::alni idx) {
    if (!is_allocated[idx]) {
      allocations[idx] = new (alloc) test_struct();

      assert(allocations[idx] && "allocator returned NULL");

      allocations[idx]->val = data[idx].val;
      is_allocated[idx] = true;
      n_loaded++;
      verify_integrity();
    }
  }

  void unload_item(tp::alni idx) {
    if (is_allocated[idx]) {
      verify_integrity();
      delete allocations[idx];
      is_allocated[idx] = false;
      n_loaded--;
      verify_integrity();
    }
  }

  void change_states(tp::Range rg, bool state, bool reversed = false,
                     bool random = false) {
    for (auto i : rg) {
      tp::alni idx = i;

      if (random) {
        idx = rand_idx(state);
      } else if (reversed) {
        idx = size - i - 1;
      }

      (state) ? load_item(idx) : unload_item(idx);
    }
  }

  // full down-up load then up-down unload
  void test1() {
    change_states({0, size}, 1);
    change_states({0, size}, 0, true);
  }

  // full down-up load then down-up unload
  void test2() {
    change_states({0, size}, 1);
    change_states({0, size}, 0);
  }

  // full random load then random unload
  void test3() {
    change_states({0, size}, 1, 0, 1);
    change_states({0, size}, 0, 0, 1);
  }

  // multipul tests 1-3
  void test4() {
    test1();
    test1();

    test2();
    test2();

    test3();
    test3();
  }

  static tp::alnf sineupf(tp::alnf size, tp::alnf x, bool reverse) {
    tp::alnf end = 4 * PI;
    tp::alnf a = (2 / 7.f) * size;
    tp::alnf b = end / size;

    tp::alni c = ((-1 * reverse) + (1 * !reverse));
    tp::alnf c1 = (x - (end * reverse)) / b;
    tp::alnf c2 = (a * sin(x - (end * reverse)));
    tp::alnf out = c1 + c2;
    return c * out;
  }

  // sin load & sin unload with ~1/2 drop factor
  void test5() {
    tp::alnf end = 4 * PI;
    tp::alnf step = end / 4.f;

    for (char i = 0; i < 2; i++) {
      for (tp::alnf x = 0; x <= end; x += step) {
        tp::alni target_alloc_count = (tp::alni)ceil(sineupf(size, x, i));
        CLAMP(target_alloc_count, 0, size);

        while (n_loaded > target_alloc_count) {
          unload_item(rand_idx(0));
        }
        while (n_loaded < target_alloc_count) {
          load_item(rand_idx(1));
        }
      }
    }
  }

#ifdef MEM_WRAP
  void check_wrap(tp::alni offset, bool after) {
    CLAMP(offset, 1, WRAP_LEN);

    test_struct* ts = allocations[rand_idx(0)];
    tp::alni shift = (sizeof(test_struct) * after) + (offset - 1) * after -
                 offset * (!after);
    tp::uint1* address = (((tp::uint1*)ts) + shift);

    tp::uint1 val = *address;
    *address = 5;
    assert(alloc->isWrapCorrupted());
    *address = val;
  }
#endif

  // mem guards test
  void test6() {
    change_states({0, size}, 1);

#ifdef MEM_WRAP
    for (tp::alni after = 0; after < 2; after++) {
      for (tp::alni offset = 1; offset <= WRAP_LEN; offset++) {
        check_wrap(offset, after);
      }
    }
#endif

    change_states({0, size}, 0);
  }

  void run_tests() {
    try {
      test1();
      test2();
      test3();
      test4();
      test5();
      if (alloc->isWrapSupport()) {
        test6();
      }

      printf("%s - passed\n", allocator_name);
      if (!alloc->isWrapSupport()) {
        printf(" WARNING: %s has no wrap support!! \n", allocator_name);
      }
    } catch (...) {
      printf("%s - failed\n", allocator_name);
    }
  }
};

void heap_alloc_test() {
  tp::HeapAlloc halloc;
  allocator_test<150> hatest(&halloc, "heap allocator", NULL);
  hatest.run_tests();
}

void chunk_alloc_test() {
  {
    tp::ChunkAlloc calloc(sizeof(test_struct), 50);
    allocator_test<50> ca_test(&calloc, "chunk allocator", global_heap);
    ca_test.run_tests();
  }
  if (global_heap) assert(global_heap->sizeInuse() == 0);
}

void pool_alloc_test() {
  {
    tp::PoolAlloc palloc(sizeof(test_struct), 50);
    allocator_test<150> pa_test(&palloc, "pool allocator", global_heap);
    pa_test.run_tests();
  }
  if (global_heap) assert(global_heap->sizeInuse() == 0);
}

void allocators_test() {

  printf("running tests on alocators:\n");

  heap_alloc_test();
  chunk_alloc_test();
  pool_alloc_test();
}