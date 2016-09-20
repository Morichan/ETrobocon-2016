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
#include "Clock.h"/*追加*/
#include "Walker.h"
#include "Flagman.h"
#include "PidWalker.h"
#include "Lifter.h"
#include "Emoter.h"
#include "PrizeArea.h"
#include "ColorChecker.h"
#include "Pedestrian.h"
#include "SonarSensor.h"
#include "ev3api.h"
/*自己位置推定*/
#include "self_localization.h"

#if defined(BUILD_MODULE)
#include "module_cfg.h"
#else
#include "kernel_cfg.h"
#endif

using namespace ev3api;

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
Clock* clock;
/*自己位置推定*/
Self_localization* self_localization;
/*自己位置推定*/
FILE *fp;
FILE *fp2;
FILE *fp3;
FILE *fp4;
void main_task(intptr_t unused) {
    pidWalker = new PidWalker();
    flagman = new Flagman();
    lifter = new Lifter();
    emoter = new Emoter();
    colorChecker = new ColorChecker();
    pedestrian = new Pedestrian();
    walker = new Walker();
    prizeArea = new PrizeArea();
    self_localization = new Self_localization();
    sonarSensor = new SonarSensor(PORT_3);
    clock = new Clock();
    walker = new Walker();
    /*自己位置関係*/
    fp = fopen("speed.txt","w");
    fp2 = fopen("direction.txt","w");
    fp3 = fopen("standard.txt","w");
    fp4 = fopen("angle.txt","w");
    int edge_direction = 1;//エッジの向きによって変更(+->右,-->左)
        int flag_edge1 = 0; //フラグNo.1
    //int flag_edge2 = 0; //フラグNo.2
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
    walker->reset();//必要???
    /*---------------Main Task from Here ここから---------------*/


   /*スタートダッシュ:motor:100まで加速*/
    /*
     使わない時はsetforwardからいじる(pidwoker)
     */
    pidWalker->startDash(70);
    while(1) {
        
            pidWalker->trace();        // PID（実質PD）制御でライントレースする
            

        
        
        /*自己位置のデータ更新*/
        self_localization->update(edge_direction);
        /*①基準地の更新*/
        self_localization->standard_point(6);//基準値を6point離れるごとに更新

        
        /*走行タスクの入力*/
/*--------------------------------------------------------------------------------------------------------------*/
        if (self_localization->near_target_coordinates(300, 0, 25,0) == 1 && flag_edge1 == 0) {
            ev3_speaker_play_tone(NOTE_E5, 20);
            /*減速*/
            pidWalker->setForward(50);
            pidWalker->setForward(30);
            
            flag_edge1 = 1;
        }
        
        if (self_localization->near_target_coordinates(310, -50, 25,0) == 1 && flag_edge1 == 1 ) {
            ev3_speaker_play_tone(NOTE_E5, 20);
            /*加速*/
            pidWalker->startDash(60);
            flag_edge1 = 2;
        }
        
        if (self_localization->near_target_coordinates(170, -65, 25,0) == 1) {
            ev3_speaker_play_tone(NOTE_E5, 20);
            /*減速*/
            pidWalker->setForward(45);
            

                    }
        if (self_localization->near_target_coordinates(125, -285, 25,0) == 1 && flag_edge1 == 2) {
            flag_edge1 = 3;
        }

        if (flag_edge1 == 3) {
            self_localization->update(edge_direction);
            /*①基準地の更新*/
            self_localization->standard_point(6);//基準値を6point離れるごとに更新
            ev3_speaker_play_tone(NOTE_E5, 20);
            /*ナビゲーション*/
            if (self_localization->navi(170,-320,30,0,1) == 1) {
                self_localization->writing_direction(fp2);
                flag_edge1 = 4;
            }
        

        }
        
        
        
        
/*--------------------------------------------------------------------------------------------------------------*/
        
        self_localization->writing_current_coordinates(fp);
        self_localization->writing_angle(fp4);
        
        /*ボタン検知したらSTOP*/
        if(ev3_button_is_pressed(BACK_BUTTON)) {
            break;
        }
        /*障害物検知したらSTOP*/
        if(sonarSensor->getDistance() < 10) {
            break;
        }
    }
    pidWalker->stop();
    // colorChecker->checkBlockColor();
    emoter->defaultSet(0);
    lifter->liftUp();
    // pedestrian->monitor();
    // pedestrian->cross();



    /*---------------Main Task upto Here ここまで---------------*/

    lifter->defaultSet(0);
    emoter->defaultSet(0);
    lifter->terminate();
    emoter->terminate();

    msg_f("Reseted", 1);
    msg_f("", 2);
    // ter_tsk(BT_TASK);         // Bluetooth通信タスクの停止
    fclose(bt);
    
    /*自己位置推定*/
    fclose(fp);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
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
