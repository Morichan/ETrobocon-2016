#include "area_control.h"
#include "area.h"
#include "pid.h"
#include <stdlib.h>

Area_control::Area_control(Run_route run_route){
  //最大Area数はenum Area_name_listの最後のFINISHED(=定数)

  Area_name_list last_area = AREA_finished;
  area_total_number = last_area;
  current_area_index = 0;
  set_area_name_array(run_route); //area_name_arrayにareaを並べる
  area = Area(area_name_array[0]);
}
void Area_control::set_area_name_array(Run_route run_route){
  switch(run_route) {
  case TEST: //テスト
    area_name_array[0] = AREA_startup;
    area_name_array[1] = AREA_standard_run;
    area_name_array[2] = AREA_finished;
    break;

  case L_ALL: //戦略：Ｌコース完全走破狙い
    /*    name_array[0] = L_Akino_tactics;    //スタートから仕様未確定エリア乗り上げ
	  name_array[1] = L_SPECIFICATIONS_UNDETERMAINED_AREA; //仕様未確定エリア
	  name_array[2] = L_AFTER_SUA;                  //仕様未確定エリア後～ゴール
	  name_array[3] = L_RIGHT_ANGLE_PARKING;               //直角駐車場
	  name_array[4] = FINISHED;                            //おしまい 
    */
    break;

  case R_ALL: //戦略：Ｒコース完全走破狙い
    /*    name_array[0] = STANDARD;           //スタート～フィギュアＬ前
	  name_array[1] = R_FIGURE_L;         //フィギュアＬ
	  name_array[2] = R_SHINKANSEN;       //新幹線
	  name_array[3] = STANDARD;           //新幹線後～第二ゴールＲ
	  name_array[4] = R_PARALLEL_PARKING; //縦列駐車
	  name_array[5] = FINISHED;           //おしまい
    */
    break;

  default:
    break;
  }
}

//最終的には、最後にモーターを停止するAreaを用意しておいてゴールとする
void Area_control::update(){
  switch (area.current_area_name) {
  case AREA_startup:
    msg_f("                      ", 4);
    msg_f("AREA_startup", 4);
    break;
  case AREA_standard_run:
    msg_f("                      ", 4);
    msg_f("AREA_standard_run", 4);
    break;
  case AREA_finished:
    msg_f("                      ", 4);
    msg_f("AREA_finished", 4);
    break;
  default:
    msg_f("                      ", 4);
    msg_f("default", 4);
    break;
  }
  char s[10];
  sprintf(s, "area.current_step:%d", area.current_step);
  msg_f("                      ", 5);
  msg_f(s, 5);

  //現在のAreaが終わったら次のAreaへ移行
  if(area.is_finished()) switch_next_area();
  //Areaのrunを実行、走る
  area.run_by_area_method();
}

void Area_control::switch_next_area(){
  current_area_index++; //インデックスを次に進めて
  area.set_current_step(0);
  area.set_this_area_is_finished(false);
  area.set_current_area_name(area_name_array[current_area_index]);
  area.pid = Pid(1.0, 0.0, 0.3, 30.0);

  ev3_speaker_play_tone(NOTE_D4, 300);
}


