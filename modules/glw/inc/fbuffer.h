
#pragma once

#include "glcommon.h"

namespace ogl {

	class fbuffer {

		GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		GLuint FramebufferName;
		GLuint renderedTexture;
		GLuint depthrenderbuffer;

		vec2f size;
		vec4f col_clear;

	public:

		fbuffer(vec2i p_size, vec4f pcol_clear);
		void begin_draw();
		void clear();
		void end_draw();
		GLuint texId();
		~fbuffer();
	};

};