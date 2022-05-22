#pragma once

#include "vec.h"

namespace tp {

	struct rgb;
	struct hsv;

	struct rgb {
		halnf r, g, b;

		rgb();
		rgb(flt4 pr, flt4 pg, flt4 pb);
		rgb(flt4 val);
		void set(flt4 pr, flt4 pg, flt4 pb);

		operator hsv() const;
	};

	struct hsv {
		halnf h, s, v;

		hsv();
		hsv(flt4 ph, flt4 ps, flt4 pv);
		void set(flt4 ph, flt4 ps, flt4 pv);

		operator rgb() const;
	};

	struct rgba {
		union {
			rgb rgbs;
			struct {
				flt4 r;
				flt4 g;
				flt4 b;
			};
		};

		flt4 a;

		rgba() {}
		rgba(flt4 val) : rgbs(val), a(val) {}
		rgba(const rgb& rgbs, flt4 val) : rgbs(rgbs), a(val) {}
		rgba(flt4 pr, flt4 pg, flt4 pb, flt4 pa) : rgbs(pr, pg, pb), a(pa) {}

		void operator=(const hsv& in) {
			rgbs = in;
			a = 1;
		}
	};

	struct hsva {
		hsv rgbs;
		flt4 a;
	};

};