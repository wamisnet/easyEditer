#ifndef EASYEDIT_h
#define EASYEDIT_h
#include<Arduino.h>
#include<Nefry.h>
class easyEditer_lib
{
public:
	void begin(String filePath);
	void setProgram();
	void startProgram();
private:
	String edittext;
	void compile();
	int searchMode(const char* mode);
	void createCode(int m, const char* c);

	int pinString(String s);

	int stateString(String s);

};
#endif
