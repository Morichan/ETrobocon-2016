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
    
    constructor_make();

}

/*リセット(anglechangeした場合に使用する)*/
/*
 clock_wise:時計回り->1
            反時計まわり->-1
 */
int Self_localization::angle_reset(int clock_wise,int rotated_angle){
    walker_sl.reset();
    right_motor_current_angle = (float)walker_sl.get_count_R();//回転数の初期化(座標は保持)
    left_motor_current_angle = (float)walker_sl.get_count_L();//回転数の初期化(座標は保持)
    //車体の角度を更新(左エッジ(edge_direction = -1):180-,右エッジ:なし)
    rad_angle = (rotated_angle * M_PI)/180;
    if (clock_wise == 1) {
       current_angle = current_angle + rad_angle;
    }else{
         current_angle =current_angle - rad_angle;
    }
   
    return 1;
}

/*リセット(イベントごとに使用)*/
int Self_localization::reset(){
    walker_sl.reset();
    right_motor_current_angle = (float)walker_sl.get_count_R();//回転数の初期化(座標は保持)
    left_motor_current_angle = (float)walker_sl.get_count_L();//回転数の初期化(座標は保持)
    return 1;
}

//wrapper
void Self_localization::constructor_make(){
    right_motor_current_angle = (float)walker_sl.get_count_R()/*ev3_motor_get_counts(1)*/;
    left_motor_current_angle = (float)walker_sl.get_count_L()/*ev3_motor_get_counts(2)*/;
    wheel_across = 8.0;
    between_wheels = 13.00;//車軸の長さ
    sup_number = 1.7;
    
    right_motor_old_angle = 0;
    left_motor_old_angle  = 0;
    right_motor_rotation_angle = 0;
    left_motor_rotation_angle = 0;
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
    standard_point_x = 0;/*基準となる座標*/
    standard_point_y = 0;/*基準となる座標*/
    tortal_slope = 0;//各式の傾きの積
    tan_angle = 0;//tanΘの値
    radian_angle = 0;//Θのラジアン値
    subtraction_radian_angle = 0;
    axis_size = 0;//ゴールテープの奴
    //πからΘを引いた値
    degree_angle = 0;//Θの角
    flag_1 = 0;
    navi_standard_point_x = 0;
    navi_standard_point_y = 0;
    x_now = 0;
    y_now = 0;
}

/*自己位置の値を更新*/
/*
 edge_direction:右->1
                左->-1
 */
void Self_localization::update(int edge_direction){
    //rotation angle of right wheel
    
    old_x = current_x;
    old_y = current_y;
    old_angle = current_angle;
    sup_number = sup_number * edge_direction;//エッジの向きによって変更
    
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
/*
 target_x:目的地のx座標
 target_y:目的地のy座標
 target_radius:目的地の周囲(半径)
 ev3_radius:ev3の半径
 */

int Self_localization::near_target_coordinates(float target_x, float target_y, float target_radius,float ev3_radius){
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

/*目的のラインの指定*/
/*
 target_line:ラインの座標
 target_axis x座標→１or y座標→ −１
 */
int Self_localization::line_target_coordinates(int target_line,int target_axis,int updown){
    if (target_axis == 1) {/*指定する座標がx軸の場合*/
        axis_size = current_x;
    }
    if(target_axis == -1){/*指定する座標がy軸の場合*/
        axis_size = current_y;
    }

    if(updown > 0) {
        if (target_line > axis_size)/*指定する座標を通った場合*/
            return 0;
        else
            return 1;
    } else {
        if (target_line < axis_size)/*指定する座標を通った場合*/
            return 0;
        else
            return 1;
    }
}


/*x基準値の更新*/
/*
 役割:前回の基準点よりもdistance分の距離以上離れると基準の位置が更新される
 current_x:現在のx座標
 current_y:現在のy座標
 standard_point_x:基準点のx座標
 standard_point_y:基準点のy座標
 */
int Self_localization::standard_point(int distance){

    if (pow((current_x - standard_point_x),2.0) + pow((current_y - standard_point_y),2.0) > distance){
        standard_point_x = current_x;
        standard_point_y = current_y;
        return 1;
    }
    return 0;
}




/*--------------------------------------------------------------------------------------------------------------*/
/*ディスプレイ表示用*/
void Self_localization::display_coordinates(){
    char str[40];
    sprintf(str, "X:%f Y:%f", current_x, current_y);
    ev3_lcd_draw_string(str, 0, 50);
}


/*--------------------------------------------------------------------------------------------------------------*/
//ファイル書き込み用
void Self_localization::writing_current_coordinates(FILE* fp) {
    
    fprintf(fp, "%f %f\n", current_x, current_y);
    
}
//ファイル書き込み用
void Self_localization::writing_current_coordinates_standard(FILE* fp) {
    
    fprintf(fp, "傾き:%f 切片:%f|||傾き:%f 切片:%f\n", origin_slope,origin_intercept, target_slope,target_intercept);
    
}


//ファイル書き込み用(目的地の方向)
void Self_localization::writing_direction(FILE* fp) {
    
    fprintf(fp, "%d\n", ev3_direction_int);
 
}


//ファイル書き込み用(車体の向き)
void Self_localization::writing_angle(FILE* fp) {
    
    fprintf(fp, "%f\n", current_angle);
    
}
