#include "easyEditer.h"


void easyEditer_lib::begin(String pageName)
{	
	_edittext = "";
	_page = pageName;
	Nefry.getWebServer()->on(("/"+_page).c_str(), [&]() {
		String content = F(
			"<!DOCTYPE HTML><html><head><meta charset=\"UTF-8\">"
			"<title>NefryEasyEditer</title><link rel = \"stylesheet\" type = \"text/css\" href = \"/nefry_css\">"
			"</head><body><div><h1>Nefry EasyEditer…");
		content += _page;
		content += F("</h1><form method='get' action='set_");
		content += _page;
		content += F("'><div class=\"row\"> <label for=\"ee\">Editer: </label> <div> <textarea name=\"ee\" rows=\"8\" cols=\"60\">");
		content += _edittext;
		content.replace(";", ";\r\n");
		content += F("</textarea> </div></div><div class=\"footer\"><button type = \"button\" onclick=\"location.href='/w");
		content += _page;
		content += F("'\">Test Program Start</button><input type=\"submit\" value=\"Save\"> </div></form><a href=\"/\">Back to top</a></div></body></html>");
		Nefry.getWebServer()->send(200, "text/html", content);
	});
	Nefry.getWebServer()->on(("/w" + _page).c_str(), [&]() {
		if(1){
			String content = F(
				"<!DOCTYPE HTML><html><head><meta charset=\"UTF-8\">"
				"<title>EasyEditer</title><link rel = \"stylesheet\" type = \"text/css\" href = \"/nefry_css\">"
				"</head><body><div><h1>");
			content += _page;
			content += F("…Test Program Start</h1>");
			content += F("<p>Compile Result : ");
			content += compileTestProgram();
			content += F("</p><a href=\"/");
			content += _page;
			content += F("\">Back to Editer</a><br><a href=\"/\">Back to top</a></div></body></html>");
			Nefry.getWebServer()->send(200, "text/html", content);
		}
		setProgram();
	});
	Nefry.getWebServer()->on(("/set_" + _page).c_str(), [&]() {
		_edittext= Nefry.getWebServer()->arg("ee");
		String content = F(
			"<!DOCTYPE HTML><html><head><meta charset=\"UTF-8\">"
			"<title>NefryEasyEditer</title><link rel = \"stylesheet\" type = \"text/css\" href = \"/nefry_css\">"
			"</head><body><div><h1>Save Nefry EasyEditer…");
		content += _page;
		content += F("</h1><p>");
		content += _edittext;

		_edittext.replace(" ", "");
		_edittext.replace("\r\n", "");

		content += F("</p><p>Compile Result : ");
		content += compileTestProgram();

		content += F("	</p><a href=\"/");
		content += _page;
		content += F("\">Back to NefryEasyEditer</a><br><a href=\"/\">Back to top</a></div></body></html>");
		_edittext.replace(" ", "");
		_edittext.replace("\r\n", "");
		Nefry.getWebServer()->send(200, "text/html", content);
	});
	Nefry.setIndexLink(_page.c_str(), ("/" + _page).c_str());
}

String easyEditer_lib::setProgram()
{
	return compile(1);
}

String easyEditer_lib::compileTestProgram()
{
	return compile(0);
}

String easyEditer_lib::compile(bool run)
{
	int  imode = -2;
	char *tok,buf[_edittext.length()];
	String err;
	_edittext.toCharArray(buf,_edittext.length());
	tok = strtok(buf, ";");
	while (tok != NULL) {
		Nefry.ndelay(1);
		if ((imode= searchMode(tok)) == -1) {
			err = tok;
			err += F(" : not found");
			return err;
		}
		switch (createCode(imode, tok, run)) {
		case -1:
			err = tok;
			err += F(" : first argument Err");
			return err;
			break;
		case -2:
			err = tok;
			err += F(" : The second argument Err");
			return err;
			break;
		case 0:
			break;
		}
		tok = strtok(NULL, ";");
	}
	return F("Complete");
}

int easyEditer_lib::searchMode(const char * mode)
{
	//if (!strncmp("digitalRead",mode,11))return 0;
	if (!strncmp("digitalWrite", mode,12))return 1;
	//if (!strncmp("analogRead", mode,10))return 2;
	if (!strncmp("analogWrite", mode,11))return 3;
	if (!strncmp("delay", mode,5))return 4;
	return -1;
}
int easyEditer_lib::stringTostate(int set,char *c) {
	int i;
	char *ret;
	String states;
	if ((ret = strchr(c, ')')) != NULL) {
		i = ret - c;
		for (int j = set; j < i; j++)states += c[j];
		return states.toInt();
	}
	else {
		return -1;
	}
}
String easyEditer_lib::stringTopin(int set,const char *c) {
	String pins;
	pins= c[set];
	pins += c[set+1];
	return pins;
}

int easyEditer_lib::createCode(int mode,  char * c, bool run)
{
	int state,pini;
	switch (mode)
	{
	case 1:
		if ((pini=pinString(stringTopin(13,c))) == -1)return -1;
		if((state=stateString((String)c[16]))==-1)return -2;

		if (run == 1) {
			pinMode(pini, OUTPUT);
			digitalWrite(pini, state);
		}

		//Nefry.println(state);
		//Nefry.println(pini);
		return 0;
		break;
	case 3:

		if ((pini = pinString(stringTopin(13, c))) == -1)return -1;
		if ((state = stringTostate(15, c)) == -1)return -2;

		if (run == 1) {
			analogWrite(pini, state);
		}

		//Nefry.println(state);
		//Nefry.println(pini);
		return 0;
		break;
	case 4:
		
		if ((state= stringTostate(6, c))==-1)return -1;

		if (run == 1) {
			//Nefry.setLed(255, 0, 0);
			Nefry.ndelay(state);
			//Nefry.setLed(255, 255, 0);
		}

		//Nefry.println(state);
		return 0;
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
	if (s.equals("h"))return 1;
	else if (s.equals("l"))return 0;
	else if (s.equals("0"))return 0;
	else if (s.equals("1"))return 1;
	return 0;
}

