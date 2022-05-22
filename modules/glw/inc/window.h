
#pragma once

#include "glcommon.h"

namespace tp {

	namespace ogl {

		class window {

			bool quit_event = false;

			vec2f m_prev_cursor;
			vec2f m_cursor;

			public:

			enum {
				FULL_SCREEN = 0x00000001,
				UNDECORATED = 0x00000100,
				FACE_CULL = 0x00000010,
			};

			GLFWwindow* winp = NULL;
			rgba col_clear;
			vec2f size;
			vec2f minsize;


			bool draw_event = false;
			bool new_frame = true;
			Timer device_frame_time = Timer(time_ms(1000.f / 70));

			void resize(vec2f psize);
			void init(alni params);

			window();
			window(vec2f size, alni params = 0);

			void set_current();

			void begin_draw(bool need_update = false);

			void clear();

			void end_draw(bool whait_for_event = false);

			void post_quit_event();
			bool CloseSignal();

			GLFWwindow* geth();

			vec2f cursor(bool normalized = false);
			vec2f prevcursor(bool normalized = false);

			float pen_pressure();

			bool rmb();

			void size_update();

			void set_viewport(rectf rect);
			void reset_viewport();

			halnf aspect_ratio() { return size.y / size.x; }

			bool SpecialKey();
			bool SpecialKey2();
			~window();
		};

	};

};