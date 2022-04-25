
#pragma once

#include "glcommon.h"

namespace ogl {

	class window {
	public:

		GLFWwindow* winp = NULL;
		vec4f col_clear;
		vec2f size;

		void resize(vec2f psize);
		void init();

		window();
		window(vec2f size);
		
		void set_current();
		
		void begin_draw();
		
		void clear();
		
		void end_draw(bool whait_for_event = false);

		bool CloseSignal();

		GLFWwindow* geth();

		vec2f cursor(bool normalized = false);

		float pen_pressure();

		bool rmb();

		void size_update();

		void set_viewport(vec4f rect);
		void reset_viewport();

		~window();
	};

};