#include "area.h"

#include "Flagman.h"
#include "PidWalker.h"
#include "Lifter.h"
#include "Emoter.h"
#include "module_cfg.h"

//app.cppにてグローバル宣言
extern FILE *bt;
extern Flagman* flagman;
extern PidWalker* pidWalker;
extern Lifter* lifter;
extern Emoter* emoter;


void Area::area_method_finished(){
  switch(current_step) {
  case 0:


    //app.cppに書いてあった終了処理
    lifter->terminate();
    emoter->terminate();
    msg_f("Reseted", 1);
    // ter_tsk(BT_TASK);         // Bluetooth通信タスクの停止
    fclose(bt);
    ext_tsk();


    break;

 default:

   break;
   
  }
}


