#pragma once

template <typename T1, typename T2> struct tuple;

template <typename T1, typename T2>
using pair = tuple<T1, T2>;

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

template <typename T1, typename T2, typename T3>
struct tripple {
	union {
		T1 x;
		T1 v1;
	};
	union {
		T2 y;
		T2 v2;
	};
	union {
		T3 z;
		T3 v3;
	};
};