#include"Pedestrian.h"

Pedestrian::Pedestrian():
    sonarSensor(PORT_3),colorSensor(PORT_2),touchSensor(PORT_1){

    }

void Pedestrian::init(){
    init_f("Pedestrian");
}


void Pedestrian::monitor(){
    int sleep_time=0;
    while(1){
        if(sonarSensor.getDistance()<=40){
            flag=1;
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
    // FILE *tp;
    // int count=0;
    // tp=fopen("gyro.txt","w");
    // walker.reset();
    pidWalker.trace();

    // lifter.liftUp();
    walker.moveAngle(20,380);

    // walker.angleChange(90,1);

    // pidWalker.trace();
    // push();

    // lifter.defaultSet(0);


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
        if(walker.get_count_L() <= -200 && walker.get_count_R() <= -200) break;
        clock.sleep(4);
    }

    walker.reset();
}


void Pedestrian::sumou(int8_t target_color){
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

    // FILE *tp;
    // int color=0;
    // tp=fopen("color.txt","w");




    msg_f("sumou",1);
    if(target_color == 2 || target_color == 5){
        if(target_color == 5){
            walker.angleChange(135,1);
            walker.edgeChange();
            while(1){
                msg_f("search blue",2);
                pidWalker.trace();
                if(colorSensor.getColorNumber() == 2 ){
                    push();
                    break;
                }
            }
            walker.edgeChange();
            while(1){
                pidWalker.trace();
                if(colorSensor.getColorNumber() == 3 ){
                    push();
                    break;
                }
            }
            walker.moveAngle(30,720);
            while(1){
                pidWalker.trace();
                if(colorSensor.getColorNumber() == 4 ){
                    push();
                    break;
                }
            }
        }else{

            walker.angleChange(90,1);

            // walker.angleChange(135,1);
            // walker.edgeChange();
            pidWalker.trace();
            push();

            walker.angleChange(160,1);

            // msg_f("search red",2);


            msg_f("search yellow",2);

            // walker.edgeChange();
            msg_f("searching",2);

            pidWalker.trace();

            push();

        }


        msg_f("search finished",2);
        walker.stop();
        // walker.edgeChange();
        // while(1){
        //     pidWalker.trace();
        //     if(colorSensor.getColorNumber() == 4 ){
        //         push();
        //         break;
        //     }
        // }
        // walker.move(30,720);
        // while(1){
        //     pidWalker.trace();
        //     if(colorSensor.getColorNumber() == 3 ){
        //         push();
        //         break;
        //     }
        // }

    }
}
