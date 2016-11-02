#ifndef EASYEDIT_h
#define EASYEDIT_h
#include<Arduino.h>
#include<Nefry.h>
class easyEditer
{
public:
	void begin(String pageName);

	String setTrigger();
	String compileTestProgram();

	void setProgram(String program);
	String getProgram();
	
private:
	
	int convertPin(const char *s,int start),
		convertValue(char *s, int start, const char end, bool  highorlow =0, int high=1023, int low=0,bool check=0);

	int fastcount;
	String _edittext;
	String _page;
	String compile(bool run);

	int searchMode(const char* mode);
	int createCode(int mode,char* c,bool run);





};
#endif
