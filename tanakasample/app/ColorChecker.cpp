#include "ColorChecker.h"

ColorChecker::ColorChecker():
    colorSensor(PORT_2), lifter() {
}

void ColorChecker::checkBlockColor() {
    colorid_t blockColor;
    blockColor = colorSensor.getColorNumber();
    if(blockColor == 2) {
        lifter.changeDefault(45);
    }
    lifter.changeDefault(-45);
}

