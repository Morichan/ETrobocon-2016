#ifndef __AREA_METHOD_STANDARD_RUN__
#define __AREA_METHOD_STANDARD_RUN__

#include "area.h"
#include "pid.h"
#include "PidWalker.h"
#include "ColorSensor.h"


#include "app.h"
#include "util.h"

#include "Flagman.h"
#include "PidWalker.h"
#include "Lifter.h"
#include "Emoter.h"

#include "area_control.h"
#include "area.h"



/**
 *27行目と38行目、ColoeSensor関係がエラーなので一時的にコメントアウト
 **/

//ColorSensor cs_area_method_standard_run{PORT_2};
int8_t turn_area_method_standard_run;
int8_t forward_area_method_standard_run;
int edge_direction_area_method_standard_run = 1;
//エッジの向きによって変更(+->右,-->左)


void Area::area_method_standard_run(){
  //PidWalker pw;
  //pw.trace();
  
  //pid.calculate(cs_area_method_standard_run.getBrightness());
  turn_area_method_standard_run = (int8_t)pid.get_output();
  self_localization.update(edge_direction_area_method_standard_run);//自己位置のデータ更新

  switch(current_step){
  case 0:
    forward_area_method_standard_run = 30;
    break;
  case 1:
    if(self_localization.near_target_coordinates(400,0,30,0) == 1) {
      
    }
    break;

  case 2:
    break;

  default:
    break;
  }
  

}

#endif

