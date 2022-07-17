
#include <chrono>
#include <thread>

#include "timer.h"

#include "macros.h"

#define GETTIMEMSC()                                                                                         \
  (time_ms)(                                                                                                 \
      std::chrono::duration_cast<std::chrono::milliseconds>(                                                 \
          std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()) \
              .time_since_epoch())                                                                           \
          .count())

#define THREAD_SLEEP(time_ms) std::this_thread::sleep_for(std::chrono::milliseconds(time_ms))

namespace tp {

	time_ms get_time() {
		return GETTIMEMSC();
	}

	void sleep(time_ms mDuration) {
		THREAD_SLEEP(mDuration);
	}


	Timer::Timer() {
		mDuration = 0;
		mStart = GETTIMEMSC();
	}

	Timer::Timer(time_ms mDuration) {
		mStart = GETTIMEMSC();
		this->mDuration = mDuration;
	}

	bool Timer::isTimeout() {
		return mDuration < GETTIMEMSC() - mStart;
	}

	void Timer::reset() {
		mStart = GETTIMEMSC();
	}

	time_ms Timer::timePased() {
		return GETTIMEMSC() - mStart;
	}

	time_ms Timer::remainder() {
		return mDuration - (GETTIMEMSC() - mStart);
	}

	time_ms Timer::start() { return mStart; }
	time_ms Timer::duration() { return mDuration; }
	void Timer::setDuration(time_ms dur) { mDuration = dur; }

	void Timer::wait() {
		if (!isTimeout()) {
			sleep(remainder());
		}
	}

	float Timer::easeIn(time_ms pDuration) {

		if (!pDuration) {
			pDuration = mDuration;
		}

		float x = (1.f / mDuration) * timePased();

		float out = (1.1f * x) / (x + 0.1f);
		CLAMP(out, 0, 1);
		return out;
	}

	float Timer::easeOut(time_ms pDuration) {
		if (!pDuration) {
			mDuration = this->mDuration;
		}

		float x = (1.f / mDuration) * timePased();

		float out = (0.1f * (1 - x)) / (x + 0.1f);
		CLAMP(out, 0, 1);
		return out;
	}

};