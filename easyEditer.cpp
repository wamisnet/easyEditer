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
