
#include "glcommon.h"

#include "strings.h"
#include "array.h"

#include "array.h"
#include "filesystem.h"
#include "..\inc\shader.h"

#include "new.h"

using namespace ogl;

//string shader_path = "../rsc/shaders/";

shader::shader() {
	programm = 0;
	VertexShaderID = 0;
	FragmentShaderID = 0;
	GeometryShaderID = 0;
}

void shader::vert_bind_source(const char* vert_src) {
	VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	compile_shader(vert_src, VertexShaderID);
}

void shader::frag_bind_source(const char* frag_src) {
	FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	compile_shader(frag_src, FragmentShaderID);
}

void shader::geom_bind_source(const char* geom_src) {
	GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
	compile_shader(geom_src, GeometryShaderID);
}

void shader::compile() {
	GLint Result = GL_FALSE;
	int InfoLogLength;

	programm = glCreateProgram();
	glAttachShader(programm, VertexShaderID);
	if (GeometryShaderID) glAttachShader(programm, GeometryShaderID);
	glAttachShader(programm, FragmentShaderID);
	glLinkProgram(programm);

	// Check the program
	glGetProgramiv(programm, GL_LINK_STATUS, &Result);
	glGetProgramiv(programm, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if (InfoLogLength > 0) {
		Array<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(programm, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(programm, VertexShaderID);
	glDetachShader(programm, FragmentShaderID);
	if (GeometryShaderID) glDetachShader(programm, GeometryShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
	if (GeometryShaderID) glDeleteShader(GeometryShaderID);
}

bool shader::compile_shader(const char* ShaderCode, GLuint ShaderID) {
	GLint Result = GL_FALSE;
	int InfoLogLength;

	char const* SourcePointer = ShaderCode;
	glShaderSource(ShaderID, 1, &SourcePointer, NULL);
	glCompileShader(ShaderID);

	// Check Shader
	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if (InfoLogLength > 0) {
		Array<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	return Result;
}


void shader::load(const char* pvert, const char* pgeom, const char* pfrag) {

	// Create the shaders
	VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (pgeom) GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
	else GeometryShaderID = 0;

	GLint Result = GL_FALSE;
	int InfoLogLength = 0;

	string vert = sfmt("%c.vert", pvert);
  string geom = pgeom ? sfmt("%c.geom", pgeom) : " ";
  string frag = sfmt("%c.frag", pfrag);

	printf("Compiling shader : %s\n", vert.cstr());
	string tmp = read_file(vert.cstr());
	compile_shader(tmp.get_writable(), VertexShaderID);

	if (GeometryShaderID) {
		// Compile Geometry Shader
		printf("Compiling shader : %s\n", geom.cstr());
		compile_shader(read_file(geom.cstr()).get_writable(), GeometryShaderID);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", frag.cstr());
	compile_shader(read_file(frag.cstr()).get_writable(), FragmentShaderID);

	compile();
}

shader::shader(const char* vert, const char* geom, const char* frag) {
	load(vert, geom, frag);
}

void shader::bind() {
	glUseProgram(programm);
}

GLuint ogl::shader::getu(const char* uid) {
	return glGetUniformLocation(programm, uid);
}

void shader::unbind() {
	glUseProgram(0);
}

shader::~shader() {
	glDeleteProgram(programm);
}
