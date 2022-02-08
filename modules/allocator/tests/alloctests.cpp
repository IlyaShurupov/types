
#include "allocators.h"

#include <stdio.h>

#include <math.h>

struct test_struct {
	
	alni val = 0;
	
	test_struct() {}
	~test_struct() {}
	
	bool operator==(const test_struct& in) {
		return in.val == val;
	}
};

template <alni size>
struct allocator_test {

	test_struct data[size];
	bool is_allocated[size];
	alni n_loaded = 0;

	test_struct* allocations[size];

	allocator* alloc;
	allocator* parent_alloc;
	const char* allocator_name = NULL;

	alni rand_idx() {
		alni idx = (alni)(randf() * (size + 1));
		CLAMP(idx, 0, size - 1);
		return idx;
	}

	allocator_test(allocator* palloc, const char* pallocator_name, allocator* p_parent_alloc = NULL) {
		parent_alloc = p_parent_alloc;
		allocator_name = pallocator_name;
		alloc = palloc;
		for (alni i = 0; i < size; i++) {
			
			RAND:
			alni val = alni(randf() * (size + 100.f));
			for (alni check_idx = 0; check_idx < size; check_idx++) {
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
		for (alni i = 0; i < size; i++) {
			if (is_allocated[i]) {
				assert(*allocations[i] == data[i]);
			}
		}

		assert(alloc->inuse_size() == n_loaded * sizeof(test_struct));
		assert(alloc->reserved_size() >= n_loaded * (alni)sizeof(test_struct));
	}

	void load_item(alni idx) {
		if (!is_allocated[idx]) {
			allocations[idx] = new(alloc) test_struct();
			
			assert(allocations[idx]);

			allocations[idx]->val = data[idx].val;
			is_allocated[idx] = true;
			n_loaded++;
			verify_integrity();
		}
	}

	void unload_item(alni idx) {
		if (is_allocated[idx]) {
			mfree(alloc, allocations[idx]);
			is_allocated[idx] = false;
			n_loaded--;
			verify_integrity();
		}
	}

	void change_states(range rg, bool state, bool reversed = false, bool random = false) {
		for (auto i : rg) {
			
			alni idx = i;

			if (random) {
				idx = rand_idx();
			}
			else if (reversed) {
				idx = size - i - 1;
			}

			(state) ? load_item(idx) : unload_item(idx);
		}
	}

	// full down-up load then up-down unload
	void test1() {
		change_states({ 0, size }, 1);
		change_states({ 0, size }, 0, true);
	}

	// full down-up load then down-up unload
	void test2() {
		change_states({ 0, size }, 1);
		change_states({ 0, size }, 0);
	}

	// full random load then random unload
	void test3() {
		change_states({ 0, size }, 1, 0, 1);
		change_states({ 0, size }, 0, 0, 1);
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

	// sin load & sin unload with ~1/2 drop factor
	void test5() {

		alnf a = (2 / 7.f) * size;
		alnf b = 4 * Pie / size;
		alnf end = 4 * Pie;
		alnf step = b / 100.f;

		alni alloc_num = 0;

		for (char i = 0; i < 2; i++) {
			for (alnf x = 0; x < b; x += step) {
			
				alni target_alloc_count = (alni)ceil((-1 * i) * (a * sin(x - (end * i)) + (x - (end * i)) / b));
				CLAMP(target_alloc_count, 0, size);

				while (alloc_num > target_alloc_count) {
					unload_item(rand_idx());
					alloc_num--;
				}
				while (alloc_num < target_alloc_count) {
					load_item(rand_idx());
					alloc_num++;
				}
			}
		}
	}

#ifdef MEM_WRAP
	void check_wrap(alni offset, bool after) {
		CLAMP(offset, 1, WRAP_LEN);
		
		test_struct* ts = allocations[rand_idx()];
		alni shift = (sizeof(test_struct) * after) + (offset - 1) * after - offset * (!after);
		uint1* address = (((uint1*)ts) + shift);
		
		*address = 5;
		assert(alloc->wrap_corrupted());
		*address = WRAP_FILL_VAL;
	}
#endif

	// mem guards test
	void test6() {

		change_states({ 0, size }, 1);

		#ifdef MEM_WRAP
		for (alni after = 0; after < 2; after++) {
			for (alni offset = 1; offset <= WRAP_LEN; offset++) {
				check_wrap(offset, after);
			}
		}
		#endif

		change_states({ 0, size }, 0);
	}

	void run_tests() {
		try {
			
			test1();
			test2();
			test3();
			test4();
			test5();
			test6();

			assert(alloc->inuse_size() == 0);

			if (parent_alloc)
				assert(parent_alloc->inuse_size() == 0);

			printf("%s - passed\n", allocator_name);
		}
		catch (...) {
			printf("%s - failed\n", allocator_name);
		}
	}
};


int main() {
	printf("running tests on alocators:\n");

	heapalloc halloc;
	allocator_test<150> hatest(&halloc, "heap allocator");
	hatest.run_tests();
	
	chunkalloc calloc(&halloc, sizeof(test_struct), 50);
	allocator_test<50> ca_test(&calloc, "chunk allocator", &halloc);
	ca_test.run_tests();

	poolalloc palloc(sizeof(test_struct), 50);
	allocator_test<150> pa_test(&palloc, "pool allocator", &pool_halloc);
	pa_test.run_tests();
}