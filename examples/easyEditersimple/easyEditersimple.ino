#include<easyEditer.h>
easyEditer editer;

/*
   easyEditerを試してみるサンプルプログラム
   2016/10/20
*/
ESP8266WebServer server;
void setup() {
  editer.begin("NefryEditer",server);//easyEditerを使えるようにします。
  pinMode(14,INPUT_PULLUP);
}

void loop() {
  if (digitalRead(14)==LOW) {
    //14pinがLOWになったときにeasyEditerを実行します。
    editer.setTrigger();//easyEditerを使えるようにします。
  }
}
