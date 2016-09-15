#include "PrizeArea.h"

void PrizeArea::getPrize() {
    lifter.liftDown();
    walker.run(20, 0);
    clock.sleep(800);
    walker.stop();
    lifter.liftUp();
    emoter.wipe(100, 4, 120);
}

void PrizeArea::carryPrize() {
    walker.angleChange(90, 1);
    walker.run(80, 35);
    clock.sleep(1320);
    walker.stop();
    emoter.turn(100);
    walker.run(80, 20);
    clock.sleep(5500);
}

