#pragma once

#include "vec3.h"

struct rgb;
struct hsv;

struct rgb {
	halnf r, g, b;

	rgb();
        rgb(halnf pr, flt4 pg, flt4 pb);
	rgb(flt4 val);
	void set(flt4 pr, flt4 pg, flt4 pb);

	operator hsv();
};

struct hsv {
  halnf h, s, v;

	hsv();
	hsv(flt4 ph, flt4 ps, flt4 pv);
	void set(flt4 ph, flt4 ps, flt4 pv);

	operator rgb();
};

struct rgba {
  rgb rgbs;
  flt4 a;
};

struct hsva {
  hsv rgbs;
  flt4 a;
};