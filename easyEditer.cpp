#include "easyEditer.h"


String easyEditer_lib::compile()
{
	int imode=-2;
	char *tok;
	tok = strtok(edittext.c_str(), ";");
	while (tok != NULL) {
		printf("%s¥n", tok);
		imode=searchMode(tok);
		if (imode == -1) {
			String err = tok;
			err += " not found";
			return err;
		}
		createCode(imode, tok);
		tok = strtok(NULL, ";");
	}
	
}

int easyEditer_lib::searchMode(const char * mode)
{
	if (!strcmp("digitalRead"))return 0;
	if (!strcmp("digitalWrite"))return 1;
	if (!strcmp("analogRead"))return 2; 
	if (!strcmp("analogWrite"))return 3;
	if (!strcmp("delay"))return 4;
	return -1;
}

void easyEditer_lib::createCode(int m, const char * c)
{
	String str;
	int i;
	switch (m)
	{
	case 0:
		sscanf(c, "digiralRead(%s);", str);
		digiralRead(pinString(str));
		break;
	case 1:
		sscanf(c, "digitalWrite(%s,%d);", str,&i);
		digitalWrite(pinString(str),i);
		break;
	case 3:
		sscanf(c, "analogWrite(%s,%d);", str,&i);
		analogWrite(pinString(str),i);
		break;
	case 4:
		sscanf(c, "delay(%d);", &i);
		Nefry.ndelay(i);
		break;
	default:
		break;
	}
}

int easyEditer_lib::pinString(String s)
{
	s.toLowerCase();
	if (s.equals("d0"))return D0;
	else if (s.equals("d1"))return D1;
	else if (s.equals("d2"))return D2;
	else if (s.equals("d3"))return D3;
	else if (s.equals("d4"))return D4;
	else if (s.equals("d5"))return D5;
	return -1;
}

int easyEditer_lib::stateString(String s)
{
	s.toLowerCase();
	if (s.equals("high"))return 1;
	else if (s.equals("low"))return 0;
	else if (s.equals("0"))return 0;
	else if (s.equals("1"))return 1;
	return 0;
}