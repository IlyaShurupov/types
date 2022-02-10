#pragma once

#include "env.h"

typedef alni time_ms;
typedef alni time_ns;

struct timer {

  time_ms start;
  time_ms duration;

  timer();
  timer(time_ms time);
  bool timeout();
  void reset();
  time_ms past();
  time_ms remain();
  void wait_out();
  
  float ease_in(time_ms duration = 0);
  float ease_out(time_ms duration = 0);
};

void sleep(time_ms duration);

time_ms get_time();
