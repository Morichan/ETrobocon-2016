#include "ev3api.h"
#include "ColorSensor.h"
#include "Clock.h"

#include "PidWalker.h"

/* default pid(2.0, 0.05, 0.13, 20.0) */
/* forward=50, pid(1.0, 0.0, 11.0, border=30) */
PidWalker::PidWalker():
    colorSensor(PORT_2), sonarSensor(PORT_3), pid(0.5, 0.0, 2.0, border) {
}

void PidWalker::start() {
    walker.reset();
}

void PidWalker::stop() {
    walker.reset();
}

/*
 * スタートダッシュ機能
 * 最初は遅めにして、だんだんスピードを上げる
 */
void PidWalker::startDash(int8_t _forward) {
    forward = 1;
    int32_t count = 0;

    
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
