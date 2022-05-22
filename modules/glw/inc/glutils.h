
#pragma once

#include "glcommon.h"
#include "mat.h"
#include "rect.h"

namespace tp {

	void init_utils();
	void finalize_utils();

	void draw_texture(GLuint out, GLuint in);
	GLuint get_tex(const char* TexId);
	void drawCurcle(vec2f pos, double radius, rgba col);

	struct fpscount {
		halni frames = 0;
		Timer time;
		halni fps = 0;

		fpscount() : time(1000) {}

		void update(bool log = true);
	};

};