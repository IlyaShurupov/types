
#pragma once

template <typename V, typename K>
struct HashNode;

template <typename V, typename K>
struct map_policy {
	virtual HashNode<V, K>** alloc_table(alni nslots) = 0;
	virtual void free_table(HashNode<V, K>** table) = 0;
	virtual alni KeyHash(const K& key) = 0;
	virtual void ValCopy(HashNode < V, K >* left, HashNode < V, K >* right) = 0;
	virtual void ValDestruct(HashNode < V, K >* node) = 0;
};

template <typename V, typename K>
struct map_policy_default : map_policy<V, K> {

	heapalloc halloc;
	poolalloc palloc;

	map_policy_default() : palloc(sizeof(HashNode<V, K>), 16) {

	}

	HashNode<V, K>** alloc_table(alni nslots) {
		return (HashNode<V, K>**)halloc.alloc(sizeof(HashNode<V, K>*) * nslots);
	}

	HashNode<V, K>* alloc_node() {
		return (HashNode<V, K>*)palloc.alloc(sizeof(HashNode<V, K>));
	}

	void free_node(HashNode<V, K>* node) {
		return palloc.free(node);
	}

	void free_table(HashNode<V, K>** table) {
		return halloc.free(table);
	}

	alni KeyHash(const K& key) {
		return hash(key);
	}

	void ValCopy(HashNode < V, K >* left, HashNode < V, K >* right) {
		left->val = right->val;
	}

	void ValDestruct(HashNode < V, K >* node) {
		// release
	}
};