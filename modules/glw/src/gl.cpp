
#include "gl.h"

#include "glcommon.h"

#include <stdio.h>

namespace tp {

	ogl::opengl::opengl() {

		if (!glfwInit()) {
			assert(0 && "GLFW Initialization Error");
		}

		if (0) {
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
		}

		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // No old OpenGL
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); // No old OpenGL

		glfwWindowHint(GLFW_SAMPLES, 8); // 4x anti-aliasing
		glEnable(GL_MULTISAMPLE);

		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

		int major, minor, revision;
		glfwGetVersion(&major, &minor, &revision);
		printf("Running against GLFW %i.%i.%i\n", major, minor, revision);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}

	ogl::opengl::~opengl() {
		glfwTerminate();
	}

};