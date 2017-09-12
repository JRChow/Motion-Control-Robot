#include <stdio.h>
#include <curl/curl.h>
#include <string>

#define FWD 2
#define BWD 1
#define LT 4
#define RT 5
#define STD 3
#define AUTO 6
#define CONT 7

void tempFunc(std::string url) {
	CURL *curl;
  	CURLcode res;

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    /* example.com is redirected, so we tell libcurl to follow redirection */ 
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
 
    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    /* always cleanup */ 
    curl_easy_cleanup(curl);
  }
}

int main(void)
{
	std::string url = "http://192.168.4.245:8080/?action=command&dest=1&plugin=0&group=1001&value=0&id=" + std::to_string(STD);
  	tempFunc(url);
  	return 0;
}


