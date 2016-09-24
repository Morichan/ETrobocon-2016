#include "ev3api.h"
#include "ColorSensor.h"
#include "Clock.h"
#include "Puzzle.h"

#include "PidWalker.h"

//none:0 black:1 blue:2 green:3 yellow:4 red:5 white:6 brown:7

Puzzle::Puzzle():
    sonarSensor(PORT_3), colorSensor(PORT_2){
}

void Puzzle::doPuzzle(){
    /*
     * 12 ------ 8 ------ 4 ------ 0
     *  |        |        |        |
     *  |  blue  |        |   red  |
     *  |        |        |        |
     * 13 ------ 9 ------ 5 ------ 1
     *  |        |        |        |
     *  |        |        |        |   ---
     *  |        |        |        |      |
     * 14 ----- 10 ------ 6 ------ 2      |
     *  |        |        |        |      |
     *  | green  |        | yellow |      |
     *  |        |        |        |      |
     * 15 ----- 11 ------ 7 ------ 3    start
     */

    // ブロックの位置を指定してね
    // ちなみにこの順番で攻略していくよ
    // 0にブロックがある時は必ずblocks[0]に入れてね
    int blocks[4];
    blocks[0] = 1;
    blocks[1] = 6;
    blocks[2] = 12;
    blocks[3] = 14;

    int mv = -1;
    int setRoot[8] = {blocks[0], mv, blocks[1], mv, blocks[2], mv, blocks[3], mv};
    int maxRoot = 8;
    blockMovedCount = 0;

    bool blackBlockFlag = false; // 黒ブロックを動かしたかフラグ
    bool notMovedFlag = false;
    int blockMovedPlace = -1;
    int rootCount;

    pidWalker.walker.edgeChange();
    pidWalker.pid.setPid(0.4, 0.0, 1.0, 30);
    pidWalker.setForward(15);

    /*
     * 色を検知したら色の値をとって次に進む
     * 1 = 黒, 2 = 青, 3 = 緑, 4 = 黄, 5 = 赤, 6 = 白
     */
    goAheadNode();

    veryOldCirclePoint = 1;
    oldCirclePoint = 1;
    nowCirclePoint = 0;

    /*
     * Rコース難所
     * ブロックを検知し移動させる
     */
    for(int i = 0; i < maxRoot; ++i) {
        rootCount = 1;
        int start = nowCirclePoint;
        int goal = setRoot[i];
        blockColor = 0;
        blockMovingFlag = true;

        // explorer.set(0, 9);                 // スタート＆ゴールの設定
        // explorer.setBlocks(4, 2, 7, 14);    // ブロックの位置指定
        // explorer.search();                  // 探索開始
        // std::size_t s = explorer.getsize(); // 探索に必要なサイズを返す
        // int r = explorer.getRoot(3)         // 3番目に移動すべきノードを返す
        // 近くのブロックと同じ色のところまで持っていきたいときは
        // explorer->set(startの番号, explorer->goalExplore(startの番号, 色));
        // explorer->set(0, explorer->goalExplore(0, "green"));

        if(blockMovedCount == 0 && blocks[0] == 0) {
            ++i;
        }

        /*
         * ブロックの場所まで移動
         */
        if(setRoot[i] >= 0 && !(blockMovedCount == 0 && blocks[0] == 0)) {
            ev3_speaker_play_tone(NOTE_E5, 20);

            explorer.set(goal, start);
            explorer.setBlocks(blocks[0], blocks[1], blocks[2], blocks[3]);
            explorer.search();
            // ブロックを前回動かしていたらその位置を更新
            if(blockMovedPlace >= 0) {
                blocks[blockMovedCount - 1] = blockMovedPlace;
            }

            /*
             * ブロックを挟んで次の場所に行くのを防ぐ
             */
            if(nowCirclePoint - oldCirclePoint ==
                    explorer.getRoot(explorer.getSize() - 2) - nowCirclePoint) {
                nextCirclePoint =
                    nextPointAsBlockIsNow(blocks[0], blocks[1], blocks[2], blocks[3]);
                goNextPoint();
                goAheadNode();
                explorer.set(goal, nowCirclePoint);
                explorer.setBlocks(blocks[0], blocks[1], blocks[2], blocks[3]);
                explorer.search();
                backedFlag = false;
            }

            /*
             * ブロックの場所まで移動する
             * Explorer::search()はブロックの存在する場所までを検索できない
             * だからブロックの存在する場所"から"現在の場所までを検索し
             * ルートをひっくり返す必要がある
             */
            for(std::size_t j=1, rootSize = explorer.getSize(); j < rootSize; ++j) {
                // 斜め移動の走る距離を変更
                if(backedFlag) {
                    midTrace = 100;
                    backedFlag = false;
                }

                nextCirclePoint = explorer.getRoot((int)rootSize - rootCount - 1);
                goNextPoint();
                goAheadNode();
                midTrace = 200;
                rootCount++;
            }

            /*
             * ブロックを運搬
             */
        } else {
            colorChecker.checkBlockColor(currentCircleColor);
            int8_t blockColor = colorChecker.getColor();
            if(blockColor == 2) {
                explorer.set(start, explorer.goalExplore(start, "blue"));
            } else if(blockColor == 3) {
                explorer.set(start, explorer.goalExplore(start, "green"));
            } else if(blockColor == 4) {
                explorer.set(start, explorer.goalExplore(start, "yellow"));
            } else if(blockColor == 5) {
                explorer.set(start, explorer.goalExplore(start, "red"));
            } else { // 黒 or 下の色と同じもの
                // if(blockColor == 1) {
                //     blackBlockFlag = false;
                // }
                nextCirclePoint =
                    nextPointAsBlockIsNow(blocks[0], blocks[1], blocks[2], blocks[3]);
                goNextPoint();
                goAheadNode();
                notMovedFlag = true;
                blockMovedPlace = -1;
                blockMovedCount++;
                continue;
            }
            notMovedFlag = false;

            explorer.setBlocks(blocks[0], blocks[1], blocks[2], blocks[3]);
            explorer.search();
            goAheadNode();

            for(std::size_t j=1, rootSize = explorer.getSize(); j < rootSize; ++j) {
                nextCirclePoint = explorer.getRoot(rootCount);
                goNextPoint();
                goAheadNode();
                rootCount++;
            }
            // ブロックの場所を更新するための変数を保持
            blockMovedPlace = nowCirclePoint;
            blockMovedCount++;

            // 通称Walker::backMoveAngle()
            int defaultCountL = pidWalker.walker.get_count_L();
            pidWalker.walker.run(-20, 0);
            while(defaultCountL - pidWalker.walker.get_count_L() < 150) {
                clock.sleep(4);
            }
            pidWalker.walker.reset();
            backedFlag = true;
            // リフターを初期値に直す
            lifter.liftUp();
            lifter.defaultSet(0);
        }

        // ブロック3つ動かしたが、黒を検知してない時はもうゴールしてもいいよね？
        if(blockMovedCount == 3 && blackBlockFlag == true) {
            break;
        }
    }

    if(!notMovedFlag) {
        nextCirclePoint =
            nextPointAsBlockIsNow(blocks[0], blocks[1], blocks[2], blocks[3]);
        goNextPoint();
        goAheadNode();
    }

    int nearistGoal = 12;
    if(blockMovedPlace >= 0) {
        blocks[3] = blockMovedPlace;
    }

    /*
     * ゴールまでの線に最も近いノードを選択
     * 移動はしているから全てが12~15の何処かにあるということはないはず
     */
    for(int i = 0; i < 4; ++i) {
        if(blocks[0] != i + 12 && blocks[1] != i + 12 &&
                blocks[2] != i + 12 && blocks[3] != i + 12) {
            nearistGoal = i + 12;
            break;
        }
    }

    explorer.set(nowCirclePoint, nearistGoal);
    explorer.setBlocks(blocks[0], blocks[1], blocks[2], blocks[3]);
    explorer.search();

    rootCount = 1;
    for(std::size_t j=1, rootSize = explorer.getSize(); j < rootSize; ++j) {
        nextCirclePoint = explorer.getRoot(rootCount);
        goNextPoint();
        goAheadNode();
        rootCount++;
    }

    if(nowCirclePoint - oldCirclePoint == -1) {
        pidWalker.walker.moveAngle(20, 180);
        pidWalker.walker.angleChange(90, 1);
        if(nowCirclePoint == 12) {
            selfLocalizationStart = 2;
        } else if(nowCirclePoint == 13) {
            selfLocalizationStart = 4;
        } else {
            selfLocalizationStart = 6;
        }
    } else if(nowCirclePoint - oldCirclePoint == 1) {
        pidWalker.walker.moveAngle(20, 180);
        pidWalker.walker.angleChange(90, -1);
        if(nowCirclePoint == 13) {
            selfLocalizationStart = 2;
        } else if(nowCirclePoint == 14) {
            selfLocalizationStart = 4;
        } else {
            selfLocalizationStart = 6;
        }
    } else {
        if(nowCirclePoint == 12) {
            selfLocalizationStart = 1;
        } else if(nowCirclePoint == 13) {
            selfLocalizationStart = 3;
        } else if(nowCirclePoint == 14) {
            selfLocalizationStart = 5;
        } else {
            selfLocalizationStart = 7;
        }
    }
    while(colorSensor.getColorNumber() != 1) {
        pidWalker.walker.run(30, 0);
        clock.sleep(4);
    }

    pidWalker.walker.moveAngle(20, 60);
    pidWalker.walker.angleChange(90, -1);
}

int Puzzle::nextStartPoint() {
    return selfLocalizationStart;
}

int Puzzle::nextPointAsBlockIsNow(int b1, int b2, int b3, int b4) {
    int nowPoint  = nowCirclePoint  - oldCirclePoint;
    int nex[6] = {};
    int next = 0;
    int max = 0;

    if(nowPoint == 4) {
        if(nowCirclePoint % 4 == 0) {
            nex[0] = nowCirclePoint - 4;
            nex[1] = nowCirclePoint + 1;
            nex[2] = nowCirclePoint - 3;
            max = 3;
        } else if((nowCirclePoint + 1) % 4 == 0) {
            nex[0] = nowCirclePoint - 4;
            nex[1] = nowCirclePoint - 1;
            nex[2] = nowCirclePoint - 5;
            max = 3;
        } else {
            nex[0] = nowCirclePoint - 4;
            nex[1] = nowCirclePoint + 1;
            nex[2] = nowCirclePoint - 1;
            nex[3] = nowCirclePoint - 3;
            nex[4] = nowCirclePoint - 5;
            max = 5;
        }
    } else if(nowPoint == -4) {
        if(nowCirclePoint % 4 == 0) {
            nex[0] = nowCirclePoint + 4;
            nex[1] = nowCirclePoint + 1;
            nex[2] = nowCirclePoint + 5;
            max = 3;
        } else if((nowCirclePoint + 1) % 4 == 0) {
            nex[0] = nowCirclePoint + 4;
            nex[1] = nowCirclePoint - 1;
            nex[2] = nowCirclePoint + 3;
            max = 3;
        } else {
            nex[0] = nowCirclePoint + 4;
            nex[1] = nowCirclePoint + 1;
            nex[2] = nowCirclePoint - 1;
            nex[3] = nowCirclePoint + 3;
            nex[4] = nowCirclePoint + 5;
            max = 5;
        }
    } else if(nowPoint == 1) {
        if(nowCirclePoint >= 12) {
            nex[0] = nowCirclePoint - 1;
            nex[1] = nowCirclePoint - 4;
            nex[2] = nowCirclePoint - 5;
            max = 3;
        } else if(nowCirclePoint < 4) {
            nex[0] = nowCirclePoint - 1;
            nex[1] = nowCirclePoint + 4;
            nex[2] = nowCirclePoint + 3;
            max = 3;
        } else {
            nex[0] = nowCirclePoint - 1;
            nex[1] = nowCirclePoint - 4;
            nex[2] = nowCirclePoint + 4;
            nex[3] = nowCirclePoint + 3;
            nex[4] = nowCirclePoint - 5;
            max = 5;
        }
    } else if(nowPoint == -1) {
        if(nowCirclePoint >= 12) {
            nex[0] = nowCirclePoint + 1;
            nex[1] = nowCirclePoint - 4;
            nex[2] = nowCirclePoint - 3;
            max = 3;
        } else if(nowCirclePoint < 4) {
            nex[0] = nowCirclePoint + 1;
            nex[1] = nowCirclePoint + 4;
            nex[2] = nowCirclePoint + 5;
            max = 3;
        } else {
            nex[0] = nowCirclePoint + 1;
            nex[1] = nowCirclePoint - 4;
            nex[2] = nowCirclePoint + 4;
            nex[3] = nowCirclePoint - 3;
            nex[4] = nowCirclePoint + 5;
            max = 5;
        }
    }

    for(int i = 0; i < max; i++) {
        if(isThereBlock(b1, b2, b3, b4, nex[i])) {
            next = nex[i];
            break;
        }
    }

    return next;
}

bool Puzzle::isThereBlock(int b1, int b2, int b3, int b4, int block) {
    if((b1 != block) && (b2 != block) && (b3 != block) && (b4 != block)) {
        return true;
    }
    return false;
}

void Puzzle::goNextPoint() {
    int nowPoint  = nowCirclePoint  - oldCirclePoint;
    int nextPoint = nextCirclePoint - nowCirclePoint;

    /*
     * nowPointで向きを調べる
     * nextPointで動きを調べる
     */

    // （doPuzzleメソッド最初の図から見て）左向き
    if(nowPoint == 4) {
        if(nextPoint == 4) {         // （doPuzzleメソッド最初の図から見て）左へ移動
            goFrontEdge();
        } else if(nextPoint == -4) { // 右へ移動
            goBackEdge();
        } else if(nextPoint == 1) {  // 下へ移動
            goLeftEdge();
        } else if(nextPoint == -1) { // 上へ移動
            goRightEdge();
        } else if(nextPoint == 5) {  // 左下へ移動
            goUpLeftEdge();
        } else if(nextPoint == 3) {  // 左上へ移動
            goUpRightEdge();
        } else if(nextPoint == -5) { // 右上へ移動
            goLowRightEdge();
        } else if(nextPoint == -3) { // 右下へ移動
            goLowLeftEdge();
        }

        // 右向き
    } else if(nowPoint == -4) {
        if(nextPoint == 4) {         // 左へ移動
            goBackEdge();
        } else if(nextPoint == -4) { // 右へ移動
            goFrontEdge();
        } else if(nextPoint == 1) {  // 下へ移動
            goRightEdge();
        } else if(nextPoint == -1) { // 上へ移動
            goLeftEdge();
        } else if(nextPoint == 5) {  // 左下へ移動
            goLowRightEdge();
        } else if(nextPoint == 3) {  // 左上へ移動
            goLowLeftEdge();
        } else if(nextPoint == -5) { // 右上へ移動
            goUpLeftEdge();
        } else if(nextPoint == -3) { // 右下へ移動
            goUpRightEdge();
        }

        // 下向き
    } else if(nowPoint == 1) {
        if(nextPoint == 4) {         // 左へ移動
            goRightEdge();
        } else if(nextPoint == -4) { // 右へ移動
            goLeftEdge();
        } else if(nextPoint == 1) {  // 下へ移動
            goFrontEdge();
        } else if(nextPoint == -1) { // 上へ移動
            goBackEdge();
        } else if(nextPoint == 5) {  // 左下へ移動
            goUpRightEdge();
        } else if(nextPoint == 3) {  // 左上へ移動
            goLowRightEdge();
        } else if(nextPoint == -5) { // 右上へ移動
            goLowLeftEdge();
        } else if(nextPoint == -3) { // 右下へ移動
            goUpLeftEdge();
        }

        // 上向き
    } else if(nowPoint == -1) {
        if(nextPoint == 4) {         // 左へ移動
            goLeftEdge();
        } else if(nextPoint == -4) { // 右へ移動
            goRightEdge();
        } else if(nextPoint == 1) {  // 下へ移動
            goBackEdge();
        } else if(nextPoint == -1) { // 上へ移動
            goFrontEdge();
        } else if(nextPoint == 5) {  // 左下へ移動
            goLowLeftEdge();
        } else if(nextPoint == 3) {  // 左上へ移動
            goUpLeftEdge();
        } else if(nextPoint == -5) { // 右上へ移動
            goUpRightEdge();
        } else if(nextPoint == -3) { // 右下へ移動
            goLowRightEdge();
        }
    }

    // 前回の位置と今回の位置を更新
    veryOldCirclePoint = nowCirclePoint;
    if(nextPoint % 4 == 0 || nextPoint == 1 || nextPoint == -1) {
        oldCirclePoint = nowCirclePoint;
    } else {
        if(nowPoint % 4 == 0 && nextPoint > 0) {
            oldCirclePoint = nowCirclePoint + nextPoint - 4;
        } else if(nowPoint % 4 == 0 && nextPoint < 0) {
            oldCirclePoint = nowCirclePoint + nextPoint + 4;
        } else if((nowPoint + 1) % 2 == 0 && nextPoint > 0) {
            oldCirclePoint = nowCirclePoint + 4;
        } else if((nowPoint + 1) % 2 == 0 && nextPoint < 0) {
            oldCirclePoint = nowCirclePoint - 4;
        }
    }
    nowCirclePoint = nextCirclePoint;

}

void Puzzle::goAheadNode() {
    //黒線
    while(1){
        pidWalker.trace();
        if(colorSensor.getColorNumber() == 2) {
            currentCircleColor = 2;
            break;
        }
        else if(colorSensor.getColorNumber() == 3) {
            currentCircleColor = 3;
            break;
        }
        else if(colorSensor.getColorNumber() == 4) {
            currentCircleColor = 4;
            break;
        }
        else if(colorSensor.getColorNumber() == 5) {
            currentCircleColor = 5;
            break;
        }
    }
    pidWalker.walker.reset();
}

void Puzzle::goLeftEdge() {
    pidWalker.walker.angleChange(45, 1);
    while (colorSensor.getColorNumber() != 1) {
        pidWalker.walker.run(20,0);
    }
    pidWalker.walker.moveAngle(20, 100);
    pidWalker.walker.angleChange(45, 1);
}

void Puzzle::goRightEdge() {
    pidWalker.walker.angleChange(45, -1);
    while (colorSensor.getColorNumber() != 1) {
        pidWalker.walker.run(20,0);
    }
    pidWalker.walker.moveAngle(20, 50);
    pidWalker.walker.angleChange(45, -1);
}

void Puzzle::goFrontEdge() {
    //if(blockMovingFlag) {
        pidWalker.walker.moveAngle(20, 150);
    // } else {
    //     pidWalker.walker.angleChange(45, 1);
    //     pidWalker.walker.moveAngle(20, 360);
    //     pidWalker.walker.angleChange(90, -1);
    //     pidWalker.walker.moveAngle(20, 180);
    //     while(colorSensor.getColorNumber() != -1) {
    //         pidWalker.walker.run(20, 0);
    //     }
    //     pidWalker.walker.moveAngle(20, 100);
    //     pidWalker.walker.angleChange(45, 1);
    // }
}

void Puzzle::goBackEdge() {
    pidWalker.walker.moveAngle(20, 20);
    pidWalker.walker.angleChange(90, 1);
    pidWalker.walker.moveAngle(20, 40);
    pidWalker.walker.angleChange(45, 1);
    pidWalker.walker.moveAngle(10, 20);
    while(colorSensor.getColorNumber() != 1) {
        pidWalker.walker.run(0, 10);
    }
    pidWalker.walker.moveAngle(20, 60);
    pidWalker.walker.angleChange(30, -1);
}

void Puzzle::goLowRightEdge() {
    goRightEdge();
    pidWalker.moveAngle(midTrace);
    pidWalker.walker.angleChange(45, -1);
    while(colorSensor.getColorNumber() != 1) {
        pidWalker.walker.run(20, 0);
    }
    pidWalker.walker.moveAngle(20, 50);
    pidWalker.walker.angleChange(45, -1);
    pidWalker.accelerate(1, 20);
}

void Puzzle::goUpRightEdge() {
    goRightEdge();
    pidWalker.moveAngle(midTrace);
    pidWalker.walker.angleChange(45, 1);
    while(colorSensor.getColorNumber() != 1) {
        pidWalker.walker.run(20, 0);
    }
    pidWalker.walker.moveAngle(20, 100);
    pidWalker.walker.angleChange(45, 1);
    pidWalker.accelerate(1, 20);
}

void Puzzle::goLowLeftEdge() {
    goLeftEdge();
    pidWalker.moveAngle(midTrace);
    pidWalker.walker.angleChange(45, 1);
    while(colorSensor.getColorNumber() != 1) {
        pidWalker.walker.run(20, 0);
    }
    pidWalker.walker.moveAngle(20, 100);
    pidWalker.walker.angleChange(45, 1);
    pidWalker.accelerate(1, 20);
}

void Puzzle::goUpLeftEdge() {
    goLeftEdge();
    pidWalker.moveAngle(midTrace);
    pidWalker.walker.angleChange(45, -1);
    while(colorSensor.getColorNumber() != 1) {
        pidWalker.walker.run(20, 0);
    }
    pidWalker.walker.moveAngle(20, 50);
    pidWalker.walker.angleChange(45, -1);
    pidWalker.accelerate(1, 20);
}

void Puzzle::runOneBlock(){
    //黒線
    while(1){
        pidWalker.trace();
        if(colorSensor.getColorNumber() == 2) {
            currentCircleColor = 2;
            break;
        }
        else if(colorSensor.getColorNumber() == 3) {
            currentCircleColor = 3;
            break;
        }
        else if(colorSensor.getColorNumber() == 4) {
            currentCircleColor = 4;
            break;
        }
        else if(colorSensor.getColorNumber() == 5) {
            currentCircleColor = 5;
            break;
        }
    }

    if(currentCircleColor == 2){
        walker.run(10,0);
        clock.sleep(300);
        count_blue = count_blue + 1;
    }
    if(currentCircleColor == 3){
        walker.run(10,0);
        clock.sleep(300);
        count_green = count_green + 1;
    }
    if(currentCircleColor == 4){
        walker.run(10,0);
        clock.sleep(300);
        count_yellow = count_yellow + 1;
    }

    if(currentCircleColor == 5){
        walker.run(10,0);
        clock.sleep(300);
        count_red = count_red + 1;
    }

    if(count_blue == 2){
        while (colorSensor.getColorNumber() != 1) {
            walker.run(20,0);
        }
    }

    //〇の内円、白の領域
    while(colorSensor.getColorNumber() == 6){
        msg_f("second", 4);
        walker.run(10, 0);
        if(colorSensor.getColorNumber() == currentCircleColor)
            break;
    }
    //〇の線上、二回目
    while(colorSensor.getColorNumber() == currentCircleColor){
        msg_f("third", 5);
        walker.run(10, 7);
        if(colorSensor.getColorNumber() == 1)
            break; //黒線に入ったら
    }
    clock.sleep(800);

}

//円の中にたどり着いたら90度曲がる
void Puzzle::turn90InCircle(){
    while(1) {}
}



