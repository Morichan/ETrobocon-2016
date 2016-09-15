/***********************************************************************
 *
 * ET-Robovon'16 tanakasample create from Katlab-sample in 2015
 *   Latest 2016.06.29   by Mori
 *
 *
 * PID制御で走るサンプルプログラムです。
 * Katlab-sampleから削除した機能はBluetooth通信です。
 * というかbt_task関数は動きません。
 * tanakasampleのmain_taskは以下のことができます。
 *
 *  - PidWalker => PID制御でライントレースします。現在は実質PD制御です。
 *  - Flagman   => ボタンが押されるまで止まります。
 *  - Lifter    => liftHandを持ち上げる、下げるなどの動作をします。
 *  - Emoter    => しっぽを回せます。
 *
 * 新たなタスクを追記する方は、
 * Main Task とコメントアウトされている間に書き加えてください。
 ***********************************************************************/

#include "app.h"
#include "util.h"

#include "Flagman.h"
#include "PidWalker.h"
#include "Lifter.h"
#include "Emoter.h"
#include "PrizeArea.h"
#include "ColorChecker.h"
#include "Pedestrian.h"
#include "SonarSensor.h"

#include "area_control.h"
#include "area.h"

#if defined(BUILD_MODULE)
#include "module_cfg.h"
#else
#include "kernel_cfg.h"
#endif

using namespace ev3api;


/**********/
const Run_route COURSE = TEST;
/**********/


/* Bluetooth */
int32_t      bt_cmd = 0;      /* Bluetoothコマンド */
FILE *bt = NULL;       /* Bluetoothファイルハンドル */

/* 関数プロトタイプ宣言 */
// void bt_task(intptr_t unused);

Flagman* flagman;
PidWalker* pidWalker;
Lifter* lifter;
Emoter* emoter;
PrizeArea* prizeArea;
ColorChecker* colorChecker;
Pedestrian* pedestrian;
Walker* walker;
SonarSensor* sonarSensor;

void main_task(intptr_t unused) {

  //Area_controlの生成
  Area_control area_control(COURSE);

    pidWalker = new PidWalker();
    flagman = new Flagman();
    lifter = new Lifter();
    emoter = new Emoter();
    colorChecker = new ColorChecker();
    pedestrian = new Pedestrian();
    walker = new Walker();
    prizeArea = new PrizeArea();
    sonarSensor = new SonarSensor(PORT_3);

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

    /*---------------Main Task from Here ここから---------------*/


    emoter->wipe(100, 5, 90); // 尾が速度100で5回、180度ワイプする
    // emoter->turn(100);         // 尾が速度100で回転する
    pidWalker->startDash(70);
    while(1) {
        pidWalker->trace();        // PID（実質PD）制御でライントレースする
        if(ev3_button_is_pressed(BACK_BUTTON)) {
            break;
        }
        if(sonarSensor->getDistance() < 60) {
            pidWalker->setForward(30);
            if(sonarSensor->getDistance() < 10) {
                pidWalker->stop();
                emoter->defaultSet(0);
                prizeArea->getPrize();
                prizeArea->carryPrize();
                break;
            }
        }
    }

    // pedestrian->monitor();
    // pedestrian->cross();


    /**********/
    /*Areaとcontrolをここで実行*/
    //while (1)
    //  area_control.update();
    /**********/



    /*---------------Main Task upto Here ここまで---------------*/

    walker->stop();
    lifter->defaultSet(0);
    emoter->defaultSet(0);
    lifter->terminate();
    emoter->terminate();

    msg_f("Reseted", 1);
    msg_f("", 2);
    // ter_tsk(BT_TASK);         // Bluetooth通信タスクの停止
    fclose(bt);

    ext_tsk();
}

//*****************************************************************************
// 関数名 : bt_task
// 引数 : unused
// 返り値 : なし
// 概要 : Bluetooth通信によるリモートスタート。 Tera Termなどのターミナルソフトから、
//        ASCIIコードで1を送信すると、リモートスタートする。
//        ASCIIコードで2を送信すると、リモートストップする。
//*****************************************************************************
void bt_task(intptr_t unused) {
    while(1) {
        uint8_t c = fgetc(bt); /* 受信 */
        switch(c) {
            case '1': // remote start
                bt_cmd = 1;
                break;
            case '2': // remote stop
                bt_cmd = 2;
                break;
            default:
                break;
        }
        fputc(c, bt); /* エコーバック */
    }
}
