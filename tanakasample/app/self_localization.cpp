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
 edge_direction:右->1
                左->-1
 */
int Self_localization::angle_reset(int edge_direction){
    walker_sl.reset();
    right_motor_current_angle = (float)walker_sl.get_count_R();//回転数の初期化(座標は保持)
    left_motor_current_angle = (float)walker_sl.get_count_L();//回転数の初期化(座標は保持)
    //車体の角度を更新(左エッジ(edge_direction = -1):180-,右エッジ:なし)
    if (edge_direction == -1) {
       current_angle = (subtraction_radian_angle) + current_angle;
    }else{
         current_angle =current_angle + (subtraction_radian_angle-100);
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
int Self_localization::line_target_coordinates(int target_line,int target_axis){
    if (target_axis == 1) {/*指定する座標がx軸の場合*/
        axis_size = current_x;
    }
    if(target_axis == -1){/*指定する座標がy軸の場合*/
        axis_size = current_y;

    }
    if (target_line > axis_size)/*指定する座標を通った場合*/
        return 0;
    else
        return 1;
    
}


/*目的地点までの移動*///<-動くかわからない
/*
 target_x:目的地のx座標
 target_y:目的地のy座標
 target_radius:目的地の周囲(半径)
 ev3_radius:ev3の半径
 edge_direction:エッジの向き 右->1 左=-1
 */
int Self_localization::navi(float target_x, float target_y, float target_radius, float ev3_radius,int edge_direction){
    update(edge_direction);
    navi_standard_point(20);
        /*②２点の座標から傾きと切片を求める(目的地x,目的地y)*/
    slope_intercept(target_x,target_y);
    /*③向きの算出 →　direction(仮)*/
    ev3_direction = turn_target_point();
    /*float → int*/
    if(edge_direction == 1){
        ev3_direction_int = ev3_direction -100;
    }else if(edge_direction == -1){
    ev3_direction_int = ev3_direction;
        ev3_direction_int -= ev3_direction_int%5;
    
    }
    x_now = current_x;
    y_now = current_y;
    /*目的地の方を向く*/
    /*
     時計回り:-1
     反時計:1
     */
    if(flag_1 == 0){
    walker_sl.angleChange(ev3_direction_int,1);
        angle_reset(edge_direction);
        current_x = x_now;
        current_y = y_now;
        flag_1 = 1;
    }
    /*基準地点が更新されるまで移動*/
        
    /*事故位置のリセット*/
   // angle_reset(edge_direction);
    
    if(flag_1 == 1) {
        walker_sl.run(25,0);
        if (navi_standard_point(20)) {
            flag_1 = 0;
        }
        clock.sleep(200);
    }
    /*近くに来たら1を返す*/
    if(near_target_coordinates(target_x,target_y,target_radius,ev3_radius) == 1){
        walker_sl.run(0,0);
        flag_1 = 2;
        return 1;
    }else{
        return 0;
    }
    
}

/*目的地の方向を向くための計算*/
/*
 tortal_slope:各式の傾きの積
 tan_angle:tanΘの値
 radian_angle:Θのラジアン値
 subtraction_radian_angle:πからΘを引いた値
 degree_angle:Θの角度
 戻り値:現在地から目的地に行くための角度を求める
 */
int Self_localization::turn_target_point(){
   tortal_slope  = origin_slope * target_slope;
    if(tortal_slope == -1){
        /*積が-1であった場合に90°である*/
        return 90;
    }else{
        /*tanΘの値を求める*/
        tan_angle = fabs((target_slope - origin_slope)/(1+target_slope * origin_slope));
        /*逆三角関数を用いてΘを求める*/
        radian_angle = atan(tan_angle);
        /*πから引く*/
        subtraction_radian_angle = M_PI - radian_angle;
        /*ラジアン→角度*/
        degree_angle = (subtraction_radian_angle * 180)/M_PI;
        
        return degree_angle;
    }
    
}



/*(基準点)２点の座標から示される直線の傾きと切片を計算*/
/*
 target_x:目的地のx座標
 target_y:目的地のy座標
 standard_point_x:基準点のx座標
 standard_point_y:基準点のy座標
 current_x:現在地のx座標
 current_y:現在地のy座標
 origin_slope:傾き(float)
 origin_intercept:切片(float)
 */
void Self_localization::slope_intercept(float target_x,float target_y){
    
    /*基準点と現在地*/
    /*傾きを求める*/
    origin_slope = (current_y - standard_point_y)/(current_x - standard_point_x);
    
    /*切片を求める*/
    origin_intercept = (current_y-standard_point_y)/(current_x - standard_point_x) + standard_point_y;

    /*目的地と現在地*/
    /*傾きを求める*/
    target_slope = (current_y - target_y)/(current_x - target_x);
    
    /*切片を求める*/
    target_intercept = (current_y-target_y)/(current_x - target_x) + target_y;

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



/*x基準値の更新(naviでの移動中)*/
/*
 役割:前回の基準点よりもdistance分の距離以上離れると基準の位置が更新される
 current_x:現在のx座標
 current_y:現在のy座標
 navi_standard_point_x:基準点のx座標
 navi_standard_point_y:基準点のy座標
 */
int Self_localization::navi_standard_point(int distance){
    
    if (pow((current_x - standard_point_x),2.0) + pow((current_y - standard_point_y),2.0) > distance){
        navi_standard_point_x = current_x;
        navi_standard_point_y = current_y;
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
