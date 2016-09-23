#ifndef __LIFTER__
#define __LIFTER__

#include "Motor.h"
#include "Clock.h"

#include "util.h"

using namespace ev3api;

class Lifter {
public:
  Lifter();
  void liftUp();
  void liftDown();
  void liftUpDownGradually(int pwm);
  void changeDefault(int angle);
  void defaultSet(int angle);
  void init();
  void terminate();
  void reset();

private:
  Motor liftHand;
  Clock clock;
};


#endif
