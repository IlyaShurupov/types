
#pragma once

#include "Macros.h"
#include "Geometry/Rect.h"
#include "Memory/Allocators.h"
#include "Color.h"
#include <thread>

template <typename BMType>
struct BitMap;

template <typename BMType>
void Fill(BitMap<BMType>* bm, Rect<int>* from_to,  BMType* value) {
  for (int i = from_to->pos.x; i < from_to->size.x; i++) {
    for (int j = from_to->pos.y; j < from_to->size.y; j++) {
      bm->Set(i, j, value);
    }
  }
}

template <typename BMType>
struct BitMap {

  vec2<int> size;
  BMType* pxlbuff = nullptr;

  BitMap() {}
  template <typename SizeType>
  BitMap(SizeType width, SizeType height) {
    size.assign(width, height);
    //pxlbuff = ALLOC_AR(BMType, (int)height * (int8)width);
  }
  ~BitMap() {
    //DEALLOC(pxlbuff);
  }

  inline BMType* Get(int x, int y) { return pxlbuff + (int8)size.x * y + x; }
  inline void Set(int x, int y, BMType* value) { *(pxlbuff + (int8)size.x* y + x) = *value; }

  void resize(int width, int height) {
    if (size.x != width || size.y != height) {
      //DEALLOC(pxlbuff);
      //pxlbuff = ALLOC_AR(BMType, height * (int8)width);
      size.assign(width, height);
    }
  }

  void Project(BitMap<BMType>* buff, vec2<int> pos) {
    
    Rect<int> unclamped_fromrect(pos, buff->size);
    Rect<int> fromrect;
    Rect<int> torect(vec2<int>(), size);
    
    torect.intersection(unclamped_fromrect, fromrect);

    vec2<int> clampsize(unclamped_fromrect.size - fromrect.size);

    vec2<bool> mns(unclamped_fromrect.pos.x < 0, unclamped_fromrect.pos.y < 0);

    if (mns.x && mns.y) {
      for (int i = 0; i < fromrect.size.x; i++) {
        for (int j = 0; j < fromrect.size.y; j++) {
          int to = size.x * (fromrect.pos.y + j) + (fromrect.pos.x + i);
          pxlbuff[to] = buff->pxlbuff[buff->size.x * (j + clampsize.y) + (i + clampsize.x)];
        }
      }
    } else if (!mns.x && !mns.y) {
      for (int i = 0; i < fromrect.size.x; i++) {
        for (int j = 0; j < fromrect.size.y; j++) {
          int to = size.x * (fromrect.pos.y + j) + (fromrect.pos.x + i);
          pxlbuff[to] = buff->pxlbuff[buff->size.x * j + i];
        }
      }
    } else if (!mns.x && mns.y) {
      for (int i = 0; i < fromrect.size.x; i++) {
        for (int j = 0; j < fromrect.size.y; j++) {
          int to = size.x * (fromrect.pos.y + j) + (fromrect.pos.x + i);
          pxlbuff[to] = buff->pxlbuff[buff->size.x * (j + clampsize.y) + i];
        }
      }
    } else {
      for (int i = 0; i < fromrect.size.x; i++) {
        for (int j = 0; j < fromrect.size.y; j++) {
          int to = size.x * (fromrect.pos.y + j) + (fromrect.pos.x + i);
          pxlbuff[to] = buff->pxlbuff[buff->size.x * j + (i + clampsize.x)];
        }
      }
    }
  }

  // draw methods
  void DrawRect(Rect<int>& rect, BMType& value) {
    /*
    Rect<int> myrect(vec2<int>(), size);
    Rect<int> projectrect;
    myrect.intersection(rect, projectrect);
    
    Rect<int> from_to1(projectrect);
    from_to1.size.assign((projectrect.size_vec() / 2) + projectrect.pos);

    Rect<int> from_to2(from_to1);
    from_to2.size.y += projectrect.size.y / 2;
    from_to2.pos.y += projectrect.size.y / 2;

    Rect<int> from_to3(from_to1);
    from_to3.pos += projectrect.size_vec() / 2;
    from_to3.size += projectrect.size_vec() / 2;

    Rect<int> from_to4(from_to1);
    from_to4.size.x += projectrect.size.x / 2;
    from_to4.pos.x += projectrect.size.x / 2;

    std::thread t1(Fill<BMType>, this, &from_to1, &value);
    std::thread t2(Fill<BMType>, this, &from_to2, &value);
    std::thread t3(Fill<BMType>, this, &from_to3, &value);
    std::thread t4(Fill<BMType>, this, &from_to4, &value);

    t1.join(); 
    t2.join();
    t3.join(); 
    t4.join(); 
    */
  }

  void DrawBounds(Rect<int>& rect, BMType& value, short thickness) {
    Rect<int> myrect(vec2<int>(), size);
    Rect<int> projectrect;
    myrect.intersection(rect, projectrect);
    int lastpxlx = projectrect.pos.x + projectrect.size.x - 1;
    int lastpxly = projectrect.pos.y + projectrect.size.y - 1;
    short th1 = 0;
    short th2 = thickness;
    while (th1 < th2) {
      if (thickness < projectrect.size.y) {
        for (int i = projectrect.pos.x; i < lastpxlx; i++) {
          Set(i, projectrect.pos.y + th1, &value);
          Set(i, lastpxly - th1, &value);
        }
      }
      if (thickness < projectrect.size.x) {
        for (int j = projectrect.pos.y; j < lastpxly; j++) {
          Set(projectrect.pos.x + th1, j, &value);
          Set(lastpxlx - th1, j, &value);
        }
      }
      th1++;
    }
  }

  void Assign(BMType* value) {
    long int len = size.x * size.y;
    for (int i = 0; i < len; i++) {
      pxlbuff[i] = *value;
    }
  }
};

