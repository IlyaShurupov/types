#pragma once

#define RGBA_32 int
#define COLOR RGBA_32

#include "Macros.h"

class Color {
  public:
  
    float r = 0;
    float g = 0;
    float b = 0;
    float a = 1.f;
    
    Color() {
    }

    Color(int4 rgba_32) {
      uint1 ba = uint1((rgba_32 & 0xFF000000) >> 24);
      uint1 br = uint1((rgba_32 & 0x00FF0000) >> 16);
      uint1 bg = uint1((rgba_32 & 0x0000FF00) >> 8);
      uint1 bb = uint1(rgba_32 & 0x000000FF);

      r = br / 255.f;
      g = bg / 255.f;
      b = bb / 255.f;
      a = ba / 255.f;
    }

    Color(float r, float g, float b, float a) {
      this->r = r;
      this->g = g;
      this->b = b;
      this->a = a;
    }

    uint4 to_argb32() {
      uint4 color = (uint1)(a * 255);
      color <<= 8;
      color |= (uint1)(r * 255);
      color <<= 8;
      color |= (uint1)(g * 255);
      color <<= 8;
      color |= (uint1)(b * 255);

      return color;
    }

    void Assign(float p_r, float p_g, float p_b, float p_a) {
      r = p_r;
      g = p_g;
      b = p_b;
      a = p_a;
    }

    ~Color() {
    }

};