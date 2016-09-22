#include "ev3api.h"
#include "ColorSensor.h"
#include "Clock.h"
#include "Puzzle.h"

#include "PidWalker.h"

//none:0 black:1 blue:2 green:3 yellow:4 red:5 white:6 brown:7

Puzzle::Puzzle():
    sonarSensor(PORT_3), colorSensor(PORT_2){
}

void Puzzle::doPuzzle(){
    pidWalker.walker.edgeChange();
    pidWalker.pid.setPid(0.4, 0.0, 2.0, 30);
    pidWalker.setForward(20);

    /*
     * 色を検知したら色の値をとって次に進む
     * 1 = 黒, 2 = 青, 3 = 緑, 4 = 黄, 5 = 赤, 6 = 白
     */
    goAheadNode();

    oldCirclePoint = 1;
    nowCirclePoint = 0;

    /*
     * explorerの基本の使い方
     */
    // スタート＆ゴールの設定
    // explorer->set(0, 10);
    // ブロックの位置指定
    // explorer->setBlocks(1,3,6,8);
    // 探索開始 vector内に経路が入ってきます(vectorは動的リスト)
    // vector<int>root = explorer->search();

    for(int i = 0; i < 1; ++i) {
        int rootCount = 0;
        std::vector<int> root = {1, 5, 8 , 4, 9, 10, 15, 10, 9, 4, 8, 5, 1, 0};

        for(std::size_t j = 0, rootSize = root.size(); j < rootSize; ++j) {
            nextCirclePoint = root[rootCount];

            goNextPoint();
            goAheadNode();

            rootCount++;
        }
    }
}

void Puzzle::goNextPoint() {
    int nowPoint  = nowCirclePoint  - oldCirclePoint;
    int nextPoint = nextCirclePoint - nowCirclePoint;

    /*
     * 12 ------ 8 ------ 4 ------ 0
     *  |        |        |        |
     *  |  blue  |        |   red  |
     *  |        |        |        |
     * 13 ------ 9 ------ 5 ------ 1
     *  |        |        |        |
     *  |        |        |        |   ---
     *  |        |        |        |      |
     * 14 ----- 10 ------ 6 ------ 2      |
     *  |        |        |        |      |
     *  | green  |        | yellow |      |
     *  |        |        |        |      |
     * 15 ----- 11 ------ 7 ------ 3    start
     *
     * nowPointで向きを調べる
     * nextPointで動きを調べる
     */

    // （上の図から見た）左向き
    if(nowPoint == 4) {
        if(nextPoint == 4) {         // （上の図から見た）左へ移動
            goFrontEdge();
        } else if(nextPoint == -4) { // 右へ移動
            goBackEdge();
        } else if(nextPoint == 1) {  // 下へ移動
            goLeftEdge();
        } else if(nextPoint == -1) { // 上へ移動
            goRightEdge();
        } else if(nextPoint == 5) {  // 左下へ移動
            goUpLeftEdge();
        } else if(nextPoint == 3) {  // 左上へ移動
            goUpRightEdge();
        } else if(nextPoint == -5) { // 右上へ移動
            goLowRightEdge();
        } else if(nextPoint == -3) { // 右下へ移動
            goLowLeftEdge();
        }

        // 右向き
    } else if(nowPoint == -4) {
        if(nextPoint == 4) {         // 左へ移動
            goBackEdge();
        } else if(nextPoint == -4) { // 右へ移動
            goFrontEdge();
        } else if(nextPoint == 1) {  // 下へ移動
            goRightEdge();
        } else if(nextPoint == -1) { // 上へ移動
            goLeftEdge();
        } else if(nextPoint == 5) {  // 左下へ移動
            goLowRightEdge();
        } else if(nextPoint == 3) {  // 左上へ移動
            goLowLeftEdge();
        } else if(nextPoint == -5) { // 右上へ移動
            goUpLeftEdge();
        } else if(nextPoint == -3) { // 右下へ移動
            goUpRightEdge();
        }

        // 下向き
    } else if(nowPoint == 1) {
        if(nextPoint == 4) {         // 左へ移動
            goRightEdge();
        } else if(nextPoint == -4) { // 右へ移動
            goLeftEdge();
        } else if(nextPoint == 1) {  // 下へ移動
            goFrontEdge();
        } else if(nextPoint == -1) { // 上へ移動
            goBackEdge();
        } else if(nextPoint == 5) {  // 左下へ移動
            goUpRightEdge();
        } else if(nextPoint == 3) {  // 左上へ移動
            goLowRightEdge();
        } else if(nextPoint == -5) { // 右上へ移動
            goLowLeftEdge();
        } else if(nextPoint == -3) { // 右下へ移動
            goUpLeftEdge();
        }

        // 上向き
    } else if(nowPoint == -1) {
        if(nextPoint == 4) {         // 左へ移動
            goLeftEdge();
        } else if(nextPoint == -4) { // 右へ移動
            goRightEdge();
        } else if(nextPoint == 1) {  // 下へ移動
            goBackEdge();
        } else if(nextPoint == -1) { // 上へ移動
            goFrontEdge();
        } else if(nextPoint == 5) {  // 左下へ移動
            goLowLeftEdge();
        } else if(nextPoint == 3) {  // 左上へ移動
            goUpLeftEdge();
        } else if(nextPoint == -5) { // 右上へ移動
            goUpRightEdge();
        } else if(nextPoint == -3) { // 右下へ移動
            goLowRightEdge();
        }
    }

    if(nextPoint % 4 == 0 || nextPoint == 1 || nextPoint == -1) {
        oldCirclePoint = nowCirclePoint;
    } else {
        if(nowPoint % 4 == 0 && nextPoint > 0) {
            oldCirclePoint = nowCirclePoint + nextPoint - 4;
        } else if(nowPoint % 4 == 0 && nextPoint < 0) {
            oldCirclePoint = nowCirclePoint + nextPoint + 4;
        } else if((nowPoint + 1) % 2 == 0 && nextPoint > 0) {
            oldCirclePoint = nowCirclePoint + 4;
        } else if((nowPoint + 1) % 2 == 0 && nextPoint < 0) {
            oldCirclePoint = nowCirclePoint - 4;
        }
    }
    nowCirclePoint = nextCirclePoint;

}

void Puzzle::goAheadNode() {
    //黒線
    while(1){
        pidWalker.trace();
        if(colorSensor.getColorNumber() == 2) {
            currentCircleColor = 2;
            break;
        }
        else if(colorSensor.getColorNumber() == 3) {
            currentCircleColor = 3;
            break;
        }
        else if(colorSensor.getColorNumber() == 4) {
            currentCircleColor = 4;
            break;
        }
        else if(colorSensor.getColorNumber() == 5) {
            currentCircleColor = 5;
            break;
        }
    }
}

void Puzzle::goLeftEdge() {
    pidWalker.walker.angleChange(45, 1);
    while (colorSensor.getColorNumber() != 1) {
        pidWalker.walker.run(20,0);
    }
    pidWalker.walker.moveAngle(20, 100);
    pidWalker.walker.angleChange(45, 1);
}

void Puzzle::goRightEdge() {
    pidWalker.walker.angleChange(45, -1);
    while (colorSensor.getColorNumber() != 1) {
        pidWalker.walker.run(20,0);
    }
    pidWalker.walker.moveAngle(20, 50);
    pidWalker.walker.angleChange(45, -1);
}

void Puzzle::goFrontEdge() {
    pidWalker.walker.moveAngle(20, 150);
}

void Puzzle::goBackEdge() {
    pidWalker.walker.moveAngle(20, 20);
    pidWalker.walker.angleChange(90, 1);
    pidWalker.walker.moveAngle(20, 40);
    pidWalker.walker.angleChange(45, 1);
    pidWalker.walker.moveAngle(10, 20);
    while(colorSensor.getColorNumber() != 1) {
        pidWalker.walker.run(0, 10);
    }
    pidWalker.walker.moveAngle(20, 60);
    pidWalker.walker.angleChange(30, -1);
}

void Puzzle::goLowRightEdge() {
    int32_t i;
    goRightEdge();
    for(i = 0; i < 200; i++) {
        pidWalker.trace();
    }
    pidWalker.walker.angleChange(45, -1);
    while(colorSensor.getColorNumber() != 1) {
        pidWalker.walker.run(20, 0);
    }
    pidWalker.walker.moveAngle(20, 50);
    pidWalker.walker.angleChange(45, -1);
    pidWalker.accelerate(1, 20);
}

void Puzzle::goUpRightEdge() {
    int32_t i;
    goRightEdge();
    for(i = 0; i < 200; i++) {
        pidWalker.trace();
    }
    pidWalker.walker.angleChange(45, 1);
    while(colorSensor.getColorNumber() != 1) {
        pidWalker.walker.run(20, 0);
    }
    pidWalker.walker.moveAngle(20, 100);
    pidWalker.walker.angleChange(45, 1);
    pidWalker.accelerate(1, 20);
}

void Puzzle::goLowLeftEdge() {
    int32_t i;
    goLeftEdge();
    for(i = 0; i < 200; i++) {
        pidWalker.trace();
    }
    pidWalker.walker.angleChange(45, 1);
    while(colorSensor.getColorNumber() != 1) {
        pidWalker.walker.run(20, 0);
    }
    pidWalker.walker.moveAngle(20, 100);
    pidWalker.walker.angleChange(45, 1);
    pidWalker.accelerate(1, 20);
}

void Puzzle::goUpLeftEdge() {
    int32_t i;
    goLeftEdge();
    for(i = 0; i < 200; i++) {
        pidWalker.trace();
    }
    pidWalker.walker.angleChange(45, -1);
    while(colorSensor.getColorNumber() != 1) {
        pidWalker.walker.run(20, 0);
    }
    pidWalker.walker.moveAngle(20, 50);
    pidWalker.walker.angleChange(45, -1);
    pidWalker.accelerate(1, 20);
}

void Puzzle::runOneBlock(){
    //黒線
    while(1){
        pidWalker.trace();
        if(colorSensor.getColorNumber() == 2) {
            currentCircleColor = 2;
            break;
        }
        else if(colorSensor.getColorNumber() == 3) {
            currentCircleColor = 3;
            break;
        }
        else if(colorSensor.getColorNumber() == 4) {
            currentCircleColor = 4;
            break;
        }
        else if(colorSensor.getColorNumber() == 5) {
            currentCircleColor = 5;
            break;
        }
    }

    if(currentCircleColor == 2){
        walker.run(10,0);
        clock.sleep(300);
        count_blue = count_blue + 1;
    }
    if(currentCircleColor == 3){
        walker.run(10,0);
        clock.sleep(300);
        count_green = count_green + 1;
    }
    if(currentCircleColor == 4){
        walker.run(10,0);
        clock.sleep(300);
        count_yellow = count_yellow + 1;
    }

    if(currentCircleColor == 5){
        walker.run(10,0);
        clock.sleep(300);
        count_red = count_red + 1;
    }

    if(count_blue == 2){
        while (colorSensor.getColorNumber() != 1) {
            walker.run(20,0);
        }
    }

    //〇の内円、白の領域
    while(colorSensor.getColorNumber() == 6){
        msg_f("second", 4);
        walker.run(10, 0);
        if(colorSensor.getColorNumber() == currentCircleColor)
            break;
    }
    //〇の線上、二回目
    while(colorSensor.getColorNumber() == currentCircleColor){
        msg_f("third", 5);
        walker.run(10, 7);
        if(colorSensor.getColorNumber() == 1)
            break; //黒線に入ったら
    }
    clock.sleep(800);

}

//円の中にたどり着いたら90度曲がる
void Puzzle::turn90InCircle(){
    while(1) {}
}



