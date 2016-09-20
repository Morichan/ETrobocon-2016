#ifndef __PUZZLE__
#define __PUZZLE__

#include "ColorSensor.h"
#include "SonarSensor.h"
#include "Clock.h"
#include "Walker.h"
#include "Emoter.h"
#include "pid.h"
#include "self_localization.h"

using ev3api;

class Puzzle {
public:
  Pedestrian();
  void runOneBlock();

private:
  Walker walker;
  PidWalker pidWalker;

}

#endif

