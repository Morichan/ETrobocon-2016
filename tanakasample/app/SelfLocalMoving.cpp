#include "SelfLocalMoving.h"

SelfLocalMoving::SelfLocalMoving():
    sonarSensor(PORT_3),colorSensor(PORT_2){
}

void SelfLocalMoving::moveRCourseStart() {
    // bool intoFirstCurve = false;
    // bool outFirstCurve = false;
    // bool intoSecondCurve = false;
    bool intoEdgeChangeCurve = false;
    // bool outEdgeChangeCurve = false;


    pidWalker.pid.setPid(0.5, 0.0, 0.4, 30);
    pidWalker.accelerate(1, 20);

    while(1) {
        if(ev3_button_is_pressed(BACK_BUTTON)) {
            break;
        }

        pidWalker.trace();

        self_localization.update(edge_direction);
        self_localization.standard_point(6); // 基準点を6ぽいんと離れるごとに更新

        /*
         * 走行タスクの入力
         */
        /*
         * intoFirstCurve = nearTarget(300, 0, 25, 0);
         * outFirstCurve = nearTarget(310, -50, 25, 1);
         * intoSecondCurve = nearTarget(180, -65, 25, 2);
         * intoEdgeChangeCurve = nearTarget(-180, -1, -1, 3);
         * outEdgeChangeCurve = nearTarget(200, 1, 1, 4);
         */
        intoEdgeChangeCurve = nearTarget(8.0, -1, 1, 0);

        if(intoEdgeChangeCurve){
            break;
        }

        self_localization.writing_current_coordinates(fp);
        self_localization.writing_angle(fp4);

    }

    while(colorSensor.getColorNumber() != 6) {
        pidWalker.trace();
    }
    ev3_speaker_play_tone(NOTE_E5, 20);
    walker.run(25,0);
    clock.sleep(600);

    while(colorSensor.getColorNumber()!= 1) {
        /*自己位置のデータ更新*/
        self_localization.update(edge_direction);
        /*①基準地の更新*/
        self_localization.standard_point(6);//基準値を6point離れるごとに更新

        walker.run(25,0);
    }

    walker.angleChange(45,-1);
    walker.angleChange(25,-1);

    fclose(fp);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
}

void SelfLocalMoving::moveRCourseToGoal(int pattern,int puzzle_edge) {
    bool into_first_curve = false;
    bool out_first_curve = false;
    bool into_second_curve = false;
    bool out_second_curve = false;
    bool into_3rd_curve = false;
    bool out_3rd_curve = false;
    bool begin_short_cut = false;
    bool into_gate = false;
    bool last_straight = false;
    bool start_short_cut = false;
    bool stolen_mainline = false;
    bool air_action = false;
    bool stolen_2nd_curve = false;

       // bool intoEdgeChangeCurve = false;
   // bool outEdgeChangeCurve = false;
    pidWalker.accelerate(0, 50);
    pidWalker.pid.setPid(0.4,0,5,30);
    if(puzzle_edge == 1 && set_edge == 0){/*12から開始*/
        start_point = 150;
        set_edge = 1;
        self_localization.set_current_xy(1,start_point);
    }
    if(puzzle_edge == 2 && set_edge == 0){/*12と13の間で開始*/
        start_point = 125;
        set_edge = 1;
        self_localization.set_current_xy(1,start_point);
    }

    if(puzzle_edge == 3 && set_edge == 0){/*13から開始*/
        start_point = 100;
        set_edge = 1;
        self_localization.set_current_xy(1,start_point);
    }
    if(puzzle_edge == 4 && set_edge == 0){/*13と14の間から開始*/
        start_point = 75;
        set_edge = 1;
        self_localization.set_current_xy(1,start_point);
    }

    if(puzzle_edge == 5 && set_edge == 0){/*14から開始*/
        start_point = 50;
        set_edge = 1;
        self_localization.set_current_xy(1,start_point);
    }
    if(puzzle_edge == 6 && set_edge == 0){/*14と15の間から開始*/
        start_point = 25;
        set_edge = 1;
        self_localization.set_current_xy(1,start_point);
    }

    
    
    if(puzzle_edge == 7 && set_edge == 0){/*15から開始*/
        start_point = 0;
        set_edge = 1;
    }
    
    while(1) {
        if(ev3_button_is_pressed(BACK_BUTTON)) {
            break;
        }

        pidWalker.trace();

        self_localization.update(edge_direction);
        self_localization.standard_point(6); // 基準点を6ぽいんと離れるごとに更新

        //走行タスクの入力
/*--------------------------------------------------------------------------------------------------------------*/
        
        if (pattern == 0) {/*通常走行パターン*/
            into_first_curve = nearTarget(175, 1, 1, 0);//x:160以上
            out_first_curve = nearTarget(-30,-1,-1,1);//y:-20以下
            into_second_curve = nearTarget(-29,-1,1,2);//(250,0)近づいたら
            out_second_curve = nearTarget(-30,-1,-1,3);//(300,-45)近づいたら
            into_3rd_curve = nearTarget(-170, -1, -1, 4);//y:-180以下
            out_3rd_curve = nearTarget(245, 1, -1, 5);//
        }
        if(pattern == 1){/*大幅ショートカットパターン(完璧)*/
            into_first_curve = nearTarget(175, 1, 1, 0);
            out_first_curve = nearTarget(-30,-1,-1,1);
            start_short_cut = nearTarget(-105,-1,-1,2);
            
            begin_short_cut = nearTarget(-150,-1,-1,3);
            into_gate = nearTarget(220,1,1,4);
            last_straight = nearTarget(0,1,1,5);
            
        }
        if (pattern == 2) {/*LAP超えてショートカット*/
            into_first_curve = nearTarget(175, 1, 1, 0);
            out_first_curve = nearTarget(-30,-1,-1,1);
            stolen_mainline = nearTarget(-85,-1,-1,2);
            into_second_curve = nearTarget(220,1,1,3);
            out_second_curve = nearTarget(215,1,-1,4);
            into_3rd_curve = nearTarget(120, 1, -1, 5);
            out_3rd_curve = nearTarget(-180, -1, 1, 6);
        }
        if(pattern == 3){/*GOAL横のカーブ超えてからショートカット*/
            into_first_curve = nearTarget(175, 1, 1, 0);
            out_first_curve = nearTarget(-30,-1,-1,1);
            air_action = nearTarget(235,1,1,2);
            stolen_2nd_curve = nearTarget(-145,-1,1,3);
            into_3rd_curve = nearTarget(270, 1, 1, 4);
            out_3rd_curve = nearTarget(340, 1, 1, 5);

            
        }
        
        
        
        
/*--------------------------------------------------------------------------------------------------------------*/
        if(into_first_curve){
            ev3_speaker_play_tone(NOTE_E5, 20);
            pidWalker.accelerate(20,25);
            pidWalker.setForward(20);
            pidWalker.pid.setPid(1.005,0,5,30);
            
            
        }
        
        if (out_first_curve) {
            ev3_speaker_play_tone(NOTE_E5, 20);
            /*自己位置のデータ更新*/
            self_localization.update(edge_direction);
            pidWalker.accelerate(30, 50);
            pidWalker.pid.setPid(0.5,0,5,30);
        }
        
        if(into_second_curve){
                ev3_speaker_play_tone(NOTE_E5, 20);
            
            pidWalker.accelerate(1,25);
            pidWalker.pid.setPid(1.005,0,5,30);
            
        }

        if (out_second_curve) {
            ev3_speaker_play_tone(NOTE_E5, 20);
            /*自己位置のデータ更新*/
            self_localization.update(edge_direction);
            pidWalker.accelerate(25, 50);
            pidWalker.pid.setPid(0.6,0,5,30);
        }
       
        if(into_3rd_curve){
            ev3_speaker_play_tone(NOTE_E5, 20);
            pidWalker.setForward(40);
            pidWalker.pid.setPid(0.7,0,5,30);
            
        }
        
        if (out_3rd_curve) {
            ev3_speaker_play_tone(NOTE_E5, 20);
            /*自己位置のデータ更新*/
            self_localization.update(edge_direction);
            pidWalker.accelerate(40, 70);
            pidWalker.pid.setPid(0.4,0,5,30);
        }
/*--------------------------------------------------------------------------------------------------------------*/
        /*最短ショートカット❶*/
        if(start_short_cut){
            self_localization.update(edge_direction);
            pidWalker.pid.setPid(0.3,0,5,40);
            ev3_speaker_play_tone(NOTE_E5, 20);
            pidWalker.setForward(50);
            
            
            
            
            }
    
       
        
        /*最短ショートカット❷*/
        if (begin_short_cut) {
            //walker.angleChange(45,-1);
            ev3_speaker_play_tone(NOTE_E5, 20);
            pidWalker.pid.setPid(0.5,0,5,30);
            walker.run(30,0);
            clock.sleep(200);
         
            
            while (colorSensor.getColorNumber() != 1) {
                self_localization.update(edge_direction);
                walker.run(40,0);
              
            }
            
            if (colorSensor.getColorNumber() == 1) {
                ev3_speaker_play_tone(NOTE_E5, 20);
                
                pidWalker.walker.edgeChange();
                /*回転*/
                walker.angleChange(60,1);
                self_localization.reset();
                walker.run(10,0);
                clock.sleep(10);
                pidWalker.accelerate(10,50);
                            }
        }
        /*最短ショートカット❸*/
        if (into_gate) {
            ev3_speaker_play_tone(NOTE_E5, 20);
            while (colorSensor.getColorNumber() != 3) {
                walker.run(50,0);
            }
        }
        /*最短ショートカット❹*/
        if (last_straight) {
            ev3_speaker_play_tone(NOTE_E5, 20);

            walker.angleChange(185,-1);
            walker.moveAngle(40,600);
            while (colorSensor.getColorNumber() != 1) {
                walker.run(20,-2);
            }
            
            if(colorSensor.getColorNumber() == 1){
            pidWalker.accelerate(0,30);
            pidWalker.accelerate(30,60);
            pidWalker.pid.setPid(0.5,0,5,30);
            }
        }
/*--------------------------------------------------------------------------------------------------------------*/
        if (stolen_mainline) {
            ev3_speaker_play_tone(NOTE_E5, 20);
            walker.angleChange(90,1);
            self_localization.reset();
            
            while (colorSensor.getColorNumber() != 1) {
                walker.run(40,0);
                self_localization.update(edge_direction);
            }
            if (colorSensor.getColorNumber() == 1) {
                walker.run(20,0);
                self_localization.update(edge_direction);
                clock.sleep(200);
                walker.angleChange(65,1);
                self_localization.reset();
                self_localization.set_current_xy(0,0);
                walker.run(10,0);
                clock.sleep(200);
                pidWalker.accelerate(0,25);
                self_localization.update(edge_direction);
                pidWalker.setForward(35);
            }
            
        }
        
        if (stolen_2nd_curve) {
            //walker.angleChange(45,-1);
            ev3_speaker_play_tone(NOTE_E5, 20);
            walker.angleChange(90,-1);
            self_localization.reset();
            
            while (colorSensor.getColorNumber() != 1) {
                self_localization.update(edge_direction);
                walker.run(25,0);
                
            }
            
            if (colorSensor.getColorNumber() == 1) {
                ev3_speaker_play_tone(NOTE_E5, 20);
                /*エッジ切り替え*/
                pidWalker.walker.edgeChange();
                /*回転*/
                walker.angleChange(70,-1);
                self_localization.reset();
                walker.run(10,0);
                clock.sleep(100);
                pidWalker.accelerate(10,40);
            }
        }

        
        if (air_action) {
            
                ev3_speaker_play_tone(NOTE_E5, 20);
            
            
        }
        self_localization.writing_current_coordinates(fp);
        
    }

    fclose(fp);
}

bool SelfLocalMoving::nearTarget(int _x, int _y, int _r, int _flag) {
    bool flag_near_target = false;

    if(_r > 2) {
        if(self_localization.near_target_coordinates(_x, _y, _r, 0) == 1 &&
                _flag == flag_edge) {
            flag_near_target = true;
            flag_edge++;
        }
    } else {
        // 半径が小さいものはそもそも難しいから、ライン指定としておく
        if(self_localization.line_target_coordinates(_x, _y, _r) == 1 &&
                _flag == flag_edge) {
            flag_near_target = true;
            flag_edge++;
        }
    }

    return flag_near_target;
}

