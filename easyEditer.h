#ifndef EASYEDIT_h
#define EASYEDIT_h
#include<Arduino.h>
#include<Nefry.h>
class easyEditer
{
public:
	void begin(String pageName, ESP8266WebServer* webServer = Nefry.getWebServer());

	//これを呼ばれたときに書き込まれているプログラムを実行します。
	String setTrigger() { return compile(1); }
	//これを呼ばれたときに書き込まれているプログラムをコンパイルします。
	String compileProgram() { return compile(0); }
	//デフォルトプログラムを入れておくことができる
	void setProgram(String program) { _edittext = program; }
	//入力されたプログラムを確認することができる
	String getProgram() { return _edittext; }
	//プログラムの返り値を設定できる
	String getValue() { return returnString; }

private:

	int spt;
	String _edittext;
	String _page;
	String returnString;

	int convertPin(const char *s),
		convertValue(char *s, const char end, bool  highorlow = 0, int high = 1023, int low = 0, bool check = 0);

	String compile(bool run);

	int searchMode(const char* mode),
		createCode(int mode, char* c, bool run);

	bool convertIf(const char* s);





};
#endif
