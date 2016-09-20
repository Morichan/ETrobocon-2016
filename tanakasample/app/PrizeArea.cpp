#include "PrizeArea.h"

PrizeArea::PrizeArea():
    sonarSensor(PORT_3) {
}

void PrizeArea::prizeMode() {
    pidWalker.pid.setPid(0.5, 0.0, 2.0, 30);
    pidWalker.accelerate(40, 70);

    while(1) {
        pidWalker.trace();

        self_l.update(1);
        self_l.standard_point(6); // 基準点を6ぽいんと離れるごとに更新

        if(ev3_button_is_pressed(BACK_BUTTON)) {
            break;
        }

        if(sonarSensor.getDistance() < 50) {
            pidWalker.brake(0, 10);
            if(sonarSensor.getDistance() < 10) {
                pidWalker.stop();
                emoter.defaultSet(0);
                self_l.update(1);
                self_l.writing_current_coordinates(fp);
                getPrize();
                self_l.update(1);
                self_l.writing_current_coordinates(fp);
                carryPrize();
                break;
            }
        }

        self_l.writing_current_coordinates(fp);

    }
    self_l.update(1);
    self_l.writing_current_coordinates(fp);

    fclose(fp);
}

void PrizeArea::getPrize() {
    lifter.liftDown();
    walker.run(20, 0);
    clock.sleep(800);
    walker.stop();
    lifter.liftUp();
    emoter.wipe(100, 3, 40);
}

void PrizeArea::carryPrize() {
    int32_t carryCount;
    int32_t defaultWheelL;

    // 90度回転する
    walker.angleChange(90, 1);
    // Goalをまっすぐ狙う
    defaultWheelL = walker.get_count_L();
    while(1) {
        self_l.update(1);
        self_l.writing_current_coordinates(fp);
        walker.run(80, 45);
        if(walker.get_count_L() - defaultWheelL > 700) {
            break;
        }
    }

    // Goalまで全力疾走！
    emoter.turn(100);
    walker.runStraightReset();
    for(carryCount = 0; carryCount < 1000; carryCount++) {
        self_l.update(1);
        self_l.writing_current_coordinates(fp);
        walker.runStraight(80);

        if(self_l.line_target_coordinates(110, 1, 1)) {
            ev3_speaker_play_tone(NOTE_E5, 80);
            while(1) {
                self_l.update(1);
                self_l.writing_current_coordinates(fp);
                walker.run(80, 40);
                if(self_l.line_target_coordinates(105, 1, -1)) {
                    walker.runStraightReset();
                    break;
                }
            }
        }
        if(self_l.line_target_coordinates(90, 1, -1)) {
            ev3_speaker_play_tone(NOTE_E5, 80);
            while(1) {
                self_l.update(1);
                self_l.writing_current_coordinates(fp);
                walker.run(80, -30);
                if(self_l.line_target_coordinates(95, 1, 1)) {
                    walker.runStraightReset();
                    break;
                }
            }
        }

        if(self_l.line_target_coordinates(900, -1, 1)) {
            break;
        }
    }
}

