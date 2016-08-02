#ifndef __PEDESTRIAN__
#define __PEDESTRIAN__

#include "SonarSensor.h"
#include "Walker.h"
#include "Clock.h"
#include "TouchSensor.h"
#include "Lifter.h"
#include "Emoter.h"
using namespace ev3api;

class Pedestrian {
public:
	Pedestrian();
	void monitor();
	void cross();

private:
	int flag=0;
	SonarSensor sonarSensor;
	TouchSensor touchSensor;
	Clock clock;
	Walker walker;
	Emoter emoter;
	Lifter lifter;
};

#endif
