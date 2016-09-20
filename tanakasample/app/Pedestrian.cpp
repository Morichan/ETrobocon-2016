#include"Pedestrian.h"

Pedestrian::Pedestrian():
  sonarSensor(PORT_3),colorSensor(PORT_2){
  }

void Pedestrian::init(){
  init_f("Pedestrian");
}


void Pedestrian::monitor(){
  int sleep_time=0;
  walker.reset();
  while(1){
    if(sonarSensor.getDistance()<=40){
      Distance=sonarSensor.getDistance();
      while(sonarSensor.getDistance()<=40){
      }
      sleep_time=1000-20*Distance;
      clock.sleep(sleep_time);
      break;
    }
  }
}

void Pedestrian::cross(){
  pidWalker.setForward(10);
  moveColor();
  walker.moveAngle(20,380);
  lifter.defaultSet(0);
}

void Pedestrian::push(){

  walker.reset();
  walker.run(20, 0);

  while(1){
    if(walker.get_count_L() >= 180 && walker.get_count_R() >= 180 ) break;
    clock.sleep(4);
  }

  walker.reset();
  clock.sleep(200);
  walker.run(-20, 0);

  while(1){
    if(walker.get_count_L() <= 130 && walker.get_count_R() <= -130) break;
    clock.sleep(4);
  }
  walker.reset();
}

void Pedestrian::moveColor(){
  while(1){
    pidWalker.trace();
    if(colorSensor.getColorNumber() == 2) break;
    if(colorSensor.getColorNumber() == 3) break;
    if(colorSensor.getColorNumber() == 4) break;
    if(colorSensor.getColorNumber() == 5) break;
  }
}

void Pedestrian::moveCross(){
  while(1){
    pidWalker.trace();
    if(colorSensor.getBrightness() <=8 ) break;
  }
}

void Pedestrian::turnLine(int8_t direction){
  while(1){
    walker.run(0,10*direction);
    if(colorSensor.getBrightness() <= 40) break;
  }
}

void Pedestrian::turnColor(){
  while(1){
    walker.run(0,10);
    if(colorSensor.getColorNumber() == 1){
      walker.moveAngle(10,20);
      break;
    }
  }
}







//-----------------------------------------------------
// blue=2 green=3 yellow=4 red=5
//       E
//       |
// 4------------3
//       |
//       |
// 5------------2
//       |
//       S
//-----------------------------------------------------

void Pedestrian::sumou(int8_t target_color){
  msg_f("ETsumou",1);
  if(target_color == 2 || target_color == 5){
    if(target_color == 2){
      msg_f("search red",2);
      walker.angleChange(90,1);

      moveColor();
      push();
      turnColor();
      moveCross();

      walker.moveAngle(10,60);
      walker.angleChange(45,1);
      turnLine(1);


    }else if(target_color == 5){
      msg_f("search blue",5);







    }


    msg_f("search green",2);
    moveCross();
    walker.moveAngle(10,40);
    walker.angleChange(135,-1);
    turnLine(1);

    moveColor();
    push();

    msg_f("search yellow",2);

    turnColor();
    moveCross();
    walker.moveAngle(10,20);

    moveColor();
    push();


    turnColor();
    moveCross();

    walker.moveAngle(10,60);
    walker.angleChange(45,1);
    turnLine(1);



  }else if(target_color == 3 || target_color == 4){
    msg_f("search red",2);
    walker.angleChange(90,1);
    moveColor();
    push();

    msg_f("search blue",2);
    turnColor();
    moveCross();
    walker.moveAngle(10,20);

    moveColor();
    push();
    
    turnLine(1);
    walker.moveAngle(10,20);
    moveCross();
    walker.moveAngle(10,40);
    walker.angleChange(135,-1);

    turnLine(1);

    moveCross();

    if(target_color == 4){
      msg_f("search green",2);
      walker.moveAngle(10,40);
      walker.angleChange(135,-1);
      turnColor();
      moveColor();
      push();


      turnColor();
      moveCross();
      walker.moveAngle(10,40);
      walker.angleChange(135,-1);
      turnLine(1);

    }else if(target_color == 3){
      msg_f("search yellow",2);
      walker.moveAngle(10,40);
      walker.angleChange(45,1);
      turnLine(1);
      moveColor();
      push();

      turnColor();
      moveCross();

      walker.moveAngle(10,60);
      walker.angleChange(45,1);
      turnLine(1);
    }
  walker.reset();
  walker.run(-20, 0);
  while(1){
    if(walker.get_count_L() <= -180 && walker.get_count_R() <= -180) break;
    clock.sleep(4);
  }
  walker.reset();
    monitor();
    walker.moveAngle(20,180);
    pidWalker.setForward(25);
    walker.moveAngle(20,270);
    while(1){
      pidWalker.trace();
    }
  }
  msg_f("search finished",2);
  walker.stop();
}

