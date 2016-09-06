#ifndef __PRIZEAREA__
#define __PRIZEAREA__

#include "Clock.h"
#include "Walker.h"
#include "Lifter.h"
#include "Emoter.h"

#include "util.h"

using namespace ev3api;

class PrizeArea {
public:
    void getPrize();

private:
    Clock clock;
    Walker walker;
    Lifter lifter;
    Emoter emoter;
};

#endif
