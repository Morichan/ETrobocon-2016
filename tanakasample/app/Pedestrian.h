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
private:
	int flag=0;
	int Distance=0;
	SonarSensor sonarSensor;
	ColorSensor colorSensor;
	TouchSensor touchSensor;
	Clock clock;
	Walker walker;
	Lifter lifter;
	PidWalker pidWalker;

};

#endif
