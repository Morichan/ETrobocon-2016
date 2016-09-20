# ETrobocon-2016 tanakasample

こんにちは、Morichanです。  
ETrobocon2016で使われるであろうC++コードのサンプル、通称tanakasampleです。  

## コンパイルの方法

```
$ pwd  
.../ev3-beta6-2-release/hrp2/sdk/ETrobocon-2016  
$ ls  
README.md tanakasample Makefile tanakasample_cpp  
$ make app=tanakasample  
```
または、  

```
$ pwd  
.../ev3-beta6-2-release/hrp2/sdk  
$ ls  
workspace beginner ETrobocon-2016  
$ make app=ETrobocon-2016/tanakasample  
```
ここで何もエラーが発生しなければapp実行ファイルができます。  
それを任意の名前にするなり何なりして、HackEVのsdカードに入れます。  
下記の階層（appsディレクトリ内）にコピペしてください  

SDCARD:/ev3rt/apps/

## 担当箇所

* area, area_control : 立山
* PrizeArea, ColorChecker, Flagman, Emoter, Lifter, PidWalker, Walker : 森
* Pedestrian : 村上
* self_localization : 岩切
* pid : 去年の資産

***

### 更新履歴

* 2016.09.16 森 : 相撲完成、懸賞運び完成、PidWalkerにaccelerateとbrake追加。
* 2016.09.14 立山 : AreaとControl関係のファイル7つを追加、  
  	        それに合わせて、app.cfg, Makefile.incに追加したファイルの行を追記。
* 2016.09.14 森 : ETappみたいなものはやめました、みんなbranch作ってちょ。
* 2016.09.06 森 : ETrobocon-2016としての更新はひとまず休止、以降ETappの更新のみ行う。

