#include "SelfLocalMoving.h"

SelfLocalMoving::SelfLocalMoving():
    sonarSensor(PORT_3) {
}

void SelfLocalMoving::moveLCourseStart() {
    // FILE* fp  = fopen("speed.txt", "w");
    // FILE* fp2 = fopen("direction.txt", "w");
    // FILE* fp3 = fopen("standard.txt", "w");
    // FILE* fp4 = fopen("angle.txt", "w");

    bool intoFirstCurve = false;
    bool outFirstCurve = false;
    bool intoSecondCurve = false;
    bool intoEdgeChangeCurve = false;


    pidWalker.accelerate(1, 70);

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
        intoFirstCurve = nearTarget(300, 0, 25, 0);
        outFirstCurve = nearTarget(310, -50, 25, 1);
        intoSecondCurve = nearTarget(170, -65, 25, 2);
        intoEdgeChangeCurve = nearTarget(125, -270, 25, 3);

        if(intoFirstCurve) {
            ev3_speaker_play_tone(NOTE_E5, 20);
            pidWalker.brake(0, 30);
        }

        if(outFirstCurve) {
            ev3_speaker_play_tone(NOTE_E5, 20);
            pidWalker.accelerate(0, 60);
        }

        if(intoSecondCurve) {
            ev3_speaker_play_tone(NOTE_E5, 20);
            pidWalker.brake(0, 45);
        }

        if(intoEdgeChangeCurve) {
            ev3_speaker_play_tone(NOTE_E5, 20);
            self_localization.update(edge_direction);

            self_localization.standard_point(6);
            pidWalker.stop();
            break;
        }

        self_localization.writing_current_coordinates(fp);
        self_localization.writing_angle(fp4);

    }

    fclose(fp);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
}

bool SelfLocalMoving::nearTarget(int _x, int _y, int _r, int _flag) {
    bool flag_near_target;

    if(self_localization.near_target_coordinates(_x, _y, _r, 0) == 1 &&
            _flag == flag_edge) {
        flag_near_target = true;
        flag_edge++;
    } else {
        flag_near_target = false;
    }

    return flag_near_target;
}

