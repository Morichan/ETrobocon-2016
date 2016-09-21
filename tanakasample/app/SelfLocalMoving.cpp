#include "SelfLocalMoving.h"

SelfLocalMoving::SelfLocalMoving():
    sonarSensor(PORT_3),colorSensor(PORT_2){
}

void SelfLocalMoving::moveLCourseStart() {
    bool intoFirstCurve = false;
    bool outFirstCurve = false;
    bool intoSecondCurve = false;
    bool intoEdgeChangeCurve = false;
    bool outEdgeChangeCurve = false;


    pidWalker.accelerate(1, 25);

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
        /*intoFirstCurve = nearTarget(300, 0, 25, 0);
        outFirstCurve = nearTarget(310, -50, 25, 1);
        intoSecondCurve = nearTarget(180, -65, 25, 2);
        intoEdgeChangeCurve = nearTarget(-180, -1, -1, 3);
        outEdgeChangeCurve = nearTarget(200, 1, 1, 4);
*/
        intoEdgeChangeCurve = nearTarget(11.5, -1, 1, 0);
       
        if(intoEdgeChangeCurve){
            ev3_speaker_play_tone(NOTE_E5, 20);
            walker.run(20,0);
            clock.sleep(600);

            while (colorSensor.getColorNumber()!= 1) {
                /*自己位置のデータ更新*/
                self_localization.update(edge_direction);
                /*①基準地の更新*/
                self_localization.standard_point(6);//基準値を6point離れるごとに更新
                
                walker.run(15,0);
                
                if (colorSensor.getColorNumber()==1) {
                    walker.angleChange(45,-1);
                    
                    break;
                    
                }
            }
            break;
            
        }
        /*
        if(intoFirstCurve) {
            ev3_speaker_play_tone(NOTE_E5, 20);
            pidWalker.pid.setPid(0.8, 0.0, 5.0, 30);
            pidWalker.brake(0, 30);
        }

        if(outFirstCurve) {
            ev3_speaker_play_tone(NOTE_E5, 20);
            pidWalker.accelerate(0, 70);
            pidWalker.pid.setPid(1.0, 0.0, 5.0, 30);
        }

        if(intoSecondCurve) {
            ev3_speaker_play_tone(NOTE_E5, 20);
            pidWalker.pid.setPid(0.5, 0.0, 2.0, 30);
            pidWalker.brake(0, 45);
        }

        if(intoEdgeChangeCurve) {
            ev3_speaker_play_tone(NOTE_E5, 20);
            pidWalker.pid.setPid(0.5, 0.0, 0.8, 30);
            pidWalker.brake(0, 10);
            pidWalker.walker.edgeChange();
            edge_direction = -1;
            pidWalker.accelerate(0, 40);
            pidWalker.pid.setPid(1.0, 0.0, 5.0, 30);
        }

        if(outEdgeChangeCurve) {
            ev3_speaker_play_tone(NOTE_E5, 20);
            pidWalker.pid.setPid(0.5, 0.0, 0.8, 30);
            pidWalker.brake(0, 10);
            pidWalker.walker.edgeChange();
            edge_direction = 1;
            pidWalker.accelerate(0, 40);
            pidWalker.pid.setPid(1.0, 0.0, 2.0, 30);
            break;
        }
         */
        self_localization.writing_current_coordinates(fp);
        self_localization.writing_angle(fp4);

    }

    fclose(fp);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
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

