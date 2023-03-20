#include <iostream>
#include "Telegram.h"
#include "Environment.h"



Telegram ::Telegram () {

}

Telegram ::~Telegram () {

}


const CURLcode Telegram::sendMsg(const String& msg) {
	String httpRequest = API + TG_SEND_MSG + CHAT_ID + (String)"&text=" + msg;
	std::cout << httpRequest;
	return E.setOption(CURLOPT_URL, httpRequest).execute();
}
