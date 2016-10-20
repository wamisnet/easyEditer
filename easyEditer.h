#ifndef EASYEDIT_h
#define EASYEDIT_h
#include<Arduino.h>
#include<Nefry.h>
class easyEditer_lib
{
public:
	void begin(String pageName);
	String setProgram();
	String compileTestProgram();
private:
	String _edittext;
	String _page;
	String compile(bool run);
	int searchMode(const char* mode);
	int stringTostate(int set, char * c);
	String stringTopin(int set, const char * c);
	int createCode(int mode,char* c,bool run);

	int pinString(String s);

	int stateString(String s);

	String escapeParameter(String param);

};
#endif
