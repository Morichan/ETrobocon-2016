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

    while(1){

        goAheadNode();

        // while(1){
        //     pidWalker.stop();
        // }

        // while(colorSensor.getColorNumber() != 1){
        //     walker.run(20, 1);
        // }

        if(currentCircleColor == 5) {
            if(flag < 1) {
                goLowLeftEdge();
                flag++;
            } else {
                goUpLeftEdge();
                flag++;
            }
        }
        if(currentCircleColor == 4) {
            if(flag < 3) {
                goUpRightEdge();
                flag++;
            } else if(flag == 3) {
                goLowRightEdge();
                flag++;
            }
        }
        if(currentCircleColor == 3) {
            if(flag < 3) {
                goUpRightEdge();
                flag++;
            } else if(flag == 3) {
                goLowRightEdge();
                flag++;
            } else {
                flag = 0;
                break;
            }
        }

        // while (colorSensor.getColorNumber() != 1) {
        //     walker.run(10,0);
        //     flag = 1;
        // }
        // if (flag == 1) {
        //     /*それぞれの色を検知したらの動作*/
        //     if(currentCircleColor == 2){
        //         count_blue = count_blue + 1;
        //     }
        //     if(currentCircleColor == 3){
        //         count_green = count_green + 1;
        //     }
        //     if(currentCircleColor == 4){
        //         count_yellow = count_yellow + 1;
        //     }
        //     if(currentCircleColor == 5 ){
        //         count_red = count_red + 1;
        //     }
        //     flag = 2;
        // }
        // clock.sleep(800);

        // if (flag == 2) {
        //     ev3_speaker_play_tone(NOTE_E5, 20);
        //     old_circle_color = currentCircleColor;
        //     break;
        // }
    }
}

void Puzzle::goAheadNode() {

    //黒線
    while(1){
        pidWalker.trace();

        /*
         * 色を検知したら色の値をとって次に進む
         * 1 = 黒, 2 = 青, 3 = 緑, 4 = 黄, 5 = 赤, 6 = 白
         */
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
    while(colorSensor.getColorNumber() != 1) {
        pidWalker.walker.run(0, 10);
    }
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



