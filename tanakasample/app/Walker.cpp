#include "Walker.h"

Walker::Walker():
    leftWheel(PORT_C), rightWheel(PORT_B) {
}

void Walker::init() {
    init_f("Walker");
}

void Walker::terminate() {
    msg_f("Stopped.", 1);
    leftWheel.stop();
    rightWheel.stop();
}

void Walker::reset() {
    leftWheel.reset();
    rightWheel.reset();
}

void Walker::stop() {
    run(0, 0);

    msg_f("stopping...", 1);
    leftWheel.reset();
    rightWheel.reset();
}

void Walker::run(int8_t pwm, int8_t turn) {
    msg_f("running...", 1);

    /* left = p-t, right = p+t -> 右 */
    /* left = p+t, right = p-t -> 左 */
    leftWheel.setPWM(pwm - turn * leftRight);
    rightWheel.setPWM(pwm + turn * leftRight);
}

int32_t Walker::get_count_L() {
    return leftWheel.getCount();
}

int32_t Walker::get_count_R() {
    return rightWheel.getCount();
}

void Walker::edgeChange() {
    if(leftRight == 1) {
        run(20, -10);
        clock.sleep(20);
        leftRight = -1;
    } else {
        run(20, 10);
        clock.sleep(20);
        leftRight = 1;
    }
}

void Walker::angleChange(int angle, int rotation) {
    int32_t defaultAngleL, defaultAngleR;

    if(rotation >= 0) {
        rotation = 1;
    } else {
        rotation -1;
    }

    angle -= angle % 45;
    angle /= 45;

    defaultAngleL = leftWheel.getCount();
    defaultAngleR = rightWheel.getCount();

    while(1) {
        run(0, 10 * rotation);
        if(rightWheel.getCount() - defaultAngleL > 73 * angle * rotation) {
            break;
        }
        clock.sleep(4);
    }
    stop();
}

