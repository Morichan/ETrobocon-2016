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
  pidWalker.setForward(15);
  int count=0,turn=0;
  while(1){

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
//////////////////左曲がり
    walker.angleChange(55,1);
    while(1){
      walker.run(15,0);
      if(colorSensor.getColorNumber() == 1) break;
    }
    walker.moveAngle(10,30);

    while(1){
      pidWalker.trace();
      if(colorSensor.getColorNumber() == 2) {
        break;
      }
      else if(colorSensor.getColorNumber() == 3) {
        break;
      }
      else if(colorSensor.getColorNumber() == 4) {
        break;
      }
      else if(colorSensor.getColorNumber() == 5) {
        break;
      }
    }
///////////////////////////
/////////////////////まっすぐ
    walker.moveAngle(5,20);
    walker.angleChange(10,-1);
    while(1){
      walker.run(15,turn);
      count++;
      if(count>=500){
        turn=-2;
      }
      if(colorSensor.getColorNumber() == 1) break;
    }




    while(1){
      pidWalker.trace();
      if(colorSensor.getColorNumber() == 2) {
        break;
      }
      else if(colorSensor.getColorNumber() == 3) {
        break;
      }
      else if(colorSensor.getColorNumber() == 4) {
        break;
      }
      else if(colorSensor.getColorNumber() == 5) {
        break;
      }
    }

///////////////////////////////////
//////////////////////左曲がり
    walker.angleChange(55,1);
    while(1){
      walker.run(15,0);
      if(colorSensor.getColorNumber() == 1) break;
    }
    walker.moveAngle(10,30);



    while(1){
      pidWalker.trace();
      if(colorSensor.getColorNumber() == 2) {
        break;
      }
      else if(colorSensor.getColorNumber() == 3) {
        break;
      }
      else if(colorSensor.getColorNumber() == 4) {
        break;
      }
      else if(colorSensor.getColorNumber() == 5) {
        break;
      }
    }

//////////////////////////////////////
//////////////////////右曲がり
    walker.angleChange(45,-1);

      while(1){
        walker.run(15,0);
        if(colorSensor.getColorNumber() == 1) break;
      }

    walker.angleChange(30,-1);


    while(1){
      pidWalker.trace();
      if(colorSensor.getColorNumber() == 2) {
        break;
      }
      else if(colorSensor.getColorNumber() == 3) {
        break;
      }
      else if(colorSensor.getColorNumber() == 4) {
        break;
      }
      else if(colorSensor.getColorNumber() == 5) {
        break;
      }
    }
///////////////////////////////////
///////////////////////Ｕターン
    walker.moveAngle(10,20);
    walker.angleChange(90,1);

    walker.moveAngle(10,40);

    while(1){
      walker.run(0,-10);
      if(colorSensor.getColorNumber() == 1) break;
    }


    while(1){
      pidWalker.trace();
      if(colorSensor.getColorNumber() == 2) {
        break;
      }
      else if(colorSensor.getColorNumber() == 3) {
        break;
      }
      else if(colorSensor.getColorNumber() == 4) {
        break;
      }
      else if(colorSensor.getColorNumber() == 5) {
        break;
      }
    }
/////////////////////////
//////////////////////右下
    walker.angleChange(120,-1);

    while(1){
      walker.run(15,0);
      if(colorSensor.getColorNumber() == 1) break;
    }

    walker.angleChange(60,-1);

    while(1){
      pidWalker.trace();
      if(colorSensor.getColorNumber() == 2) {
        break;
      }
      else if(colorSensor.getColorNumber() == 3) {
        break;
      }
      else if(colorSensor.getColorNumber() == 4) {
        break;
      }
      else if(colorSensor.getColorNumber() == 5) {
        break;
      }
    }
////////////////////////////
//////////////////////
///////////////////////Ｕターン
    walker.moveAngle(10,20);
    walker.angleChange(90,1);

    walker.moveAngle(10,40);

    while(1){
      walker.run(0,-10);
      if(colorSensor.getColorNumber() == 1) break;
    }


    while(1){
      pidWalker.trace();
      if(colorSensor.getColorNumber() == 2) {
        break;
      }
      else if(colorSensor.getColorNumber() == 3) {
        break;
      }
      else if(colorSensor.getColorNumber() == 4) {
        break;
      }
      else if(colorSensor.getColorNumber() == 5) {
        break;
      }
    }
/////////////////////////
/////////////////////右上
//    walker.angleChange(30,-1);
//    walker.moveAngle(20,180);
//
//    while(1){
//      walker.run(15,0);
//      if(colorSensor.getColorNumber() == 1) break;
//    }
//    
//    walker.angleChange(45,-1);
//
//    while(1){
//      pidWalker.trace();
//      if(colorSensor.getColorNumber() == 2) {
//        break;
//      }
//      else if(colorSensor.getColorNumber() == 3) {
//        break;
//      }
//      else if(colorSensor.getColorNumber() == 4) {
//        break;
//      }
//      else if(colorSensor.getColorNumber() == 5) {
//        break;
//      }
//    }
///////////////////////////
/////////////////////左上
//    walker.angleChange(20,1);
//    walker.moveAngle(20,180);
//
//    while(1){
//      walker.run(15,0);
//      if(colorSensor.getColorNumber() == 1) break;
//    }
//    
//    walker.moveAngle(10,40);
//    walker.angleChange(45,1);
//
//    while(1){
//      pidWalker.trace();
//      if(colorSensor.getColorNumber() == 2) {
//        break;
//      }
//      else if(colorSensor.getColorNumber() == 3) {
//        break;
//      }
//      else if(colorSensor.getColorNumber() == 4) {
//        break;
//      }
//      else if(colorSensor.getColorNumber() == 5) {
//        break;
//      }
//    }
///////////////////////////
//////////////////////左下
    walker.angleChange(100,1);

    while(1){
      walker.run(15,0);
      if(colorSensor.getColorNumber() == 1) break;
    }

    walker.moveAngle(10,40);

    walker.angleChange(45,1);

    while(1){
      pidWalker.trace();
      if(colorSensor.getColorNumber() == 2) {
        break;
      }
      else if(colorSensor.getColorNumber() == 3) {
        break;
      }
      else if(colorSensor.getColorNumber() == 4) {
        break;
      }
      else if(colorSensor.getColorNumber() == 5) {
        break;
      }
    }
///////////////////////////

    while(1){
      pidWalker.stop();
    }
    // while(1){
    //     pidWalker.stop();
    // }

    // while(colorSensor.getColorNumber() != 1){
    //     walker.run(20, 1);
    // }

    //if(currentCircleColor == 5) {
    //    walker.angleChange(45, 1);
    //   break;
    //} else {
    //    break;
    //}
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



