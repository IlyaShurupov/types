
#pragma once

#include "allocators.h"
#include "vec2.h"
#include "rect.h"

template <typename Type>
struct array2d {

  vec2i size;
  Type* buff = nullptr;

  array2d() {}

  array2d(vec2i size) { resize(size); }

  ~array2d() {
    delete buff;
  }

  inline Type& Get(alni x, alni y) {
    assert(x < size.x&& y < size.y&& x >= 0 && y >= 0);
    return buff + size.x * y + x;
  }
  inline void Set(alni x, alni y, const Type& value) {
    assert(x < size.x && y < size.y && x >= 0 && y >= 0);
    *(buff + size.x * y + x) = value;
  }

  void resize(vec2i newsize) {
    if (size.x != newsize.x || size.y != newsize.y) {
      if (buff) 
        delete buff;
      buff = new Type[newsize.x * newsize.y];
      size = newsize;
    }
  }

  void project(array2d<Type>* in, vec2i pos) {
    
    recti unclamped_fromrect(pos, in->size);
    recti fromrect;
    recti torect(vec2i(), size);
    
    torect.intersection(unclamped_fromrect, fromrect);

    vec2i clampsize(unclamped_fromrect.size - fromrect.size);

    vec2<bool> mns(unclamped_fromrect.pos.x < 0, unclamped_fromrect.pos.y < 0);

    if (mns.x && mns.y) {
      for (int i = 0; i < fromrect.size.x; i++) {
        for (int j = 0; j < fromrect.size.y; j++) {
          int to = size.x * (fromrect.pos.y + j) + (fromrect.pos.x + i);
          buff[to] = in->buff[in->size.x * (j + clampsize.y) + (i + clampsize.x)];
        }
      }
    } else if (!mns.x && !mns.y) {
      for (int i = 0; i < fromrect.size.x; i++) {
        for (int j = 0; j < fromrect.size.y; j++) {
          int to = size.x * (fromrect.pos.y + j) + (fromrect.pos.x + i);
          buff[to] = in->buff[in->size.x * j + i];
        }
      }
    } else if (!mns.x && mns.y) {
      for (int i = 0; i < fromrect.size.x; i++) {
        for (int j = 0; j < fromrect.size.y; j++) {
          int to = size.x * (fromrect.pos.y + j) + (fromrect.pos.x + i);
          buff[to] = in->buff[in->size.x * (j + clampsize.y) + i];
        }
      }
    } else {
      for (int i = 0; i < fromrect.size.x; i++) {
        for (int j = 0; j < fromrect.size.y; j++) {
          int to = size.x * (fromrect.pos.y + j) + (fromrect.pos.x + i);
          buff[to] = in->buff[in->size.x * j + (i + clampsize.x)];
        }
      }
    }
  }

  void assign(const Type& value) {
    halni len = size.x * size.y;
    for (halni i = 0; i < len; i++) {
      buff[i] = value;
    }
  }
};

