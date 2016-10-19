#include "easyEditer.h"


void easyEditer_lib::begin(String pageName)
{	
	_page = pageName;
	Nefry.getWebServer()->on(("/"+_page).c_str(), [&]() {
		Nefry.println(_page);
		String content = F(
			"<!DOCTYPE HTML><html><head><meta charset=\"UTF-8\">"
			"<title>NefryEasyEditer</title><link rel = \"stylesheet\" type = \"text/css\" href = \"/nefry_css\">"
			"</head><body><div><h1>Nefry EasyEditer…");
		content += _page;
		content += F("</h1><form method='get' action='set_");
		content += _page;
		content += F("'><div class=\"row\"> <label for=\"ee\">Editer: </label> <div> <input name=\"ee\" value=\"");
		content += _edittext;
		content += F("\"> </div></div><div class=\"footer\"><button type = \"button\" onclick=\"location.href='/w");
		content += _page;
		content += F("'\">Test Program Start</button><input type=\"submit\" value=\"Save\"> </div></form><a href=\"/\">Back to top</a></div></body></html>");
		Nefry.getWebServer()->send(200, "text/html", content);
	});
	Nefry.getWebServer()->on(("/w" + _page).c_str(), [&]() {
		Nefry.println(ESP.getFreeHeap());
		if(1){
			String content = F(
				"<!DOCTYPE HTML><html><head><meta charset=\"UTF-8\">"
				"<title>EasyEditer</title><link rel = \"stylesheet\" type = \"text/css\" href = \"/nefry_css\">"
				"</head><body><div><h1>");
			content += _page;
			content += F("…Test Program Start</h1><a href=\"/");
			content += _page;
			content += F("\">Back to Editer</a><br><a href=\"/\">Back to top</a></div></body></html>");
			Nefry.getWebServer()->send(200, "text/html", content);
		}
		Nefry.println(ESP.getFreeHeap());
		setProgram();
		Nefry.println(ESP.getFreeHeap());
	});
	Nefry.getWebServer()->on(("/set_" + _page).c_str(), [&]() {
		_edittext= escapeParameter(Nefry.getWebServer()->arg("ee"));
		String content = F(
			"<!DOCTYPE HTML><html><head><meta charset=\"UTF-8\">"
			"<title>NefryEasyEditer</title><link rel = \"stylesheet\" type = \"text/css\" href = \"/nefry_css\">"
			"</head><body><div><h1>Save Nefry EasyEditer…");
		content += _page;
		content += F("</h1><p>");
		content += _edittext;
		content += F("</p><a href=\"/");
		content += _page;
		content += F("\">Back to NefryEasyEditer</a><br><a href=\"/\">Back to top</a></div></body></html>");

		Nefry.getWebServer()->send(200, "text/html", content);
	});
	Nefry.setIndexLink(_page.c_str(), ("/" + _page).c_str());
}

void easyEditer_lib::setProgram()
{
	compile();
}

String easyEditer_lib::compile()
{
	int  imode = -2; // 文字列長さ
	char *tok,buf[200];
	_edittext.toCharArray(buf, 200);
	tok = strtok(buf, ";");
	while (tok != NULL) {
		//printf(tok);
		Nefry.ndelay(1);
		imode=searchMode(tok);
		Nefry.print("intmode : ");
		Nefry.println(imode);
		if (imode == -1) {
			String err = tok;
			err += " not found";
			return err;
		}
		Nefry.print("compile:");
		Nefry.println(ESP.getFreeHeap());
		createCode(imode, tok);
		tok = strtok(NULL, ";;");
	}
	
}

int easyEditer_lib::searchMode(const char * mode)
{
	Nefry.print("mode:");
	Nefry.println(mode);
	if (!strncmp("digitalRead",mode,11))return 0;
	if (!strncmp("digitalWrite", mode,12))return 1;
	if (!strncmp("analogRead", mode,10))return 2;
	if (!strncmp("analogWrite", mode,11))return 3;
	if (!strncmp("delay", mode,5))return 4;
	return -1;
}

bool easyEditer_lib::createCode(int m,  char * c)
{
	//char str[100];
	String pins = "D5", states;
	char *ret;
	int i;
	switch (m)
	{
	case 1:
		pins = c[13]; pins += c[14];
		Nefry.println(pins);
		states = c[16];
		Nefry.println(states);
		if (pinString(pins) == -1)return -1;
		pinMode(pinString(pins), OUTPUT);
		digitalWrite(pinString(pins),stateString(states));
		return 0;
		break;
	case 3:
		pins = c[12]; pins += c[13];
		Nefry.println(pins);
		if ((ret = strchr(c, ')')) != NULL) {
			i = ret - c;
			for (int j = 15; j < i; j++)states += c[j];
			Nefry.println(states.toInt());
		}
		else {
			return -1;
		}
		Nefry.println(states);
		if (pinString(pins) == -1)return -1;
		analogWrite(pinString(pins),states.toInt());
		return 0;
		break;
	case 4:
		if ((ret = strchr(c, ')')) != NULL) {
			i = ret - c;
			for (int j = 6; j < i; j++)states += c[j];
			Nefry.println(states.toInt());
		}
		else {
			return -1;
		}
		Nefry.setLed(255, 0, 0);
		Nefry.ndelay(states.toInt());
		Nefry.setLed(255, 255, 0);
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

String easyEditer_lib::escapeParameter(String param) {
	param.replace("%28", "(");
	param.replace("%29", ")");
	param.replace("%3B", ";");
	param.replace("%3C", "<");
	param.replace("%3D", "=");
	param.replace("%3E", ">");
	param.replace("%7B", "{");
	param.replace("%7D", "}");
	return param;
}