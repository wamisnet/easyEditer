#include "easyEditer.h"
String functionPointer[] = {
	"digitalread",
	"digitalwrite",
	"analogread",
	"analogwrite",
	"delay",
	"nefry.setled",
	"if"
};
//関数を検索します。
int easyEditer::searchMode(const char * mode) {
	for (int i = 0; i < (sizeof functionPointer / sizeof functionPointer[0]); i++) {
		if (!strncmp(functionPointer[i].c_str(), mode, functionPointer[i].length()))return i;
	}
	return -1;
}


int easyEditer::createCode(int mode, char * c, bool run)
{
	int state, pini, st[5];
	char *ret;
	if ((ret = strchr(c, '(')) != NULL) {
		spt = ret - c + 1;
		switch (mode)
		{
		case 0:
			if ((pini = convertPin(c)) == -1)return -1;
			if (run == 1) {
				pinMode(pini, INPUT);
				returnString += digitalRead(pini);
				returnString += ":";
			}
			return 0;
			break;
		case 1:
			if ((pini = convertPin(c)) == -1)return -1;
			if ((state = convertValue(c, ')', 1)) == -1)return -2;

			if (run == 1) {
				pinMode(pini, OUTPUT);
				digitalWrite(pini, state);
			}
			//Nefry.println(state);
			//Nefry.println(pini);
			return 0;
			break;
		case 2:
			if (run == 1) {
				returnString += analogRead(A0);
				returnString += ":";
			}
			return 0;
			break;
		case 3:

			if ((pini = convertPin(c)) == -1)return -1;
			if ((state = convertValue(c, ')')) == -1)return -2;

			if (run == 1) {
				analogWrite(pini, state);
			}

			//Nefry.println(state);
			//Nefry.println(pini);
			return 0;
			break;
		case 4:

			if ((state = convertValue(c, ')', 0, 32767)) == -1)return -1;

			if (run == 1) {
				//Nefry.setLed(255, 0, 0);
				Nefry.ndelay(state);
				//Nefry.setLed(255, 255, 0);
			}
			//Nefry.println(state);
			return 0;
			break;
		case 5:
			if ((st[0] = convertValue(c, ',', 0, 255)) == -1)return -1;
			if ((st[1] = convertValue(c, ',', 0, 255)) == -1)return -2;
			if ((st[2] = convertValue(c, ')', 0, 255)) == -1)return -3;

			if (run == 1) {
				Nefry.setLed(st[0], st[1], st[2]);
			}

			return 0;
			break;
		case 6:

			return 0;
			break;
		default:
			return -1;
			break;
		}
	}
}

bool easyEditer::convertIf(const char * s)
{
	//if文の条件式を検索
	//else文をどうするか
	//flgをたてる方向で作成する
	//グローバル変数を1つ用意し、elseに入るときだけフラグをたてる
	char *ret;
	if ((ret = strstr(c, "==")) != NULL) {

	}

	return false;
}

//プログラムをコンパイルします。
String easyEditer::compile(bool run)
{
	int  imode = -2;
	String err;
	err = _edittext;
	err.toLowerCase();
	char *tok, buf[err.length()];
	err.toCharArray(buf, err.length());
	err = "";
	tok = strtok(buf, ";}");
	returnString = "";
	while (tok != NULL) {
		Nefry.ndelay(1);

		//関数を検索
		if ((imode = searchMode(tok)) == -1) {
			err = tok;
			err += F(" : not found");
			return err;
		}

		//実行結果を表示
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
		case -3:
			err = tok;
			err += F(" : The third argument Err");
			return err;
			break;
		case 0:
			break;
		}
		tok = strtok(NULL, ";}");
	}
	return F("Complete");
}

//プログラムの引数のピン番号を数値化する。
int easyEditer::convertPin(const char *s)
{
	String pins;
	pins = s[spt];
	pins += s[spt + 1];
	spt += 3;
	pins.toLowerCase();
	if (pins.equals("d0"))return D0;
	else if (pins.equals("d1"))return D1;
	else if (pins.equals("d2"))return D2;
	else if (pins.equals("d3"))return D3;
	else if (pins.equals("d4"))return D4;
	else if (pins.equals("d5"))return D5;
	return -1;
}

//プログラムの引数を数値化する。
//(検索元文字列,区切り文字,0or1,high,low,区切り無効化チェック)
int easyEditer::convertValue(char *s, const char end, bool highorlow, int high, int low, bool check)
{
	char *ret;
	String states;
	if (check == 1) {
		states = s;
	}
	else {
		if ((ret = strchr(s + spt, end)) != NULL) {
			int i = ret - s;//終了文字
			for (int j = spt; j < i; j++)states += s[j];
			spt = i + 1;
		}
		else {
			return -1;
		}
	}
	//Nefry.println(states);
	if (states.length() != 0) {
		if (highorlow == 1) {
			states.toLowerCase();
			if (states.equals("high"))return 1;
			else if (states.equals("low"))return 0;
			else if (states.equals("0"))return 0;
			else if (states.equals("1"))return 1;
			return 0;
		}
		else {
			if (low <= states.toInt())
				if (high >= states.toInt())
					return states.toInt();//文字のときは0を返す
				else
					return high;
			else
				return low;
		}
	}
	else return -1;
}

void easyEditer::begin(String pageName, ESP8266WebServer* webServer)
{
	_edittext = "";
	_page = pageName;
	webServer->on(("/" + _page).c_str(), [&]() {
		String content = F(
			"<!DOCTYPE HTML><html><head><meta charset=\"UTF-8\">"
			"<title>NefryEasyEditer</title><link rel = \"stylesheet\" type = \"text/css\" href = \"/nefry_css\">"
			"</head><body><div><h1>Nefry EasyEditer…");
		content += _page;
		content += F("</h1><form method='get' action='set_");
		content += _page;
		content += F("'><div class=\"row\"> <label for=\"ee\">Program Editer: </label> <div> <textarea name=\"ee\" rows=\"8\" cols=\"57\">");
		content += _edittext;
		content.replace(";", ";\r\n");
		content += F("</textarea> </div></div><div class=\"footer\"><button type = \"button\" onclick=\"location.href='/w");
		content += _page;
		content += F("'\">Program Start</button><input type=\"submit\" value=\"Save\"> </div></form><a href=\"/\">Back to top</a></div></body></html>");
		webServer->send(200, "text/html", content);
	});
	webServer->on(("/w" + _page).c_str(), [&]() {
		if (1) {
			String content = F(
				"<!DOCTYPE HTML><html><head><meta charset=\"UTF-8\">"
				"<title>EasyEditer</title><link rel = \"stylesheet\" type = \"text/css\" href = \"/nefry_css\">"
				"</head><body><div><h1>");
			content += _page;
			content += F("…Test Program Start</h1>");
			content += F("<p>Compile Result : ");
			content += compileProgram();
			content += F("</p><a href=\"/");
			content += _page;
			content += F("\">Back to Editer</a><br><a href=\"/\">Back to top</a></div></body></html>");
			webServer->send(200, "text/html", content);
		}
		setTrigger();
	});
	webServer->on(("/set_" + _page).c_str(), [&]() {
		_edittext = webServer->arg("ee");
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
		content += compileProgram();
		content += F("</p><a href=\"/w");
		content += _page;
		content += F("\">Program Start</a><br>");
		content += F("<a href=\"/");
		content += _page;
		content += F("\">Back to NefryEasyEditer</a><br><a href=\"/\">Back to top</a></div></body></html>");
		_edittext.replace(" ", "");
		_edittext.replace("\r\n", "");
		webServer->send(200, "text/html", content);
	});
	Nefry.setIndexLink(_page.c_str(), ("/" + _page).c_str());
}

