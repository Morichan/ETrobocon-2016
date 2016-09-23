#include "ColorChecker.h"

ColorChecker::ColorChecker():
  colorSensor(PORT_2), lifter() {
  
  
  }

void ColorChecker::init(){
  init_f("ColorChecker");
}

void ColorChecker::checkBlockColor() {
  int angle = 65,flag=1,count=0;
  walker.reset();
  walker.run(-10,0);


  while(1){
    if(walker.get_count_L() <= -50 && walker.get_count_R() <= -50){
      break;
    }
  }

  walker.reset();
  while(1){

    lifter.changeDefault(angle);
    if(colorSensor.getColorNumber() == 2){
      color_id = 2;
      msg_f("blue",1);
      break;
    }else if(colorSensor.getColorNumber() == 3){
      color_id = 3;
      msg_f("green",1);
      break;
    }else if(colorSensor.getColorNumber() == 4){
      color_id = 4;
      msg_f("yellow",1);
      break;
    }else if(colorSensor.getColorNumber() == 5){
      color_id = 5;
      msg_f("red",1);
      break;
    }else if(colorSensor.getColorNumber() == 0){
      color_id =1;
      msg_f("black",1);
      break;
    }else if(colorSensor.getColorNumber() != 0){
      while(1){
        angle=-5;
        lifter.changeDefault(angle);
        clock.sleep(200);
        count++;
        if(count>3){
          msg_f("red",1);
          color_id = 5;
          break;
        }else if (colorSensor.getColorNumber() ==0){
          msg_f("error",1);
          break;
        }
      }
      break;
    }
  }
  if(count>0){
    lifter.changeDefault(-65+count*5);
  }else{
    lifter.changeDefault(-65);
  }
  walker.reset();
  walker.run(-10,0);
  while(1){
    if(walker.get_count_L() <= -90 && walker.get_count_R() <= -90){
      break;
    }
  }
  pidWalker.setForward(20);
  pidWalker.pid.setPid(0.5,0.0,2.0,10);
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
  walker.reset();
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
  pidWalker.setForward(15);
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
