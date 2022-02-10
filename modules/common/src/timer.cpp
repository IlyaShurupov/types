
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


time_ms get_time() {
  return GETTIMEMSC();
}

void sleep(time_ms duration) {
  THREAD_SLEEP(duration);
}


timer::timer() {
  duration = 0;
  start = 0;
}

timer::timer(time_ms duration) {
  start = GETTIMEMSC();
  this->duration = duration;
}

bool timer::timeout() {
  return duration < GETTIMEMSC() - start;
}

void timer::reset() {
  start = GETTIMEMSC();
}

time_ms timer::past() {
  return GETTIMEMSC() - start;
}

time_ms timer::remain() {
  return duration - (GETTIMEMSC() - start);
}

void timer::wait_out() {
  if (!timeout()) {
    sleep(remain());
  }
}

float timer::ease_in(time_ms duration) {

  if (!duration) {
    duration = this->duration;
  }

  float x = (1.f / duration) * past();

  float out = (1.1f * x) / (x + 0.1f);
  CLAMP(out, 0, 1);
  return out;
}

float timer::ease_out(time_ms duration) {
  if (!duration) {
    duration = this->duration;
  }

  float x = (1.f / duration) * past();

  float out = (0.1f *(1 - x)) / (x + 0.1f);
  CLAMP(out, 0, 1);
  return out;
}
