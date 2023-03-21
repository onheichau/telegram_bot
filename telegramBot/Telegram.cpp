#include <iostream>
#include "Telegram.h"
#include "Environment.h"





const CURLcode Telegram::sendMsg(const String& msg) {
	String httpRequest = API + TG_SEND_MSG + CHAT_ID + (String)"&text=" + msg;
	return E.setOption(CURLOPT_URL, httpRequest).execute();
}
