
#pragma once

#define HASHMAP_LOAD_FACTOR 2/3.f
#define HASHMAP_MIN_SIZE 4
#define HASHMAP_PERTURB_SHIFT 5
#define HASHMAP_DELETED_SLOT(table, idx) ((table)[idx] == (HashNode<V, K>*)-1)

#define MAP_VALID_IDX(idx) (idx >= 0)

#include "allocators.h"


namespace tp {

	template <typename K, typename V, int SZ>
	class MapIterator;

	struct MapIdx {
		alni idx;
		MapIdx(alni i) { idx = i; }
		MapIdx(halni i) { idx = i; }
		operator alni() { return idx; }
		operator bool() { return idx != -1; }
		bool operator == (const MapIdx& in) { return idx == in.idx; }
		bool operator != (const MapIdx& in) { return idx != in.idx; }
		bool operator >= (const MapIdx& in) { return idx >= in.idx; }
		bool operator <= (const MapIdx& in) { return idx <= in.idx; }
		bool operator > (const MapIdx& in) { return idx > in.idx; }
		bool operator < (const MapIdx& in) { return idx < in.idx; }
		void operator++() { idx++; }
		void operator--() { idx--; }
		alni operator-() { return -idx; }
	};

	template <typename V, typename K>
	struct HashNode {
		K key;
		V val;
	};

	template < typename V, typename K, int table_size = HASHMAP_MIN_SIZE >
	class HashMap {

		friend MapIterator<K, V, table_size>;

		PoolAlloc palloc;
		HashNode<V, K>** table;
		alni nslots;
		alni nentries = 0;

		public:

		HashMap(uint2 palloc_chunck_size = 30) : palloc(sizeof(HashNode<V, K>), palloc_chunck_size) {
			nslots = next_pow_of_2(table_size - 1);
			table = new HashNode<V, K>*[nslots]();
		}

		HashNode<V, K>** buff() const { return table; }
		alni size() const { return nentries; }
		alni slotsSize() const { return nslots; }

		void put(const K& key, const V& val) {
			alni idx = find_slot(key, false);

			if (!table[idx] || HASHMAP_DELETED_SLOT(table, idx)) {
				table[idx] = new (palloc) HashNode<V, K>();
				table[idx]->key = key;
				nentries++;
			} else if (table[idx]) {
				table[idx]->~HashNode();
				new (&table[idx]) HashNode<V, K>();
			}

			table[idx]->val = val;

			if ((float) nentries / nslots > HASHMAP_LOAD_FACTOR) {
				rehash();
			}
		}

		V& get(const K& key) {
			alni idx = find_slot(key, true);

			if (idx == -1) {
				throw Exeption("hmap key not found", false);
			}

			return table[idx]->val;
		}

		V& operator[](alni idx) {
			return table[idx]->val;
		}

		MapIdx presents(const K& key) {
			return find_slot(key, true);
		}

		bool presents(const K& key, alni& idx_out) {
			idx_out = find_slot(key, true);
			return idx_out == -1 ? 0 : 1;
		}

		V& getSlotVal(alni slot_idx) {
			return table[slot_idx]->val;
		}

		HashNode<V, K>* getSlot(const K& key) {
			alni idx = find_slot(key, true);

			if (idx == -1) {
				throw Exeption("hmap key not found", false);
			}

			return table[idx];
		}

		void remove(const K& key) {
			alni idx = find_slot(key, true);

			if (idx == -1) {
				throw Exeption("hmap key not found", false);
			}

			delete table[idx];

			table[idx] = (HashNode<V, K>*) - 1;

			nentries--;
			if ((float) nentries / nslots < 1 - HASHMAP_LOAD_FACTOR) {
				rehash();
			}
		}

		void operator=(const HashMap<V, K, table_size>& in) {
			clear();

			nslots = in.nslots;
			table = new HashNode<V, K>*[nslots]();

			for (alni i = 0; i < nslots; i++) {
				if (in.table[i] && !HASHMAP_DELETED_SLOT(in.table, i)) {
					put(in.table[i]->key, in.table[i]->val);
				}
			}
		}

		void clear() {
			for (alni i = 0; i < nslots; i++) {
				if (table[i] && !HASHMAP_DELETED_SLOT(table, i)) {
					delete table[i];
				}
			}
			delete[] table;
			table = NULL;
			nslots = NULL;
			nentries = NULL;
		}

		MapIterator<K, V, table_size> begin() {
			return MapIterator<K, V, table_size>(this);
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


		private:

		void rehash() {
			alni nslots_old = nslots;
			HashNode<V, K>** table_old = table;

			nslots = next_pow_of_2((uint8) ((1.f / (HASHMAP_LOAD_FACTOR)) * nentries + 1));
			table = new HashNode<V, K>*[nslots]();
			nentries = 0;

			for (alni i = 0; i < nslots_old; i++) {
				if (!table_old[i] || HASHMAP_DELETED_SLOT(table_old, i)) {
					continue;
				}

				alni idx = find_slot(table_old[i]->key, false);
				table[idx] = table_old[i];
				nentries++;
			}

			delete[] table_old;
		}

		alni find_slot(const K& key, bool existing) {
			alni hashed_key = hash(key);
			alni mask = nslots - 1;
			alni idx = hashed_key & mask;
			alni shift = (hashed_key >> HASHMAP_PERTURB_SHIFT) & ~1;
			alni checked_num = 0;

			NEXT:

			if (HASHMAP_DELETED_SLOT(table, idx)) {
				if (existing) {
					goto SKIP;
				}
				return idx;
			} else if (!table[idx]) {
				if (existing) {
					goto SKIP;
				}
				return idx;
			} else if (table[idx]->key == key) {
				return idx;
			}

			SKIP:
			checked_num++;
			if (checked_num == nslots) {
				return -1;
			}
			idx = ((5 * idx) + 1 + shift) & mask;
			goto NEXT;
		}
	};


	template <typename K, typename V, int SZ>
	class MapIterator {

		public:

		HashMap<V, K, SZ>* map;
		HashNode<V, K>* iter;
		alni slot_idx;
		alni entry_idx;

		HashNode<V, K>* operator->() { return iter; }

		MapIterator(HashMap<V, K, SZ>* _map) {
			slot_idx = -1;
			entry_idx = -1;
			map = _map;
			this->operator++();
		}

		operator K& () {
			return iter->key;
		}

		operator HashNode<V, K>* () {
			return iter;
		}

		operator V& () {
			return iter->val;
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
};