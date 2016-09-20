#ifndef __SELF_LOCALIZATION__
#define __SELF_LOCALIZATION__
//self_localization.c
//#include "common_definition.h"
#include <stdio.h>
#include "Walker.h"
#include "Clock.h"
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
    /*typedef struct structure {*/
    float right_motor_current_angle, right_motor_old_angle, right_motor_rotation_angle;
    float left_motor_current_angle, left_motor_old_angle, left_motor_rotation_angle;
    float wheel_across;
    float between_wheels;
    float right_wheel_moving_distance, left_wheel_moving_distance;
    float moving_distance;
    float turning_angle;
    float current_x, current_y, current_angle;
    float old_x, old_y, old_angle;
    float standard_point_x,standard_point_y;/*基準となる座標*/
    float origin_slope,origin_intercept;/*(基準点と現在地)傾きと切片*/
    float target_slope,target_intercept;/*傾きと切片*/
    float tortal_slope,tan_angle,radian_angle,subtraction_radian_angle,degree_angle;
    float axis_size;
    float ev3_direction;
    int ev3_direction_int,flag_1;
    float navi_standard_point_x;
    float navi_standard_point_y;
    float x_now,y_now;
    /*turn_target_point*/
    
    //target_x,target_y,target_radius,ev3_radius;//目的地指定用
    /*} self_localization;*/
    double sup_number;//後輪の誤差を減らすための補助数
  //member methods
public:
    
    //Constructor
    Self_localization();
    //リセット（回転した時に使用）
    int angle_reset(int edge_direction);
    //リセット
    int reset();
    //自己位置の計算(エッジによって変更の必要あり)
    void update(int edge_direction);
    /*目的地の指定*/
    int near_target_coordinates(float target_x, float target_y, float target_radius,
                                                  float ev3_radius);
    /*目的の指定(ゴールテープ方式)*/
    int line_target_coordinates(int target_line,int target_axis);
    /*①基準となる座標の指定*/
    int standard_point(int distance);
    /*②(目的地)２点の座標から示される直線の傾きと切片を計算*/
    void slope_intercept(float target_x,float target_y);
    /*目的地を向くための計算*/
    int turn_target_point();
    /*目的地までの移動*/
    int navi(float target_x, float target_y, float target_radius, float ev3_radius,int edge_direction);
    int navi_standard_point(int distance);
    
    
    //ファイル書き出し,画面出力
    void writing_current_coordinates(FILE*);
    void writing_current_coordinates_standard(FILE*);
    void display_coordinates();
    void writing_direction(FILE*);
    void writing_angle(FILE*);
private:
    //
    Walker walker_sl;
    Clock clock;
    void constructor_make();

};

#endif
