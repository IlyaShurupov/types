
#pragma once

#include "glcommon.h"
#include "vec2.h"
#include "rect.h"

void init_utils();
void finalize_utils();

void draw_texture(GLuint out, GLuint in);
GLuint get_tex(const char* TexId);
void drawCurcle(vec2f pos, double radius, vec4f col);

void CamProjectMat(float out[4][4], float fovy, float aspect, float zNear, float zFar);
void CamViewMat(float out[4][4], vec3f const& eye, vec3f const& center, vec3f const& up);

struct fpscount {
  halni frames = 0;
  timer time;
  halni fps = 0;

  fpscount() : time(1000) {}

  void update(bool log = true);
};
