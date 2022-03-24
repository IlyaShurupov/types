#pragma once

#include "vec3.h"

struct rgba;
struct hsva;

struct rgba {
	flt4 r, g, b, a;

	rgba();
	rgba(flt4 pr, flt4 pg, flt4 pb, flt4 pa);
	rgba(flt4 val);
	void set(flt4 pr, flt4 pg, flt4 pb, flt4 pa);

	operator hsva();
};

struct hsva {
	flt4 h, s, v, a;

	hsva();
	hsva(flt4 ph, flt4 ps, flt4 pv, flt4 pa);
	void set(flt4 ph, flt4 ps, flt4 pv, flt4 pa);

	operator rgba();
};
