#include <iostream>
#include <curl/curl.h>
#include "Environment.h"

using namespace std;

int main () {

	E.setOption(CURLOPT_URL, "https://api.telegram.org/bot5709829084:AAGJ8LKXVB2H7_mb-5_PLoyHBkO9UiBgbeI/sendMessage?chat_id=-972743720&text=echo+from+a+container").execute();

	return 0;
}


