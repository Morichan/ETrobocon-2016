#include "ev3api.h"
#include "ColorSensor.h"
#include "Clock.h"
#include "Puzzle.h"

#include "PidWalker.h"

//none:0 black:1 blue:2 green:3 yellow:4 red:5 white:6 brown:7


Puzzle::Puzzle():
  sonarSensor(PORT_3), colorSensor(PORT_2){}


  void Puzzle::doPuzzle(){
    pidWalker.pid.setPid(0.4, 0.0, 2.0, 5);
    pidWalker.setForward(20);


    while(1){

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

      //while(1){
      //pidWalker.stop();
      // }

      //    while(colorSensor.getColorNumber() != 1){
      //walker.run(20, 1);
      //}

      walker.angleChange(45, 1);



      //〇の線上、一回目
      //    while(colorSensor.getColorNumber() == currentCircleColor){
      //  msg_f("first", 3);
      //  walker.run(10, 7);
      //  if(colorSensor.getColorNumber() == 6) break; //白の領域に入ったら
      //}
      //〇の内円、白の領域
      while(colorSensor.getColorNumber() == 6){
        msg_f("second", 4);
        walker.run(10, 0);
        if(colorSensor.getColorNumber() == currentCircleColor) break;
      }
      //〇の線上、二回目
      while(colorSensor.getColorNumber() == currentCircleColor){
        msg_f("third", 5);
        walker.run(10, 7);
        if(colorSensor.getColorNumber() == 1) break; //黒線に入ったら
      }


    }
  }

void Puzzle::runOneBlock(){

}

//円の中にたどり着いたら90度曲がる
void Puzzle::turn90InCircle(){
  while(1) {}
}



