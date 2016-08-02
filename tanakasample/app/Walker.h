#ifndef __WALKER__
#define __WALKER__

#include "Motor.h"
#include "Clock.h"

#include "util.h"

using namespace ev3api;

class Walker {
public:
  Walker();
  void run(int8_t pwm, int8_t turn);
  void edgeChange();
  void init();
  void terminate();
  void reset();

private:
  Motor leftWheel;
  Motor rightWheel;
  Clock clock;
  int8_t leftRight = 1;
};


#endif
