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
  Distance=254;
  while(1){
    if(sonarSensor.getDistance()<=50){
      Distance=sonarSensor.getDistance();
      sleep_time=1000-20*Distance;
      clock.sleep(sleep_time);
      break;
    }
  }
}

void Pedestrian::cross(int8_t t_color){
  pidWalker.setForward(20);
  pidWalker.pid.setPid(0.5, 0.0, 2.0, 30);
  moveColor();
  if(t_color==5){
    walker.moveAngle(30,180);
    walker.angleChange(25,-1);
    pidWalker.setForward(30);
    lifter.defaultSet(0);
    moveCross();

    walker.moveAngle(20,50);
    walker.angleChange(120,-1);
    turnColor(1);
  }else{
    walker.moveAngle(25,380);
    lifter.defaultSet(0);
  }
  pidWalker.setForward(10);
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
    if(walker.get_count_L() <= -130 && walker.get_count_R() <= -130) break;
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
    walker.run(0,15*direction);
    if(colorSensor.getBrightness() <= 40) break;
  }
}

void Pedestrian::turnColor(int8_t way){
  while(1){
    walker.run(0,20*way);
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
  int count=0,turn=0;;
  msg_f("ETsumou",1);

  pidWalker.pid.setPid(0.5, 0.0, 2.0, 30);
  if(target_color == 2 || target_color == 5){
    if(target_color == 2){
      msg_f("search red",2);
      walker.angleChange(90,1);

      moveColor();
      push();
      turnColor(1);
      pidWalker.setForward(10);
      moveCross();
      pidWalker.setForward(15);
      walker.moveAngle(15,70);
      walker.angleChange(45,1);
      turnLine(1);
      pidWalker.setForward(15);

    }else if(target_color == 5){
      msg_f("search blue",5);
      moveColor();
      push();
      turnColor(-1);
      walker.angleChange(25,-1);
      pidWalker.setForward(10);
      moveCross();
      walker.angleChange(85,-1);
      while(1){
        walker.run(15,turn);
        count++;
        if(count>=100){
          turn=1;
        }
        if(colorSensor.getBrightness() <=40) break;
      }
      pidWalker.setForward(10);
    }

    msg_f("search green",2);
    moveCross();
    walker.angleChange(80,-1);
    //    walker.moveAngle(20,40);
    //    walker.angleChange(120,-1);
    //    turnLine(1);

    moveColor();
    push();

    msg_f("search yellow",2);

    turnColor(1);
    pidWalker.setForward(20);
    moveCross();
    walker.moveAngle(20,20);

    moveColor();
    push();


    turnColor(-1);
    walker.angleChange(25,-1);

    pidWalker.setForward(10);
    moveCross();

    walker.moveAngle(20,70);



  }else if(target_color == 3 || target_color == 4){
    msg_f("search red",2);
    walker.angleChange(90,1);
    moveColor();
    push();

    msg_f("search blue",2);
    turnColor(1);
    pidWalker.setForward(20);
    moveCross();
    walker.moveAngle(20,20);

    moveColor();
    push();
    turnColor(-1);
    walker.angleChange(25,-1);
    pidWalker.setForward(10);
    moveCross();
    walker.angleChange(85,-1);
    while(1){
      count++;
      walker.run(15,turn);
      if(count>=100){
        turn=1;
      }
      if(colorSensor.getBrightness() <=40) break;
    }


    pidWalker.setForward(10);
    moveCross();
    if(target_color == 4){
      msg_f("search green",2);
      walker.angleChange(80,-1);
      //      walker.moveAngle(20,60);
      //      walker.angleChange(110,-1);
      //      turnColor(1);
      moveColor();
      push();

      pidWalker.setForward(10);
      turnColor(1);
      moveCross();
      walker.moveAngle(20,50);
      walker.angleChange(135,-1);
      turnLine(-1);


    }else if(target_color == 3){
      msg_f("search yellow",2);
      walker.moveAngle(20,40);
      walker.angleChange(45,1);
      turnLine(1);
      moveColor();
      push();

      turnColor(-1);
      walker.angleChange(25,-1);
      pidWalker.setForward(10);
      moveCross();

      walker.moveAngle(20,60);
    }
  }

  walker.stop();
  monitor();

  clock.sleep(1500);
  pidWalker.setForward(10);
  walker.angleChange(45,1);
  turnLine(1);
  if(sonarSensor.getDistance() <=30){
    clock.sleep(500);
  }
  moveColor();
  walker.angleChange(10,-1);
  walker.moveAngle(30,360);
  while(1){
    walker.run(30,0);
    if(colorSensor.getColorNumber() == 1) break;
  }
  walker.angleChange(30,-1);
  pidWalker.setForward(5);
  count=0;
  while(1){
    pidWalker.trace();
    count++;
    if(count>=300) break;
  }
  count=0;
  pidWalker.accelerate(0, 40);
  while(1){
    pidWalker.trace();
    count++;
    if(count>=800) break;
  }
  msg_f("search finished",2);
}

