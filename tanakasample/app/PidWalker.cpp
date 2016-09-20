#include "ev3api.h"
#include "ColorSensor.h"
#include "Clock.h"

#include "PidWalker.h"

/* default pid(2.0, 0.05, 0.13, 20.0) */
/* forward=50, pid(1.0, 0.0, 11.0, border=30) */
PidWalker::PidWalker():
    colorSensor(PORT_2), sonarSensor(PORT_3), pid(1.0, 0.0, 5.0, border) {
}

void PidWalker::start() {
    walker.reset();
}

void PidWalker::stop() {
    walker.reset();
}

/*
 * 加速機能
 * 最初は遅めにして、だんだんスピードを上げる
 * 一つ目の引数はスタートするときのスピード
 * 二つ目の引数までのスピードに上げる
 */
void PidWalker::accelerate(int8_t startForward, int8_t _forward) {
    int32_t count = 0;

    /*
     * もしstartForward == 0なら、現在の速度から更に加速
     */
    if(startForward != 0) {
        forward = startForward;

        // 最初のスピードより最後のスピードのほうが遅いときは0からに強制
        if(forward > _forward || forward < 0) {
            forward = 0;
        }

        // 最終的なスピードがマイナスの時は、初期値と同じにする（加速しない）
        if(_forward < 0) {
            _forward = forward;
        }
    }

    while(1) {
        if(ev3_button_is_pressed(BACK_BUTTON)) {
            break;
        }
        if(forward >= _forward) {
            break;
        }

        brightness = colorSensor.getBrightness();

        pid.calculate(brightness);
        turn = (int8_t)pid.get_output();

        walker.run(forward, turn);

        /*
         * 20msec周期でforwardを増やす
         */
        count++;
        if(count % 5 == 0) {
            forward++;
        }
        clock.sleep(4); /* 4msec周期起動 */
    }
}

/*
 * ブレーキ機能
 * だんだんスピードを下げる
 * 一つ目の引数は初期スピード、0なら現在のスピード
 * 二つ目の引数までのスピードに下げる
 */
void PidWalker::brake(int8_t startForward, int8_t _forward) {
    int32_t count = 0;

    if(startForward > 0) {
        forward = startForward;
    }

    if(forward < _forward) {
        forward = _forward;
    }

    while(1) {
        if(ev3_button_is_pressed(BACK_BUTTON)) {
            break;
        }
        if(forward <= _forward) {
            break;
        }

        brightness = colorSensor.getBrightness();

        pid.calculate(brightness);
        turn = (int8_t)pid.get_output();

        walker.run(forward, turn);

        /*
         * 12msec周期でforwardを減らす
         */
        count++;
        if(count % 3 == 0) {
            forward--;
        }
        clock.sleep(4); /* 4msec周期起動 */
    }
}

void PidWalker::setForward(int8_t _forward) {
    forward = _forward;
}

/*
 * PID制御で走行する。
 */
void PidWalker::trace() {

    brightness = colorSensor.getBrightness();

    pid.calculate(brightness);
    turn = (int8_t)pid.get_output();

    walker.run(forward, turn);

    clock.sleep(4); /* 4msec周期起動 */
}
