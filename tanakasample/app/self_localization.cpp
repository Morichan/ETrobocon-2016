#include "self_localization.h"
//#include "common_definition.h"
int32_t right_motor;
int32_t left_motor;
//Constructor
/*
Pid::Pid(double p_gain,double i_gain,double d_gain,double target){
  wrapper_of_constructor(p_gain,i_gain,d_gain,0,0,target,0);
}
*/
Self_localization::Self_localization(){
  //left_motor = (PORT_C);
  //right_motor(PORT_B);
   
    //モータの角位置をゼロにリセットする
    walker_sl.reset();
    
    self_localization_constructor_make();

}

//wrapper
void Self_localization::self_localization_constructor_make(){
    right_motor_current_angle = (float)walker_sl.get_count_R()/*ev3_motor_get_counts(1)*/;
    left_motor_current_angle = (float)walker_sl.get_count_L()/*ev3_motor_get_counts(2)*/;
    right_motor_old_angle = 0;
    left_motor_old_angle  = 0;
    right_motor_rotation_angle = 0;
    left_motor_rotation_angle = 0;
    wheel_across = 8.0;
    //  new_self_localization->between_wheels = 15.9;
    between_wheels = 13.00;//車軸の長さ
    moving_distance = 0;
    turning_angle = 0;
    right_wheel_moving_distance = 0;
    left_wheel_moving_distance  = 0;
    current_x = 0;
    current_y = 0;
    current_angle = 0;
    old_x = 0;
    old_y = 0;
    old_angle = 0;
    sup_number = 1.7;
}

/*自己位置の値を更新*/
void Self_localization::self_localization_update(){
    //rotation angle of right wheel
    
    old_x = current_x;
    old_y = current_y;
    old_angle = current_angle;
    
    right_motor_old_angle = right_motor_current_angle;
    right_motor_current_angle = (float)walker_sl.get_count_R()/*ev3_motor_get_counts(right_motor)*/;
    right_motor_rotation_angle =
    right_motor_current_angle - right_motor_old_angle + sup_number;//補助数を足して誤差を減らす
    right_wheel_moving_distance =
    wheel_across * M_PI * (right_motor_rotation_angle / 360.0);
    
    //rotation angle of left wheel
    left_motor_old_angle = left_motor_current_angle;
    left_motor_current_angle = (float)walker_sl.get_count_L()/*ev3_motor_get_counts(left_motor)*/;
    left_motor_rotation_angle =
    left_motor_current_angle - left_motor_old_angle + sup_number;//補助数を足して誤差を減らす
    left_wheel_moving_distance =
    wheel_across * M_PI * (left_motor_rotation_angle / 360.0);
    
    //moving distance and turning angle
    moving_distance =
    (right_wheel_moving_distance + left_wheel_moving_distance) / 2;
    turning_angle = (right_wheel_moving_distance - left_wheel_moving_distance)
    / between_wheels;
    
    //updating of coordinates
    current_x = old_x + (moving_distance * cos(old_angle + (turning_angle/2)));
    current_y = old_y + (moving_distance * sin(old_angle + (turning_angle/2)));
    current_angle = old_angle + turning_angle;
}
/*目的地を指定*/
int Self_localization::self_localization_near_target_coordinates(float target_x, float target_y, float target_radius,float ev3_radius){
    float distance_between_two_points =
    sqrt( pow((target_x - current_x), 2) + pow((target_y - current_y), 2));
    float total_of_two_radius = target_radius + ev3_radius;
    
    char s[50];
    sprintf(s, "dbtp:%f, totr:%f", distance_between_two_points, total_of_two_radius);
    ev3_lcd_draw_string(s, 0, 60);
    
    if(distance_between_two_points > total_of_two_radius)
        return 0;
    else
        return 1;
}

void Self_localization::self_localization_display_coordinates(){
    char str[40];
    sprintf(str, "X:%f Y:%f", current_x, current_y);
    ev3_lcd_draw_string(str, 0, 50);
}


//ファイル書き込み用
void Self_localization::self_localization_writing_current_coordinates(FILE* fp) {
    
    fprintf(fp, "%f %f\n", current_x, current_y);
    
    return;
}
