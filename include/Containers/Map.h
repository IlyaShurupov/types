
#pragma once

#include "Macros.h"
#include "List.h"
#include "Strings/Strings.h"

#include "Memory/Mem.h"

#define HASHMAP_LOAD_FACTOR 2/3.f
#define HASHMAP_MIN_SIZE 4
#define HASHMAP_PERTURB_SHIFT 5
#define HASHMAP_DELETED_SLOT(table, idx) ((table)[idx] == (HashNode<V, K>*)-1)

template <typename K, typename V, typename HF, typename CF, int SZ>
class MapIterator;

template <typename Val>
struct CopyBytes {
	inline const Val& operator() (const Val& val) {
		return val;
	}
};

template <typename V, typename K>
struct HashNode {
	K key;
	V val;
};

template <typename V, typename K, typename Hashfunc, typename CopyValfunc = CopyBytes<V>, int table_size = HASHMAP_MIN_SIZE>
class HashMap {

	void free_table() {
		for (alni i = 0; i < nslots; i++) {
			table[i] = nullptr;
		}
	}

public:

	HashNode<V, K>** table;
	alni nslots;
	alni nentries = 0;
	bool del_values = true;

	Hashfunc hash;
	CopyValfunc copy_val;

	void rehash() {
		int nslots_old = nslots;
		HashNode<V, K>** table_old = table;

		nslots = next_pow_of_2((uint8)((1.f / (HASHMAP_LOAD_FACTOR)) * nentries + 1));
		table = new HashNode<V, K>*[nslots]();
		free_table();
		nentries = 0;

		for (int i = 0; i < nslots_old; i++) {
			if (!table_old[i] || HASHMAP_DELETED_SLOT(table_old, i)) {
				continue;
			}

			int idx = find_slot(table_old[i]->key, false);
			table[idx] = table_old[i];
			nentries++;
		}
		delete table_old;
	}

	int find_slot(const K& key, bool existing) {
		int hashed_key = hash(key);
		int mask = nslots - 1;
		int idx = hashed_key & mask;
		int shift = (hashed_key >> HASHMAP_PERTURB_SHIFT) & ~1;

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
		table = new HashNode<V, K>*[nslots]();
		free_table();
	}

	void Put(const K& key, const V& val) {
		int idx = find_slot(key, false);

		if (!table[idx] || HASHMAP_DELETED_SLOT(table, idx)) {
			table[idx] = new HashNode<V, K>;
			table[idx]->key = key;
			nentries++;
		}
		else if (table[idx] && del_values) {
			delete table[idx]->val;
		}

		table[idx]->val = val;

		if ((float)nentries / nslots > HASHMAP_LOAD_FACTOR) {
			rehash();
		}
	}

	int Presents(const K& key) {
		int idx = find_slot(key, true);
		return idx == -1 ? -1 : idx;
	}

	V& Get(const K& key) {
		int idx = find_slot(key, true);
		assert(idx != -1);
		return table[idx]->val;
	}

	HashNode<V, K>* GetEntry(const K& key) {
		int idx = find_slot(key, true);
		return table[idx];
	}

	void Remove(const K& key) {
		int idx = find_slot(key, true);

		if (del_values) {
			delete table[idx]->val;
		}

		delete table[idx];
		table[idx] = (HashNode<V, K>*) - 1;

		nentries--;
		if ((float)nentries / nslots < 1 - HASHMAP_LOAD_FACTOR) {
			rehash();
		}
	}

	void operator=(const HashMap<V, K, Hashfunc, CopyValfunc, table_size>& in) {
		clear();

		nslots = in.nslots;
		table = new HashNode<V, K>*[nslots]();

		for (int i = 0; i < nslots; i++) {
			if (in.table[i] && !HASHMAP_DELETED_SLOT(in.table, i)) {
				Put(in.table[i]->key, copy_val(in.table[i]->val));
			}
		}
	}

	void clear() {
		for (int i = 0; i < nslots; i++) {
			if (table[i] && !HASHMAP_DELETED_SLOT(table, i)) {
				if (del_values) {
					delete table[i]->val;
				}
				delete table[i];
			}
		}
		delete table;
	}

	MapIterator<K, V, Hashfunc, CopyValfunc, table_size> begin() {
		return MapIterator<K, V, Hashfunc, CopyValfunc, table_size>(this);
	}

	alni end() {
		return nslots;
	}

	int SlotIdx(int entry_idx_in) {
		int entry_idx = -1;
		for (int slot_idx = 0; slot_idx < nslots; slot_idx++) {
			if (table[slot_idx]) {
				entry_idx++;
			}
			if (entry_idx == entry_idx_in) {
				return slot_idx;
			}
		}
		return -1;
	}

	HashNode<V, K>* GetEntry(int idx) {
		return table[SlotIdx(idx)];
	}

	~HashMap() {
		clear();
	}
};


template <typename K, typename V, typename HF, typename CF, int SZ>
class MapIterator {

public:

	HashMap<V, K, HF, CF, SZ>* map;
	HashNode<V, K>* iter;
	int slot_idx;
	int entry_idx;

	HashNode<V, K>* operator->() { return iter; }

	MapIterator(HashMap<V, K, HF, CF, SZ>* _map) {
		slot_idx = -1;
		entry_idx = -1;
		map = _map;
		this->operator++();
	}

	void operator++() {
		slot_idx++;

		while (HASHMAP_DELETED_SLOT(map->table, slot_idx) || !map->table[slot_idx]) {
			slot_idx++;

			if (slot_idx == map->nslots) {
				return;
			}
		}

		iter = map->table[slot_idx];
		entry_idx++;
	}

	bool operator!=(int p_idx) {
		return slot_idx != p_idx;
	}

	const MapIterator& operator*() { return *this; }
};

struct StrHashPolicy {
	inline int operator()(const Str& str) {
		return hash_string(str.str);
	}
};

template< typename Type, typename CopyValfunc = CopyBytes<Type>, int table_size = HASHMAP_MIN_SIZE >
using Dict = HashMap<Type, Str, StrHashPolicy, CopyValfunc, table_size>;