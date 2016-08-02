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

void Walker::run(int8_t pwm, int8_t turn) {
    msg_f("running...", 1);

    /* left = p-t, right = p+t -> 右 */
    /* left = p+t, right = p-t -> 左 */
    leftWheel.setPWM((pwm - turn) * leftRight);
    rightWheel.setPWM((pwm + turn) * leftRight);
}

void Walker::edgeChange() {
    if(leftRight == 1) {
        run(0, 50);
        clock.sleep(100);
        leftRight = -1;
    } else {
        run(0, -50);
        clock.sleep(100);
        leftRight = 1;
    }
}

