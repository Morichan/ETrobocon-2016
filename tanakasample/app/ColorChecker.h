#ifndef __COLORCHECKER__
#define __COLORCHECKER__

#include "ColorSensor.h"
#include "Clock.h"

#include "Lifter.h"
#include "util.h"

using namespace ev3api;

class ColorChecker {
public:
  ColorChecker();
  void checkBlockColor();

private:
  ColorSensor colorSensor;
  Clock clock;
  Lifter lifter;
};


#endif
