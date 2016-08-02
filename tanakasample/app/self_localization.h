#ifndef __SELF_LOCALIZATION__
#define __SELF_LOCALIZATION__
//self_localization.c
//#include "common_definition.h"
#include <stdio.h>
#include "Walker.h"
#include <math.h>
#define KP 0.45
#define KI 0.06
#define KD 0.04
#define CYCLE_ANGLE 40
#define RADIUS 40
#define VEHICLE_WIDTH 160

//自己位置推定用クラス
class Self_localization{
  //menber variables
/*private:*/
private:
    //self_localization.h
    //構造体::self_localization
    /*typedef struct self_localization_structure {*/
    float right_motor_current_angle, right_motor_old_angle, right_motor_rotation_angle;
    float left_motor_current_angle, left_motor_old_angle, left_motor_rotation_angle;
    float wheel_across;
    float between_wheels;
    float right_wheel_moving_distance, left_wheel_moving_distance;
    float moving_distance;
    float turning_angle;
    float current_x, current_y, current_angle;
    float old_x, old_y, old_angle;
    //target_x,target_y,target_radius,ev3_radius;//目的地指定用
    /*} self_localization;*/
    double sup_number;//後輪の誤差を減らすための補助数
  //member methods
public:
   
    //Constructor
    Self_localization();
    //自己位置の計算
    void self_localization_update();
    /*目的地の指定*/
    int self_localization_near_target_coordinates(float target_x, float target_y, float target_radius,
                                                  float ev3_radius);
    //ファイル書き出し
    void self_localization_writing_current_coordinates(FILE*);
    void self_localization_display_coordinates();
private:
    //
    Walker walker_sl;
    void self_localization_constructor_make();

};

#endif
