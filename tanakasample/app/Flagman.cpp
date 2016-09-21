#include "Flagman.h"



Flagman::Flagman():
    touchSensor(PORT_1), sonarSensor(PORT_3) {
}

void Flagman::startingWait() {
    int8_t bt_cmd = 0;

    while (1) {
        if(bt_cmd == 1) {
            break;
        }
        if(touchSensor.isPressed()) {
            break;
        }

        clock.sleep(20);
    }
}

bool Flagman::rewardModeFlag() {
    bool flag = false;

    if(sonarSensor.getDistance() < 30) {
        flag = true;
    }

    return flag;
}

