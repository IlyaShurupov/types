
#pragma once

#include "glcommon.h"


namespace ogl {

	class window {
		public:

		enum {
			FULL_SCREEN = 0x00000001,
			UNDECORATED = 0x00000100,
			FACE_CULL = 0x00000010,
		};

		GLFWwindow* winp = NULL;
		rgba col_clear;
		vec2f size;

		void resize(vec2f psize);
		void init(alni params);

		window();
		window(vec2f size, alni params = 0);

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

		void set_viewport(rectf rect);
		void reset_viewport();

		halnf aspect_ratio() { return size.y / size.x; }

		~window();
	};

};