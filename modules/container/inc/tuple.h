#pragma once

template <typename T1, typename T2>
struct tuple {

	tuple() {

	}

	tuple(T1 t1, T2 t2) {
		this->head = t1;
		this->tail = t2;
	}

	union {
		T1 head;
		T1 x;
	};
	union {
		T2 tail;
		T2 y;
	};
};