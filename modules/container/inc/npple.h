#pragma once

namespace tp {

	template <typename T1, typename T2> struct Tuple;

	template <typename T1, typename T2>
	using pair = Tuple<T1, T2>;

	template <typename T1, typename T2>
	struct Tuple {

		Tuple() {

		}

		Tuple(T1 t1, T2 t2) {
			this->head = t1;
			this->tail = t2;
		}

		union {
			T1 t1;
			T1 head;
			T1 x;
		};
		union {
			T2 t2;
			T2 tail;
			T2 y;
		};
	};

	template <typename T1, typename T2, typename T3>
	struct Tripple {
		union {
			T1 t1;
			T1 x;
			T1 v1;
		};
		union {
			T2 t2;
			T2 y;
			T2 v2;
		};
		union {
			T3 t3;
			T3 z;
			T3 v3;
		};
	};

};