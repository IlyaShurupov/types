
#pragma once

#include "glcommon.h"

namespace tp {

	namespace ogl {

		class shader {

			GLuint programm;
			GLuint VertexShaderID;
			GLuint FragmentShaderID;
			GLuint GeometryShaderID;

			bool compile_shader(const char* ShaderCode, GLuint ShaderID);
			void load(const char* vert, const char* geom, const char* frag);

			public:

			shader();

			void vert_bind_source(const char* vert_src);
			void frag_bind_source(const char* frag_src);
			void geom_bind_source(const char* geom_src);

			void compile();

			shader(const char* vertid, const char* geomid, const char* fragid);
			void bind();
			GLuint getu(const char* uid);
			void unbind();
			~shader();
		};

	};

};