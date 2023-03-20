#include "Telegram.h"
#include "Environment.h"



Telegram ::Telegram () {

}

Telegram ::~Telegram () {

}


const CURLcode Telegram::sendMsg(const String& msg) {
	String httpRequest = API + TG_REQUEST + CHAT_ID + (String)"text=" + msg;
	return E.setOption(CURLOPT_URL, httpRequest).execute();
}
