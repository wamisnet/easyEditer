#ifndef EASYEDIT_h
#define EASYEDIT_h
#include<Arduino.h>
#include<Nefry.h>
class easyEditer_lib
{
public:
	void begin(String pageName);
	void setProgram();
	void startProgram();
private:
	String _edittext;
	String _page;
	String compile();
	int searchMode(const char* mode);
	bool createCode(int m,char* c);

	int pinString(String s);

	int stateString(String s);

	String escapeParameter(String param);

};
#endif
