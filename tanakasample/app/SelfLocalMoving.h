#ifndef __SELFLOCALMOVING__
#define __SELFLOCALMOVING__

#include "Clock.h"
#include "PidWalker.h"
#include "SonarSensor.h"
#include "Self_localization.h"
#include "ColorSensor.h"
#include "util.h"

using namespace ev3api;

class SelfLocalMoving {
public:
    SelfLocalMoving();
    void moveRCourseStart();
    void moveRCourseToGoal(int pattern, int puzzle_dege);
    bool nearTarget(int _x, int _y, int _r, int _flag);

private:
    Walker walker;
    Clock clock;
    PidWalker pidWalker;
    SonarSensor sonarSensor;
    ColorSensor colorSensor;
    Self_localization self_localization;
    FILE* fp  = fopen("speed.txt", "w");
    FILE* fp2 = fopen("direction.txt", "w");
    FILE* fp3 = fopen("standard.txt", "w");
    FILE* fp4 = fopen("angle.txt", "w");
    int edge_direction = -1; // エッジの向きによって変更(+ => 右, - => 左)
    int flag_edge = 0;     // フラグ
    int set_edge = 0;
    float start_point;
};

#endif
