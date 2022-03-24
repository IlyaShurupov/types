
#include "color.h"


rgba::rgba() { r = g = b = a = 1.f; }
rgba::rgba(flt4 pr, flt4 pg, flt4 pb, flt4 pa) { set(pr, pg, pb, pa); }
rgba::rgba(flt4 val) { set(val, val, val, val); }
void rgba::set(flt4 pr, flt4 pg, flt4 pb, flt4 pa) { r = pr; b = pb; g = pg; a = pa; }

rgba::operator hsva() {
	hsva out;

	if (!(r || g || b)) {
		return out;
	}

	if (r > g && r > b) {
		out.v = r;
		if (g > b) {
			out.s = 1 - (b / r);
			out.h = NULL + (g / (g + r)) * PI13; // rg
		}
		else {
			out.s = 1 - (g / r);
			out.h = PI23 + (r / (b + r)) * PI13; // br
		}
	}
	else if (g > r && g > b) {
		out.v = g;
		if (r > b) {
			out.s = 1 - (b / g);
			out.h = NULL + (g / (g + r)) * PI13; // rg
		}
		else {
			out.s = 1 - (r / g);
			out.h = PI13 + (b / (g + b)) * PI13; // gb
		}
	}
	else {
		out.v = b;
		if (r > g) {
			out.s = 1 - (g / b);
			out.h = PI23 + (r / (b + r)) * PI13; // br
		}
		else {
			out.s = 1 - (r / b);
			out.h = PI13 + (b / (g + b)) * PI13; // gb
		}
	}

	return out;
}



hsva::hsva() { h = s = v = a = 0.f; }
hsva::hsva(flt4 ph, flt4 ps, flt4 pv, flt4 pa) { set(ph, ps, pv, pa); }
void hsva::set(flt4 ph, flt4 ps, flt4 pv, flt4 pa) { h = ph; s = ps; v = pv; a = pa; }

hsva::operator rgba() {
	rgba out;
	double hh, p, q, t, ff;
	long i;

	if (s <= 0.0) {  // < is bogus, just shuts up warnings
		out.r = v;
		out.g = v;
		out.b = v;
		return out;
	}

	hh = h;
	if (hh >= 360.0) hh = 0.0;
	hh /= 60.0;
	i = (long)hh;
	ff = hh - i;
	p = v * (1.0 - s);
	q = v * (1.0 - (s * ff));
	t = v * (1.0 - (s * (1.0 - ff)));

	switch (i) {
	case 0:
		out.r = v;
		out.g = t;
		out.b = p;
		break;
	case 1:
		out.r = q;
		out.g = v;
		out.b = p;
		break;
	case 2:
		out.r = p;
		out.g = v;
		out.b = t;
		break;
	case 3:
		out.r = p;
		out.g = q;
		out.b = v;
		break;
	case 4:
		out.r = t;
		out.g = p;
		out.b = v;
		break;
	case 5:
	default:
		out.r = v;
		out.g = p;
		out.b = q;
		break;
	}
	return out;
}



/*
struct rgbab {

	uint4 rgba;

	rgbab() { rgba = 0xFFFFFFFF; }
	rgbab(uint4 color) { rgba = color; }
	rgbab(uint1 r, uint1 b, uint1 g, uint1 a) { set(r, b, g, a); }
	rgbab(const rgbab& in) { rgba = in.rgba; }

	void set(uint4 color) { rgba = color; }
	void set(uint1 r, uint1 b, uint1 g, uint1 a) {
		rgba = (uint1)(a);
		rgba <<= 8; rgba |= (uint1)(r);
		rgba <<= 8; rgba |= (uint1)(g);
		rgba <<= 8; rgba |= (uint1)(b);
	}

	operator rgbaf() {
		rgbaf out;
		out.r = uint1((rgba & 0x00FF0000) >> 16) / 255.f;
		out.g = uint1((rgba & 0x0000FF00) >> 8) / 255.f;
		out.b = uint1(rgba & 0x000000FF) / 255.f;
		out.a = uint1((rgba & 0xFF000000) >> 24) / 255.f;
		return out;
	}
};
*/