
#include "window.h"
#include "glutils.h"
#include "env.h"
#include "tablet.h"

#include <stdio.h>

void GLAPIENTRY
MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity, message);
}

using namespace ogl;

void window::resize(vec2f psize) {
	size = psize;
	if (winp) {
		// apply
	}
}

window::window() {
	col_clear = rgba(0.f, 0.f, 0.f, 0.5f);
	size = vec2f(1024.f, 768.f);
	init(0);
}

namespace WIN {
	#include <Windows.h>
	#define GLFW_EXPOSE_NATIVE_WIN32
	#include "GLFW/glfw3native.h"
};


void window::init(alni params) {
	resize(size);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();

	if (params & FULL_SCREEN) {
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	}

	winp = glfwCreateWindow((int) size.x, (int) size.y, "NULL", NULL, NULL);

	if (winp == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
	}

	if (!(params & FULL_SCREEN)) {
		glfwSetWindowPos(winp, 50, 50);
	}

	glfwSetInputMode(winp, GLFW_STICKY_KEYS, GL_TRUE);

	// wtf
	glfwMakeContextCurrent(winp);
	//glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
	}

	WIN::HDC ourWindowHandleToDeviceContext = WIN::GetDC(WIN::GetActiveWindow());
	printf("%s %s \n", (char*) glGetString(GL_VERSION), "OPENGL VERSION IN USE");

	glEnable(GL_ALPHA_TEST);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_BLEND_SRC);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	if (params & FACE_CULL) {
		glEnable(GL_CULL_FACE);
	}

	glEnable(GL_DEBUG_OUTPUT);

	if (params & UNDECORATED) {
		glfwSetWindowAttrib(winp, GLFW_DECORATED, GLFW_FALSE);
	}

	glDebugMessageCallback(MessageCallback, 0);

	int x, y;
	glfwGetWindowSize(winp, &x, &y);
	size.x = (halnf) x;
	size.y = (halnf) y;

	init_utils();


	tablet_init((int*) WIN::glfwGetWin32Window(winp));

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity(); 
	//glRotatef(90, 1, 0, 0);
	//gluLookAt(0, 0, -1, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	//glfwSwapInterval(1); // Enable vsync
}

window::window(vec2f psize, alni params) {
	size = psize;
	init(params);
}

void window::begin_draw() {
	// Render on the whole framebuffer, complete from the lower left corner to the upper right
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, (GLsizei) size.x, (GLsizei) size.y);
	size_update();
}

void window::clear() {
	glClearColor(col_clear.r, col_clear.g, col_clear.b, col_clear.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void window::end_draw(bool whait_for_event) {
	glfwSwapBuffers(winp);
	tablet_update((int*) WIN::glfwGetWin32Window(winp));

	if (whait_for_event) {
		glfwWaitEvents();
	} else {
		glfwPollEvents();
	}
}

inline void window::set_current() {
	glfwMakeContextCurrent(winp);
}

void window::post_quit_event() {
	quit_event = true;
}

bool window::CloseSignal() {
	return (glfwGetKey(winp, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(winp) != 0) || quit_event;
}

GLFWwindow* window::geth() {
	return winp;
}

float window::pen_pressure() {
	float out = tablet_pressure();
	if (glfwGetMouseButton(winp, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && !out) {
		return 1.f;
	}
	return out;
}

vec2f ogl::window::cursor(bool normalized) {
	vec2<alnf> out;
	glfwGetCursorPos(winp, &out.x, &out.y);

	if (normalized) {
		out.x = (out.x / size.x - 0.5) * 2;
		out.y = -(out.y / size.y - 0.5) * 2;
	} else {
		out.y = size.y - out.y;
	}

	return vec2f(out.x, out.y);
}

bool ogl::window::rmb() {
	static bool prev_state = 0;

	bool state = glfwGetMouseButton(winp, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;

	if (state == prev_state) {
		return 0;
	} else {
		prev_state = state;
		return state;
	}
}

void window::size_update() {
	int x, y;
	glfwGetWindowSize(winp, &x, &y);
	size.x = (halnf) x;
	size.y = (halnf) y;
}

void window::set_viewport(rectf rect) {
	rect.pos.clamp(0, 3000);
	rect.size.clamp(5, 3000);

	glViewport((GLsizei) rect.pos.x, (GLsizei) rect.pos.y, (GLsizei) rect.size.x, (GLsizei) rect.size.y);
}

void window::reset_viewport() {
	glViewport((GLsizei) 0, (GLsizei) 0, (GLsizei) size.x, (GLsizei) size.y);
}

window::~window() {
	glfwDestroyWindow(winp);
}

