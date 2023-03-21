#include <stdio.h>
#include <curl/curl.h>


int main () {
	CURL* curl = curl_easy_init();
	//CURLcode result;


	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "https://api.telegram.org/bot5709829084:AAGJ8LKXVB2H7_mb-5_PLoyHBkO9UiBgbeI/sendMessage?chat_id=-972743720&text=echo+from+a+container");
		curl_easy_perform(curl);
	}

	curl_easy_cleanup(curl);
	return 0;
}
