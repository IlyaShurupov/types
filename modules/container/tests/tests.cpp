
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

void queue_test() {
	tp::Queue<int> q;

	q.push(0);
	q.push(1);
	q.push(2);
	q.push(3);
	q.push(4);
	q.push(5);

	for (auto node : q) {
		printf("%i ", node.mIter->data);
	}


	q.pop();
	q.pop();
	q.pop();
	q.pop();
	q.pop();
	q.pop();

	q.push(3);
	q.push(4);
	q.push(5);

	for (auto node : q) {
		printf("%i ", node.mIter->data);
	}
}

void list_test() {
	tp::List<int> q;

	q.pushBack(0);
	q.pushBack(1);
	q.pushBack(2);
	q.pushBack(3);
	q.pushBack(4);
	q.pushBack(5);

	for (auto node : q) {
		printf("%i ", node.data());
	}


	q.popBack();
	q.popFront();

	q.pushBack(4);
	q.pushFront(1);

	q.invert();

	for (auto node : q) {
		printf("%i ", node.data());
	}


	q.sort([](ListNode<int>* const& node1, ListNode<int>* const& node2){ return node1->data > node2->data; });

	for (auto node : q) {
		printf("%i ", node.data());
	}
}


int main() {
	list_test();
}