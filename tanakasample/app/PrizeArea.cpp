#include "PrizeArea.h"

void PrizeArea::getPrize() {
    lifter.liftDown();
    walker.run(10, 0);
    clock.sleep(1000);
    walker.stop();
    lifter.liftUp();
    emoter.wipe(100, 5, 120);
}
