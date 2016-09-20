#ifndef __PID_WALKER__
#define __PID_WALKER__

#include "ColorSensor.h"
#include "SonarSensor.h"
#include "Clock.h"
#include "Walker.h"
#include "Emoter.h"
#include "pid.h"
#include "self_localization.h"

using namespace ev3api;

class PidWalker {
public:
    PidWalker();
    void start();
    void stop();
    void accelerate(int8_t startForward, int8_t _forward);
    void brake(int8_t startForward, int8_t _forward);
    void setForward(int8_t _forward);
    void trace();
    Walker walker;

private:
    Emoter emoter;
    Self_localization self_localization;
    ColorSensor colorSensor;
    SonarSensor sonarSensor;
    Clock clock;
    Pid pid;
    int8_t forward = 25;
    int8_t turn = 0;
    int8_t border = 30;
    int8_t brightness;
    int8_t speedometer;
};




#endif
