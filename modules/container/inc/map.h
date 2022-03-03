
#pragma once

#include "map_policies.h"

#define HASHMAP_LOAD_FACTOR 2/3.f
#define HASHMAP_MIN_SIZE 4
#define HASHMAP_PERTURB_SHIFT 5
#define HASHMAP_DELETED_SLOT(table, idx) ((table)[idx] == (HashNode<V, K>*)-1)

#define MAP_VALID_IDX(idx) (idx >= 0)

template <typename V, typename K>
struct HashNode {
	K key;
	V val;

	~HashNode() {
		return;
	}
};

template <typename K, typename V, typename hmpolicy, int SZ>
class MapIterator;

template < typename V, typename K, typename hmpolicy = map_policy_default< V, K >, int table_size = HASHMAP_MIN_SIZE >
class HashMap {

public:

	HashNode<V, K>** table;
	alni nslots;
	alni nentries = 0;
	hmpolicy mp;

	void rehash() {
		alni nslots_old = nslots;
		HashNode<V, K>** table_old = table;

		nslots = next_pow_of_2((uint8)((1.f / (HASHMAP_LOAD_FACTOR)) * nentries + 1));
		table = mp.alloc_table(nslots);
		nentries = 0;

		for (alni i = 0; i < nslots_old; i++) {
			if (!table_old[i] || HASHMAP_DELETED_SLOT(table_old, i)) {
				continue;
			}

			alni idx = find_slot(table_old[i]->key, false);
			table[idx] = table_old[i];
			nentries++;
		}

		mp.free_table(table_old);
	}

	alni find_slot(const K& key, bool existing) {
		alni hashed_key = mp.KeyHash(key);
		alni mask = nslots - 1;
		alni idx = hashed_key & mask;
		alni shift = (hashed_key >> HASHMAP_PERTURB_SHIFT) & ~1;

	NEXT:

		if (HASHMAP_DELETED_SLOT(table, idx)) {
			if (existing) {
				return -1;
			}
			return idx;
		}
		else if (!table[idx]) {
			if (existing) {
				return -1;
			}
			return idx;
		}
		else if (table[idx]->key == key) {
			return idx;
		}

		idx = ((5 * idx) + 1 + shift) & mask;
		goto NEXT;

	}

	HashMap() {
		nslots = next_pow_of_2(table_size - 1);
		table = mp.alloc_table(nslots);
	}

	void Put(const K& key, const V& val) {
		alni idx = find_slot(key, false);

		if (!table[idx] || HASHMAP_DELETED_SLOT(table, idx)) {
			table[idx] = mp.alloc_node();
			table[idx]->key = key;
			nentries++;
		}
		else if (table[idx]) {
			mp.ValDestruct(table[idx]);
		}

		table[idx]->val = val;

		if ((float)nentries / nslots > HASHMAP_LOAD_FACTOR) {
			rehash();
		}
	}

	alni Presents(const K& key) {
		alni idx = find_slot(key, true);
		return idx == -1 ? -1 : idx;
	}

	bool Presents(const K& key, alni& idx_out) {
		idx_out = find_slot(key, true);
		return idx_out == -1 ? 0 : 1;
	}

	V& from_slot_idx(alni slot_idx) {
		return table[slot_idx]->val;
	}

	V& Get(const K& key) {
		alni idx = find_slot(key, true);

		if (idx == -1) {
			throw typesExeption("hmap key not found", false);
		}
		
		return table[idx]->val;
	}

	V& operator[](alni idx) {
		return table[idx]->val;
	}

	HashNode<V, K>* GetEntry(const K& key) {
		alni idx = find_slot(key, true);
		
		if (idx == -1) {
			throw typesExeption("hmap key not found", false);
		}

		return table[idx];
	}

	void Remove(const K& key) {
		alni idx = find_slot(key, true);

		if (idx == -1) {
			throw typesExeption("hmap key not found", false);
		}


		mp.ValDestruct(table[idx]);


		mp.free_node(table[idx]);
		table[idx] = (HashNode<V, K>*) - 1;

		nentries--;
		if ((float)nentries / nslots < 1 - HASHMAP_LOAD_FACTOR) {
			rehash();
		}
	}

	void operator=(const HashMap<V, K, hmpolicy, table_size>& in) {
		clear();

		nslots = in.nslots;
		table = mp.alloc_table(nslots);

		for (alni i = 0; i < nslots; i++) {
			if (in.table[i] && !HASHMAP_DELETED_SLOT(in.table, i)) {
				//Put(in.table[i]->key, copy_val(in.table[i]->val));
			}
		}
	}

	void clear() {
		for (alni i = 0; i < nslots; i++) {
			if (table[i] && !HASHMAP_DELETED_SLOT(table, i)) {
				mp.ValDestruct(table[i]);
				mp.free_node(table[i]);
			}
		}
		mp.free_table(table);
		table = NULL;
		nslots = NULL;
		nentries = NULL;
	}

	MapIterator<K, V, hmpolicy, table_size> begin() {
		return MapIterator<K, V, hmpolicy, table_size>(this);
	}

	alni end() {
		return nslots;
	}

	alni SlotIdx(alni entry_idx_in) {
		alni entry_idx = -1;
		for (alni slot_idx = 0; slot_idx < nslots; slot_idx++) {
			if (table[slot_idx]) {
				entry_idx++;
			}
			if (entry_idx == entry_idx_in) {
				return slot_idx;
			}
		}
		return -1;
	}

	HashNode<V, K>* GetEntry(alni idx) {
		return table[SlotIdx(idx)];
	}

	~HashMap() {
		clear();
	}
};


template <typename K, typename V, typename hmpolicy, int SZ>
class MapIterator {

public:

	HashMap<V, K, hmpolicy, SZ>* map;
	HashNode<V, K>* iter;
	alni slot_idx;
	alni entry_idx;

	HashNode<V, K>* operator->() { return iter; }

	MapIterator(HashMap<V, K, hmpolicy, SZ>* _map) {
		slot_idx = -1;
		entry_idx = -1;
		map = _map;
		this->operator++();
	}

	void operator++() {
		slot_idx++;

		while ((HASHMAP_DELETED_SLOT(map->table, slot_idx) || !map->table[slot_idx]) && (slot_idx != map->nslots)) {
			slot_idx++;
		}

		if (slot_idx != map->nslots) {
			iter = map->table[slot_idx];
			entry_idx++;
		}
	}

	bool operator!=(alni p_idx) {
		return slot_idx != p_idx;
	}

	const MapIterator& operator*() { return *this; }
};