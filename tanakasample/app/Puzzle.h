#ifndef __PUZZLE__
#define __PUZZLE__

#include "ColorSensor.h"
#include "SonarSensor.h"
#include "Clock.h"
#include "Walker.h"
#include "Emoter.h"
#include "pid.h"
#include "self_localization.h"
#include "PidWalker.h"


using namespace ev3api;

class Puzzle {
public:
  Puzzle();
  void runOneBlock();
  void doPuzzle();
  void turn90InCircle();

private:
  int currentCircleColor;
  Walker walker;
  PidWalker pidWalker;
  SonarSensor sonarSensor;
  ColorSensor colorSensor;
  Clock clock;
};

#endif

