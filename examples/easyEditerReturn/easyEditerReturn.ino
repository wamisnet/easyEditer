#include<easyEditer.h>
easyEditer editer;

/*
   easyEditerを試してみるサンプルプログラム
   2016/10/20
*/

void setup() {
  editer.begin("NefryEditer");//easyEditerを使えるようにします。
}

void loop() {
  if (Nefry.push_SW()) {
    //NefryのSWを押したときにeasyEditerを実行します。
    editer.setTrigger();//easyEditerを使えるようにします。
    Nefry.print(editer.getValue());
  }
}
