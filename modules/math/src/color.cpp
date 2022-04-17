
#include "color.h"

rgb::rgb() { r = g = b = 1.f; }
rgb::rgb(flt4 pr, flt4 pg, flt4 pb) { set(pr, pg, pb); }
rgb::rgb(flt4 val) { set(val, val, val); }
void rgb::set(flt4 pr, flt4 pg, flt4 pb) {
  r = pr;
  b = pb;
  g = pg;
}

rgb::operator hsv() {
  hsv out;

  if (!(r || g || b)) {
    return out;
  }

  if (r > g && r > b) {
    out.v = r;
    if (g > b) {
      out.s = 1 - (b / r);
      out.h = (flt4)(NULL + (g / (g + r)) * PI13);  // rg
    } else {
      out.s = 1 - (g / r);
      out.h = (flt4)(PI23 + (r / (b + r)) * PI13);  // br
    }
  } else if (g > r && g > b) {
    out.v = g;
    if (r > b) {
      out.s = 1 - (b / g);
      out.h = (flt4)(NULL + (g / (g + r)) * PI13);  // rg
    } else {
      out.s = 1 - (r / g);
      out.h = (flt4)(PI13 + (b / (g + b)) * PI13);  // gb
    }
  } else {
    out.v = b;
    if (r > g) {
      out.s = 1 - (g / b);
      out.h = (flt4)(PI23 + (r / (b + r)) * PI13);  // br
    } else {
      out.s = 1 - (r / b);
      out.h = (flt4)(PI13 + (b / (g + b)) * PI13);  // gb
    }
  }

  return out;
}

hsv::hsv() { h = s = v = 0.f; }
hsv::hsv(flt4 ph, flt4 ps, flt4 pv) { set(ph, ps, pv); }
void hsv::set(flt4 ph, flt4 ps, flt4 pv) {
  h = ph;
  s = ps;
  v = pv;
}

hsv::operator rgb() {
  rgb out;
  flt4 hh, p, q, t, ff;
  long i;

  if (s <= 0.f) {
    out.r = v;
    out.g = v;
    out.b = v;
    return out;
  }

  hh = h * 360.f;
  if (hh >= 360.f) hh = 0.f;
  hh /= 60.f;
  i = (long)hh;
  ff = hh - i;
  p = v * (1.f - s);
  q = v * (1.f - (s * ff));
  t = v * (1.f - (s * (1.f - ff)));

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