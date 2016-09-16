#ifndef __PEDESTRIAN__
#define __PEDESTRIAN__

#include "SonarSensor.h"
#include "ColorSensor.h"
#include "Walker.h"
#include "Clock.h"
#include "TouchSensor.h"
#include "Lifter.h"
#include "PidWalker.h"
#include "util.h"

using namespace ev3api;

class Pedestrian {
public:
	Pedestrian();
	void init();
	void monitor();
	void cross();
	void sumou(int8_t target_color);
	void push();
  void moveColor();
  void moveCross();
  void turnLine(int8_t direction);
  void turnColor(int8_t way);
private:
	int Distance=0;
	SonarSensor sonarSensor;
	ColorSensor colorSensor;
	Clock clock;
	Walker walker;
	Lifter lifter;
	PidWalker pidWalker;

};

#endif
