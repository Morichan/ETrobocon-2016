#include "ev3api.h"
#include "ColorSensor.h"
#include "Clock.h"

#include "PidWalker.h"

/* default pid(2.0, 0.05, 0.13, 20.0) */
PidWalker::PidWalker():
    colorSensor(PORT_2), sonarSensor(PORT_3), pid(0.5, 0.0, 2.0, border) {
}

/* forward=50, pid(1.0, 0.0, 11.0, border=30) */
void PidWalker::trace() {

    /* 両輪のリセット */
    walker.reset();

    while(1) {
        // スピードの初期化
        forward = 25;

        if(ev3_button_is_pressed(BACK_BUTTON)) {
            break;
        }
        if(sonarSensor.getDistance() < 10) {
            walker.edgeChange();
        }

        brightness = colorSensor.getBrightness();

        if(border > brightness) {
            speedometer = border - brightness;
        } else {
            speedometer = brightness - border;
        }
        // forward -= speedometer;

        pid.pid_calculate(brightness);
        turn = (int8_t)pid.pid_get_output();

        walker.run(forward, turn);

        clock.sleep(4); /* 4msec周期起動 */
    }

    walker.reset();
}
