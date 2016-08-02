#ifndef __WALKER__
#define __WALKER__

#include "Motor.h"
#include "util.h"

using namespace ev3api;

class Walker {
public:
  Walker();
  void run(int8_t pwm, int8_t turn);
  void init();
  void terminate();
  void reset();

private:
  Motor leftWheel;
  Motor rightWheel;
};


#endif
