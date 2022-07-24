
#pragma once

#include "glcommon.h"

#include "keycodes.h"

#include "list.h"
#include "npple.h"

namespace tp {

	namespace ogl {

		class window {

			bool quit_event = false;

			vec2f m_prev_cursor;
			vec2f m_cursor;

			public:

			enum {
				FULL_SCREEN = 0x00000001,
				FACE_CULL = 0x00000010,
				UNDECORATED = 0x00000100,
				HIDDEN = 0x00001000
			};

			GLFWwindow* winp = NULL;
			rgba col_clear;
			vec2f size;
			vec2f minsize = vec2f(200, 200);

			bool draw_event = false;
			bool new_frame = true;
			Timer device_frame_time = Timer(time_ms(1000.f / 70));

			struct EventQueue {
				tp::List<KeyEvent> keys;
			} event_queue;
			

			void resize(vec2f psize);
			void reposition(vec2f pos);
			void init(alni params);

			window();
			window(vec2f size, alni params = 0);

			void set_current();
			void setRectWorld(tp::rect<tp::alnf> rec);
			void begin_draw(bool need_update = false);
			void update_event_queue(bool whait_for_event);
			void clear();

			void end_draw();

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

			void showWindow();
			bool SpecialKey();
			bool SpecialKey2();
			~window();

			alni sizeAllocatedMem();
			alni sizeUsedMem();
		};

	};

};