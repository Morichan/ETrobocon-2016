#include "PrizeArea.h"

void PrizeArea::getPrize() {
    lifter.liftDown();
    walker.run(20, 0);
    clock.sleep(800);
    walker.stop();
    lifter.liftUp();
    emoter.wipe(100, 4, 60);
}

void PrizeArea::carryPrize() {
    int32_t carryCount;
    int32_t defaultWheelL;

    // 90度回転する
    walker.angleChange(90, 1);
    // Goalをまっすぐ狙う
    defaultWheelL = walker.get_count_L();
    while(1) {
        walker.run(80, 40);
        if(walker.get_count_L() - defaultWheelL > 920) {
            break;
        }
    }

    // Goalまで全力疾走！
    emoter.turn(100);
    for(carryCount = 0; carryCount < 1000; carryCount++) {
        walker.runStraight(90);
    }
}

