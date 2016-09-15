#include "area.h"
#include "pid.h"

Area::Area(Area_name_list area_name){
  current_step = 0;
  this_area_is_finished = false;
  current_area_name = area_name;
  //pid = new Pid::Pid{1.0, 0.0, 0.3, 30.0};
}



//Areaが終わっているかどうかの判定
bool Area::is_finished() {
  return this_area_is_finished;
}

void Area::run_by_area_method(){
  //  ev3_speaker_play_tone(NOTE_E5, 20);
  //それぞれのArea用メソッドに飛ぶ
  //その後、current_stepを見てArea用メソッド内をswitchで動作していく
  switch(current_area_name){
  case AREA_startup:
    area_method_startup();
    break;
  case AREA_standard_run:
    area_method_standard_run();
    break;
  case AREA_finished:
    area_method_finished();
    break;
    /*  case STANDARD:
    Tactics_standard(strategy->tactics_main);
    break;
  case L_BARCODE:
    Tactics_barcode(tactics);
    break;
  case U_TURN:
    Tactics_u_turn(stratgy->tactics_main);
    break;
  case R_FIGURE_L:
    Tactics_r_figure(strategy->tactics_main);
    break;
  case R_SHINKANSEN:
    Tactics_r_shinkansen(strategy->tactics_main);
    break;
  case R_PARALLEL_PARKING:
    Tactics_r_parallel_parking(strategy->tactics_main);
    break;
  case L_RIGHT_ANGLE_PARKING:
    Tactics_l_right_angle_parking(strategy->tactics_main);
    break;*/
  default:
    break;
  }


}

void Area::set_current_step(int arg){
  current_step = arg;
}
void Area::set_this_area_is_finished(bool arg){
  this_area_is_finished = arg;
}
void Area::set_current_area_name(Area_name_list arg){
  current_area_name = arg;
}


