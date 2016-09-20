#ifndef __PRIZEAREA__
#define __PRIZEAREA__

#include "Clock.h"
#include "SonarSensor.h"
#include "Walker.h"
#include "Lifter.h"
#include "Emoter.h"
#include "PidWalker.h"

#include "util.h"

using namespace ev3api;

class PrizeArea {
public:
    PrizeArea();
    void prizeMode();
    void getPrize();
    void carryPrize();

private:
    Clock clock;
    SonarSensor sonarSensor;
    Walker walker;
    Lifter lifter;
    Emoter emoter;
    PidWalker pidWalker;
};

#endif
