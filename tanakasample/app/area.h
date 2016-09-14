#ifndef __AREA__
#define __AREA__

#include "pid.h"
#include "self_localization.h"

typedef enum _Area_name_list{
  AREA_startup,
  AREA_standard_run,
  AREA_finished //これ以降に書き足さないこと！ 増やす場合は上に追加すること
} Area_name_list;


class Area{
public:
  int current_step;
  bool this_area_is_finished;
  Area_name_list current_area_name;
  Pid pid{1.0, 0.0, 0.3, 30.0};
  Self_localization self_localization; //自己位置推定


  Area(Area_name_list area_name);
  bool is_finished();
  void run_by_area_method();
  void set_current_step(int arg);
  void set_this_area_is_finished(bool arg);
  void set_current_area_name(Area_name_list arg);
  //以後、実際の走行メソッド

  void area_method_startup();
  void area_method_standard_run();
  void area_method_finished();
};



#endif

