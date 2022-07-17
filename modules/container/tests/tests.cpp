
#include "containers.h"

using namespace tp;

void avl_test() {
	struct avl_ts {
		alni val;

		bool operator==(const avl_ts& in) {
			return in.val == val;
		}

		bool operator>(const avl_ts& in) {
			return val > in.val;
		}
	};

	AvlTree<avl_ts> avl;
	avl.insert(avl_ts({0}));
}

void hmap_test() {
	HashMap<alni, alni> mp;

	mp.put(1, 1);
	mp.put(2, 2);
	mp.put(3, 3);
	mp.put(4, 4);
	mp.put(5, 5);
	mp.put(6, 6);

	mp.remove(1);
	mp.remove(2);
	mp.remove(3);

	mp.put(1, 1);
	mp.put(2, 2);
	mp.put(3, 3);

	alni val = mp.get(4);
	val = sizeof(HashMap<int, const char*>);
}

void node_test() {
	{
		//ContainerTypeHolder<int*, 0, 0> node(NULL);
		//ContainerTypeHolder<int*, 0, 0> node2 = node;
	}
}

int main() {
	node_test();
}