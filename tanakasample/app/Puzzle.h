#ifndef __PUZZLE__
#define __PUZZLE__

#include "ColorSensor.h"
#include "SonarSensor.h"
#include "Clock.h"
#include "Walker.h"
#include "Lifter.h"
#include "pid.h"
#include "self_localization.h"
#include "Explorer.h"
#include "ColorChecker.h"
#include "PidWalker.h"


using namespace ev3api;

class Puzzle {
public:
    Puzzle();
    void runOneBlock();
    void doPuzzle();
    int nextStartPoint();
    int nextPointAsBlockIsNow(int b1, int b2, int b3, int b4);
    bool isThereBlock(int b1, int b2, int b3, int b4, int block);
    void turn90InCircle();

    void goNextPoint();
    void goAheadNode();
    void goLeftEdge();
    void goRightEdge();
    void goFrontEdge();
    void goBackEdge();
    void goLowRightEdge();
    void goUpRightEdge();
    void goLowLeftEdge();
    void goUpLeftEdge();

private:
    int selfLocalizationStart = 1;
    int currentCircleColor;
    int count_red = 0;
    int count_blue = 0;
    int count_yellow = 0;
    int count_green = 0;
    int old_circle_color;/*前回の色保存用*/
    int oldCirclePoint;  // 前回の位置保存（自機の向き確認用）
    int nowCirclePoint;  // 現在の位置保存
    int nextCirclePoint; // 未来の位置保存
    int veryOldCirclePoint; // 本来の前回の位置保存
    bool backedFlag = false;
    bool blockMovingFlag = false;
    int blockMovedCount = 0;
    int blockColor = 0;
    int flag = 0;
    int midTrace = 150;
    Walker walker;
    Lifter lifter;
    Explorer explorer;
    ColorChecker colorChecker;
    PidWalker pidWalker;
    SonarSensor sonarSensor;
    ColorSensor colorSensor;
    Clock clock;
};

#endif

