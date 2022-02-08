
#include "containers.h"

struct avl_ts {
	alni val;

	bool operator==(const avl_ts& in) {
		return in.val == val;
	}

	bool operator>(const avl_ts& in) {
		return val > in.val;
	}
};

int main() {

	avltree<avl_ts> avl;
	avl.insert(avl_ts({0}));


	HashMap<alni, alni> mp;
	
	mp.Put(1, 1);
	mp.Put(2, 2);
	mp.Put(3, 3);
	mp.Put(4, 4);
	mp.Put(5, 5);
	mp.Put(6, 6);

	mp.Remove(1);
	mp.Remove(2);
	mp.Remove(3);

	mp.Put(1, 1);
	mp.Put(2, 2);
	mp.Put(3, 3);

	alni val = mp.Get(4);
	val = sizeof(HashMap<int, const char*>);

}