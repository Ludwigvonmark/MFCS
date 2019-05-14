/*
 * install libcurl dev
 * $ apt-get install libcurl4-gnutls-dev
 * 
ALPHA VANGATE API
BOBTQW93HBTTD0UL

https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=GOOG&outputsize=full&apikey=BOBTQW93HBTTD0U&datatype=csv
 */
//for compile: g++ xxx.cpp -std=c++11 -lcurl -o xxx

#ifndef VANTAGE_CSTM_H_
#define VANTAGE_CSTM_H_

#include <math.h>
#include <unistd.h>
#include "vector"
#include <limits>
#include <sys/wait.h>

#include <chrono>
#include <thread>

#include <stdio.h>
#include <curl/curl.h>
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

class vantage {
    public:
        std::string DATA;
        std::string function = "TIME_SERIES_DAILY";//TIME_SERIES_INTRADAY,TIME_SERIES_DAILY,TIME_SERIES_DAILY_ADJUSTED,TIME_SERIES_WEEKLY,TIME_SERIES_WEEKLY_ADJUSTED,TIME_SERIES_MONTHLY,TIME_SERIES_MONTHLY_ADJUSTED
        std::string interv="15min";//1min,5min,15min,30min,60min only for time series intraday
        std::string ouptsize="full";//compact
        std::string type = "csv";
        vantage(){

        };
        ~vantage(){};
		// To store the token in "AuthCode", which is a public variable in the class.
        void auth(std::string code){
            AuthCode = code;
        }

        void get(std::string code){
            std::string website;
            if(AuthCode.length() == 0){
                std::cout<<"No server key"<<std::endl;
                return;
            }
                        
            if(function=="TIME_SERIES_INTRADAY"){
                ouptsize="compact";//full                
                website = "https://www.alphavantage.co/query?function="+function+"&symbol="+code+"&interval="+interv+"&outputsize="+ouptsize+"&apikey="+AuthCode+"&datatype="+type;         
            }
            else{
                website = "https://www.alphavantage.co/query?function="+function+"&symbol="+code+ "&outputsize="+ouptsize+"&apikey="+AuthCode+"&datatype="+type;
            }

            download(website);
            return;
            }

	private: 
		std::string AuthCode;
		// This void function "download" is to download a file from internet.
		// The "download" function is developed with a general purpose of downloading
		// files from internet. It is based on the socket programming.
void download(std::string website){
    DATA.clear();
    std::string readBuffer;
CURL *curl;
  CURLcode res;
 
  //std::string readBuffer;
     // std::cout << "BUFEX" << std::endl;
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL,website.c_str());
    /* example.com is redirected, so we tell libcurl to follow redirection */ 
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK){
      fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
    }
    
    /* always cleanup */ 
    curl_easy_cleanup(curl);
    //      std::cout << "BUFEX" << std::endl;
    if((readBuffer.size()>0)&&(readBuffer.find("html")==std::string::npos)&&(readBuffer.find("}")==std::string::npos)&&(readBuffer.find("Error")==std::string::npos)){
         DATA=readBuffer;
         //std::cout << DATA << std::endl; 
    }
    else{
        std::cout << readBuffer << std::endl;
        std::cout << "Alpha Vantage Server Error on:"<<website<< std::endl;
        DATA.clear();
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
        download(website);//Recursive download at error
    }

  }
			
}
};

#endif /* QUANDL_H_ */
