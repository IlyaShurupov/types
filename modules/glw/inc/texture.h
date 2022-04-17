
#pragma once

#include "glcommon.h"
#include "array2d.h"
#include "color.h"

namespace ogl {

	class texture {
		GLuint id;
	public:
	  GLuint getid();
		void update(const array2d<rgba>& buff);
		void draw(const GLuint& out = 0);
		texture();
		~texture();
	};

};