#ifndef __COLORCHECKER__
#define __COLORCHECKER__

#include "ColorSensor.h"
#include "Clock.h"
#include "PidWalker.h"
#include "Walker.h"
#include "Lifter.h"
#include "util.h"


using namespace ev3api;

class ColorChecker {
public:
  ColorChecker();
  void init();
  void checkBlockColor();
  void hoshitori();
  int8_t getColor();
private:
  ColorSensor colorSensor;
  Clock clock;
  PidWalker pidWalker;
  Walker walker;
  Lifter lifter;
  int8_t color_id;
};


#endif
