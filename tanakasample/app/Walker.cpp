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

void Walker::runStraightReset() {
    oldWheelL = 0;
    oldWheelR = 0;
}

void Walker::runStraight(int8_t pwm) {
    msg_f("running straight...", 1);

    int32_t wheelL, wheelR;

    nowWheelL = get_count_L();
    nowWheelR = get_count_R();

    /*
     * もしoldWheelLとoldWheelRのどちらかでも0なら、
     * 初期値ということだから値をセットして今回は終わり
     */
    if(oldWheelL == 0 || oldWheelR == 0) {
        oldWheelL = get_count_L();
        oldWheelR = get_count_R();

    } else {
        wheelL = nowWheelL - oldWheelL;
        wheelR = nowWheelR - oldWheelR;

        if(pwm > 0) {
            // 前に進む
            if(wheelL > wheelR) {
                straightTurn = wheelL - wheelR;
            } else {
                straightTurn = wheelR - wheelL;
            }
        } else {
            // 後ろに戻る
            if(wheelL > wheelR) {
                straightTurn = wheelR - wheelL;
            } else {
                straightTurn = wheelL - wheelR;
            }
        }
        run(pwm, straightTurn);
    }
    clock.sleep(4);
}

int32_t Walker::get_count_L() {
    return leftWheel.getCount();
}

int32_t Walker::get_count_R() {
    return rightWheel.getCount();
}

int Walker::edgeChange() {
    int32_t default_countL = get_count_L();
    int32_t default_countR = get_count_R();

    if(leftRight == 1) {
        while(1) {
            run(10, 5);
            clock.sleep(4);
            if(get_count_L() - default_countL > 20) {
                break;
            }
        }
        leftRight = -1;
    } else {
        while(1) {
            run(10, 5);
            clock.sleep(4);
            if(get_count_R() - default_countR > 20) {
                break;
            }
        }
        clock.sleep(10);
        leftRight = 1;
    }

    return leftRight;
}

/*
 * 車輪の回転角分だけ進む
 */
void Walker::moveAngle(int8_t pwm, int angle) {

    leftWheel.reset();
    rightWheel.reset();

    leftWheel.setPWM(pwm);
    rightWheel.setPWM(pwm);

    while(1){
        if(leftWheel.getCount() >= angle && rightWheel.getCount() >= angle ) break;
        clock.sleep(4);
    }

    leftWheel.reset();
    rightWheel.reset();
}

/*
 * 主機能:45度単位で回転
 * rotation = 1 -> 反時計回り, rotation = -1 -> 時計回り
 *
 * beta機能:5度単位で回転
 * 精度はあまりよろしくない
 */
void Walker::angleChange(int angle, int rotation) {
    int32_t defaultAngleL;
    int8_t dAngle = 75; // 45度におけるモーター回転数（床材によって変わる？）

    if(rotation >= 0) {
        if(leftRight == 1) {
            rotation = 1;
        } else {
            rotation = -1;
        }
    } else {
        if(leftRight == 1) {
            rotation = -1;
        } else {
            rotation = 1;
        }
    }

    /*
     * 本来は45度単位だから、angleは45で割る
     * ベータ機能として5度単位でも曲がれるようにしている
     * そのため、もしangleが5度単位である場合はdAngleを9分割する
     */
    if(angle % 5 == 0 && angle % 45 != 0) {
        dAngle = 8;
        angle /= 5;
    } else {
        angle -= angle % 45;
        angle /= 45;
    }

    defaultAngleL = leftWheel.getCount();

    while(1) {
        run(0, 10 * rotation);
        if(rotation >= 0) {
            if(leftWheel.getCount() - defaultAngleL < -dAngle * angle * rotation ||
                leftWheel.getCount() - defaultAngleL > dAngle * angle * rotation) {
                break;
            }
        } else {
            if(leftWheel.getCount() - defaultAngleL > -dAngle * angle * rotation ||
                leftWheel.getCount() - defaultAngleL < dAngle * angle * rotation) {
                break;
            }
        }
        clock.sleep(4);
    }
    stop();
}

