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


void Area::area_method_startup(){
  switch(current_step) {
  case 0:

    //app.cppのmain_task()のうち、走行前の準備部分
    /* LCD画面表示 */
    msg_f("ET-Robocon'16 tanakasample", 1);
    msg_f(" create from Katlab-sample.", 2);
    /* Open Bluetooth file */
    bt = ev3_serial_open_file(EV3_SERIAL_BT);
    assert(bt != NULL);
    // act_tsk(BT_TASK);          // Bluetooth通信タスクの起動
    /* 初期化完了通知 */
    ev3_led_set_color(LED_ORANGE);
    /* スタート待機 */
    flagman->startingWait();
    /* スタート通知 */
    ev3_led_set_color(LED_GREEN);
    /* 手と尻尾のリセット */
    lifter->reset();
    emoter->reset();

    //次のステップへ
    this_area_is_finished = true;

    break;

  case 1:

    break;

 default:

   break;
  }
}


