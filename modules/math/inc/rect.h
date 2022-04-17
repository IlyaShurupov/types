
#pragma once

#include "vec2.h"

template <typename Type>
class rect;
using rectf = rect<halnf>;
using recti = rect<halni>;
using vec4f = rectf;
using vec4i = recti;

template <typename Type>
class rect {
 public:
  union {
    vec2<Type> v1;
    vec2<Type> pos;
    struct {
      Type x;
      Type y;
    };
  };
  union {
    vec2<Type> v2;
    vec2<Type> size;
    struct {
      Type w;
      Type z;
    };
  };

  rect() {}

  template <typename ConversionType>
  rect(const rect<ConversionType>& rec) {
    this->pos = rec.pos;
    this->size = rec.size;
  }

  rect(const vec2<Type>& pos, const vec2<Type>& size) {
    this->pos = pos;
    this->size = size;
  }

  rect(Type posx, Type posy, Type sizex, Type sizey) {
    this->pos.assign(posx, posy);
    this->size.assign(sizex, sizey);
  }

  // assign
  template <typename InType>
  rect<Type>& assign(InType p1x, InType p1y, InType p2x, InType p2y) {
    pos.assign(p1x, p1y);
    size.assign(p2x, p2y);
    return *this;
  }

  // conversion
  template <typename ConversionType>
  rect<Type>& operator=(const rect<ConversionType>& rect) {
    pos = rect.pos;
    size = rect.size;
    return *this;
  }

  bool operator==(rect<Type>& rect) const {
    return (pos == rect.pos && size == rect.size);
  }

  bool enclosed_in(rect<Type>& rect, bool prnt = false) const {
    if (prnt) {
      return (pos.x + size.x <= rect.size.x && pos.y + size.y <= rect.size.y &&
              pos.x >= 0 && pos.y >= 0);
    }

    *(vec2<Type>*)(&pos) -= rect.pos;
    bool ret = this->enclosed_in(rect, true);
    *(vec2<Type>*)(&pos) += rect.pos;
    return ret;
  }

  void intersection(rect<Type>& in, rect<Type>& out) const {
    if (overlap(in)) {
      out = *this;
      for (char i = 0; i < 2; i++) {
        CLAMP(out.pos[i], in.pos[i], in.pos[i] + in.size[i]);
        Type p2 = pos[i] + size[i];
        CLAMP(p2, in.pos[i], in.pos[i] + in.size[i]);
        out.size[i] = p2 - out.pos[i];
      }
    } else {
      out.size.assign(0, 0);
      out.pos.assign(0, 0);
    }
  }

  // argument inside
  bool inside(const vec2<Type>& p) const {
    return (p.each_compre(pos) && (pos + size).each_compre(p));
  }

  bool inside(Type x, Type y) const {
    return (pos.x < x && pos.y < y && pos.x + size.x > x && pos.y + size.y > y);
  }

  inline vec2<Type> size_vec() const { return vec2<Type>(size.x, size.y); }

  inline vec2<Type> size_vecw() const {
    return vec2<Type>(size.x + pos.x, size.y + pos.y);
  }

  void invy(Type scr_y) { pos.y = scr_y - pos.y - size.y; }

  void move(Type dx, Type dy) {
    pos.x += dx;
    pos.y += dy;
  }

  inline bool above(const rect<Type>& rect) const {
    return (pos.y + size.y < rect.pos.y);
  }
  inline bool bellow(const rect<Type>& rect) const {
    return (rect.pos.y + rect.size.y < pos.y);
  }
  inline bool right(const rect<Type>& rect) const {
    return (pos.x + size.x < rect.pos.x);
  }
  inline bool left(const rect<Type>& rect) const {
    return (rect.pos.x + rect.size.x < pos.x);
  }

  inline bool intersect_y(const rect<Type>& in) const {
    if (INRANGE(in.pos.x, pos.x, pos.x + size.x)) return true;
    if (INRANGE(pos.x, in.pos.x, in.pos.x + in.size.x)) return true;
    return false;
  }

  inline bool intersect_x(const rect<Type>& rect) const {
    if (INRANGE(rect.pos.y, pos.y, pos.y + size.y)) return true;
    if (INRANGE(pos.y, rect.pos.y, rect.pos.y + rect.size.y)) return true;
    return false;
  }

  bool overlap(const rect<Type>& rect) const {
    return (intersect_x(rect) && intersect_y(rect));
  }

  void clamp(const rect<Type>& bounds) {
    vec2<Type> p3(pos + size);
    vec2<Type> max(bounds.pos + bounds.size);

    pos.clamp(bounds.pos, max);
    p3.clamp(bounds.pos, max);

    size = p3 - pos;
  }
};