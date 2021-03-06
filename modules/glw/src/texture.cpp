
#include "texture.h"

#include "glutils.h"

namespace tp {

	using namespace ogl;

	GLuint ogl::texture::getid() { return id; }

	texture::texture() {
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	texture::~texture() { glDeleteTextures(1, &id); }

	alni ogl::texture::sizeAllocatedMem() {
		return sizeof(GLuint);
	}

	alni ogl::texture::sizeUsedMem() {
		return sizeof(GLuint);
	}


	void texture::update(const Array2D<rgba>& buff) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) buff.size().x, (GLsizei) buff.size().y, 0, GL_RGBA, GL_FLOAT, buff.buff());
	}

	void texture::draw(const GLuint& out) {
		draw_texture(out, id);
	}

};