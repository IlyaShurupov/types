#pragma once

#include "env.h"

namespace tp {

	typedef alni time_ms;
	typedef alni time_ns;

	class Timer {

		time_ms mStart;
		time_ms mDuration;

		public:
		
		Timer();
		Timer(time_ms time);

		time_ms start();
		time_ms duration();
		void setDuration(time_ms dur);

		bool isTimeout();
		void reset();
		time_ms timePased();
		time_ms remainder();
		void wait();

		float easeIn(time_ms duration = 0);
		float easeOut(time_ms duration = 0);
	};

	void sleep(time_ms duration);
	time_ms get_time();
};