
#include "Time/Timer.h"
#include "Macros.h"
#include <chrono>
#include <thread>

#define GETTIMEMSC()                                                                                         \
  (TIME_MS__)(                                                                                                 \
      std::chrono::duration_cast<std::chrono::milliseconds>(                                                 \
          std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()) \
              .time_since_epoch())                                                                           \
          .count())

#define THREAD_SLEEP(time_ms) std::this_thread::sleep_for(std::chrono::milliseconds(time_ms))


TIME_MS__ get_time() {
  return GETTIMEMSC();
}


void TreadSleep(TIME_MS__ duration) {
  THREAD_SLEEP(duration);
}


Timer::Timer() {
  duration = 0;
  start = 0;
}

Timer::Timer(TIME_MS__ duration) {
  start = GETTIMEMSC();
  this->duration = duration;
}

bool Timer::timeout() {
  //printf("\n ----------- %lli", GETTIMEMSC());
  return duration < GETTIMEMSC() - start;
}

void Timer::reset() {
  start = GETTIMEMSC();
}

TIME_MS__ Timer::past() {
  return GETTIMEMSC() - start;
}

TIME_MS__ Timer::remain() {
  return duration - (GETTIMEMSC() - start);
}

void Timer::wait_out() {
  if (!timeout()) {
    TreadSleep(remain());
  }
}

float Timer::ease_in(TIME_MS__ duration) {

  if (!duration) {
    duration = this->duration;
  }

  float x = (1.f / duration) * past();

  float out = (1.1f * x) / (x + 0.1f);
  CLAMP(out, 0, 1);
  return out;
}

float Timer::ease_out(TIME_MS__ duration) {
  if (!duration) {
    duration = this->duration;
  }

  float x = (1.f / duration) * past();

  float out = (0.1f *(1 - x)) / (x + 0.1f);
  CLAMP(out, 0, 1);
  return out;
}
