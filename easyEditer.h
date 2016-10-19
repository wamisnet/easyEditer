#ifndef EASYEDIT_h
#define EASYEDIT_h
#include<Arduino.h>
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

};
#endif
