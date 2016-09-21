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
    
    int count_red = 0;
    int count_blue = 0;
    int count_yellow = 0;
    int count_green = 0;
    int old_circle_color;/*前回の色保存用*/
    int flag = 0;
  Walker walker;
  PidWalker pidWalker;
  SonarSensor sonarSensor;
  ColorSensor colorSensor;
  Clock clock;
    
};

#endif

