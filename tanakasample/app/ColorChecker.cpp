#include "ColorChecker.h"

ColorChecker::ColorChecker():
  colorSensor(PORT_2), lifter() {
  }

void ColorChecker::checkBlockColor() {

  walker.reset();
  walker.run(-10,0);

  while(1){
    if(walker.get_count_L() <= -60 && walker.get_count_R() <= -60){
      break;
    }
  }
  walker.reset();

  lifter.changeDefault(75);

  walker.run(5,0);

  while(1){
    if(colorSensor.getColorNumber() == 2){
      color_id=2;
      break;
    }
    if(colorSensor.getColorNumber() == 3){
      color_id=3;
      break;
    }
    if(colorSensor.getColorNumber() == 4){
      color_id=4;
      break;
    }
    if(colorSensor.getColorNumber() == 5){
      color_id=5;
      break;
    }
  }
  walker.reset();

  lifter.changeDefault(-75);

  walker.reset();
  walker.run(-10,0);

  while(1){
    if(walker.get_count_L() <= -60 && walker.get_count_R() <= -60){
      break;
    }
  }
  walker.reset();
  //    if(blockColor == 2) {
  //        lifter.changeDefault(45);
  //    }
  //    lifter.changeDefault(-45);

}
void ColorChecker::hoshitori() {
  int count=0,turn=0;
  pidWalker.setForward(40);
  pidWalker.pid.setPid(0.5, 0.0, 2.0, 30);
  while(1){
    pidWalker.trace();
    if (colorSensor.getColorNumber() == 2){
      color_id = 2;
      break;
    }
    if (colorSensor.getColorNumber() == 3){
      color_id = 3;
      break;
    }
    if (colorSensor.getColorNumber() == 4){
      color_id = 4;
      break;
    }
    if (colorSensor.getColorNumber() == 5){
      color_id = 5;
      break;
    }
  }

  walker.angleChange(95,1);
  walker.reset();
  while(1){
    count++;
    walker.run(20,turn);
    if (count >= 1000){
       turn=2;
    }
    if (colorSensor.getBrightness() <= 40) break;
  }
  count=0;
  pidWalker.setForward(10);
  while(1){
    count++;
    pidWalker.trace();
    if (count >= 250) break;
  }
  pidWalker.setForward(30);
  while(1){
    pidWalker.trace();
    if(walker.get_count_L() >= 1500) break;
  }
  walker.reset();
}


int8_t ColorChecker::getColor() {
  return color_id;
}
