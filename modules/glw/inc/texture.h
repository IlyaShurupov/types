
#pragma once

#include "glcommon.h"
#include "array2d.h"
#include "color.h"

namespace tp {

	namespace ogl {

		class texture {
			GLuint id;
			public:
			GLuint getid();
			void update(const Array2D<rgba>& buff);
			void draw(const GLuint& out = 0);
			texture();
			~texture();
		};

	};

};