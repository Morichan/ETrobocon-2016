#ifndef __FLAGMAN__
#define __FLAGMAN__

#include "TouchSensor.h"
#include "SonarSensor.h"
#include "Clock.h"
#include "app.h"

using namespace ev3api;

class Flagman {
public:
  Flagman();
  void startingWait();
  bool rewardModeFlag();

private:
  TouchSensor touchSensor;
  SonarSensor sonarSensor;
  Clock clock;
};


#endif
