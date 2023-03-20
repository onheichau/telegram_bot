#include <iostream>
#include <curl/curl.h>
#include "Telegram.h"

using namespace std;

int main () {

	Telegram T;

	T.sendMsg("hello");

	return 0;
}


