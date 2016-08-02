#ifndef __PID_WALKER__
#define __PID_WALKER__

#include "ColorSensor.h"
#include "SonarSensor.h"
#include "Clock.h"
#include "Walker.h"
#include "pid.h"
#include "self_localization.h"

using namespace ev3api;

class PidWalker {
public:
  PidWalker();
  void trace();

private:
  Walker walker;
    Self_localization self_localization;
  ColorSensor colorSensor;
  SonarSensor sonarSensor;
  Clock clock;
  Pid pid;
  int8_t forward;
  int8_t turn = 0;
  int8_t border = 30;
  int8_t brightness;
  int8_t speedometer;
};




#endif
