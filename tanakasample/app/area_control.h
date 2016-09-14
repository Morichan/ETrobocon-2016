#ifndef __AREA_CONTROL__
#define __AREA_CONTROL__

#include "area.h"
#include "self_localization.h"

typedef enum _Run_route{
  R_ALL,
  L_ALL,
  TEST
} Run_route;

class Area_control{
public:
  Area_name_list area_name_array[20]; //暫定的に20個用意しとく
  int area_total_number; //Areaの総数
  int current_area_index; //現在が何個目のAreaなのか
  Area area{area_name_array[0]};

  //AreaとArea_controlどちらに持たせる??
  Area_control(Run_route run_route);
  void set_area_name_array(Run_route run_route);
  void update();
  void switch_next_area();
};


#endif

