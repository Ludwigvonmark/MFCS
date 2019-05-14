/*
 * quandl(linux).h
 *
 *  Created on: 30/09/2013 (Updated on 10/09/2014) ((Revised on 15/08/2016))
 *      Author: Zhiwei Fu
 *      Revision: Jose Marqués Fernández    http://www.epigijon.uniovi.es/
 */

/* This programme is free software. It is developed by Dr Zhiwei Fu as a product
   contributing to quandl.com.
   This programme is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY, without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

//OpenSSL required, for install on ubuntu: sudo apt-get install libssl-dev  
//for compile: g++ xxx.cpp -std=c++11 -lssl -lcrypto -o xxx

#ifndef QUANDL_H_
#define QUANDL_H_
#include <iostream>
#include <fstream> //for perror()
#include <string> // string for c++
#include <string.h> // for memset()/bzero() For C
#include <sys/socket.h> 
#include <netinet/in.h> // for sockaddr_in, hotons
#include <arpa/inet.h> // for inet_addr
#include <netdb.h> //for gethostbyname(), gethostbyaddr()
#include <netinet/tcp.h> // for TCP_NODELAY
#include <time.h> // time.h
#include <iostream>       // std::cout, std::hex, std::dec
#include <string>         // std::string
#include <sstream>        // std::stringstream
#include <locale>         // std::locale, std::isxdigit

// for https
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <memory.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <stdarg.h>

#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

//using namespace std;

class quandl {
	public:
std::string DATA;
		quandl(){

                };
		~quandl(){};
		// To store the token in "AuthCode", which is a public variable in the class.
		void auth(std::string code){
			AuthCode = code;
		}

		//To download file from the website defined by the first 
		//argument.
		// To determine the website address by the token stored in "code"
		// and call the function "download"
		void get(std::string code){
			//Set optional variables by default
			std::string order = "asc"; 
			std::string type = "csv";

			std::string website = "https://www.quandl.com/api/v3/datasets/" + code 
				+ "." + type + "?sort_order=" + order;
			if(AuthCode.length() == 0){
				std::cout << "It would appear you are\'nt using an authentication"
				  << " token. Please visit https://www.quandl.com/help/c++"
				  << " or your usage may be limited.\n";
			}
			else{
				website += "&api_key=" + AuthCode;
			}

			std::string FileName;
			int iLength = code.length();
			for (int i=0; i < iLength; i++){
				if(code.substr(i, 1) == "/"){
					FileName = code.substr(i+1, iLength - i -1);
					break;
				}
			}

			download(website, FileName, type);
			return;
		}

		// All parameters are prescribed by users.
		// 1. Quandl code;
		// 2. Ascending/descending order;
		// 3. Start date;
		// 4. End date;
		// 5. Transformation;
		// 6. collapse;
		// 7. Rows;
		// 8. Output type
		// There are 7 optional arguments compared to the one above.
		void get(std::string code, std::string order, std::string StartDate, std::string EndDate,
			std::string transformation, std::string collapse, std::string rows, std::string type){

			std::string website = "https://www.quandl.com/api/v3/datasets/" + code 
				+ "." + type + "?sort_order=" + order;
			if(AuthCode.length() == 0){
				std::cout << "It appear you are\'nt using an authentication"
				  << " token. Please visit https://www.quandl.com/help/api for getting one"
				  << " ; otherwise your usage may be limited.\n";
			}
			else{
				website += "&api_key=" + AuthCode;
			}

			website += "&trim_start=" + StartDate;
			website += "&trim_end=" + EndDate;
			website += "&transformation=" + transformation;
			website += "&collapse=" + collapse;
			website += "&rows=" + rows;

			std::string FileName;
			int iLength = code.length();
			for (int i=0; i < iLength; i++){
				if(code.substr(i, 1) == "/"){
					FileName = code.substr(i+1, iLength - i -1);
					break;
				}
			}

			download(website, FileName, type);
		}
	private: 
		std::string AuthCode;
		// This void function "download" is to download a file from internet.
		// The "download" function is developed with a general purpose of downloading
		// files from internet. It is based on the socket programming.
		void download(std::string website, std::string FileName, std::string FileType){
DATA.clear();
                    struct sockaddr_in ServAddr;
                    struct hostent *ServInf;
                    int sockfd;
                    int iMessage = 5048576; //~ MB for the buff storage
                    char message[iMessage];
                    std::string request = "GET ";
                    std::ofstream fid;
                    int iStart, iEnd, iLength;
                    
                    SSL_CTX *ctx;
                    SSL *ssl;                 
                    OpenSSL_add_all_algorithms();
                    ERR_load_crypto_strings();
                    SSL_load_error_strings();
      
                    if(SSL_library_init() < 0){
                        std::cerr<<"OpenSSL error"<<std::endl;
                        return;
                    }
                    
                    ctx = SSL_CTX_new (SSLv23_client_method ());
                    SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);

                    //To define the server's address.
                    memset(&ServAddr, 0, sizeof(ServAddr));
                    std::string host;
                    iLength = website.length();
                    iStart = 0; iEnd = iLength - 1;
                    if(website.substr(0,8) == "https://") iStart = 8;
                    else if(website.substr(0,7) == "http://") iStart = 7;
                    for(int i = iStart; i<iLength; i++){
                        if(website.substr(i, 1)=="/"){
                            iEnd = i - 1;
                            break;
                        }
                    }
                    host = website.substr(iStart, iEnd - iStart + 1);

                    ServInf = gethostbyname(host.c_str());
                    if(ServInf == NULL){
                        std::cerr<<"HostName error"<<std::endl;
                        return;
                    }
                    
                    //Get a socket
                    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
                        std::cerr<<"Socket error"<<std::endl;
                        return;
                    }
                    
                    memset(&ServAddr, 0, sizeof(ServAddr));
                    ServAddr.sin_family = AF_INET;
                    ServAddr.sin_port = htons(443);
                    ServAddr.sin_addr.s_addr = *(long*)(ServInf->h_addr);

                    if ( connect(sockfd, (struct sockaddr*)&ServAddr, sizeof(ServAddr)) == -1 ) {
                       std::cerr<<"Conection error"<<std::endl;
                       return;
                    }

                    ssl = SSL_new(ctx); 
                    SSL_set_fd(ssl, sockfd);
    
                    SSL_connect(ssl);

                    // To send the request.
                    request += website + " HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n";                    
                    SSL_write(ssl, request.c_str(), request.length());

                    // To initiate the file.
                 /*   fid.open((FileName + "." + FileType).c_str());
                    if(fid.fail()){
                        std::cerr << "OpeningFile error" << std::endl;
                        return;
                    }*/

                    // To read dataflow for the file.
                    int iRecv = 1;                    
                    std::string RcvDt;
                    
                    while (iRecv != 0 && iRecv != -1){
                        memset(message, '\0', sizeof(message));
                       iRecv= SSL_read(ssl, message, iMessage);
                        RcvDt+=std::string(message);
                        
                    }
                    /*//DBG
                       std::cout<<"----------RcvDt------------"<<std::endl<<std::endl;
                        std::cout<<RcvDt;
                    //DBG*/
                    std::string initS="\r\n\r\n";
                    std::string servR="\r\n";
                    
                    std::size_t found = RcvDt.find(initS);
                    std::size_t found2 = RcvDt.find(initS,found+servR.size());
                    
                    
                    if (found!=std::string::npos){//Se elimina el mensage del servidor en este punto
                        RcvDt.erase(0,found+10);
                    }
                    
                    
                    std::locale loc;
                    for(int i=0;i<RcvDt.size()-8;i++){//Elimina las banderas intercaladas en los datos. WHILE no se está en el final del archivo y no se soncuentra antes del final
                        
                        if(RcvDt[i]==servR[0]&&RcvDt[i+1]==servR[1]){
                            for(int t=2;t<7;t++){//Busca banderas del servidor de tamaño 0 a 4 bites
                                if(RcvDt[i+t]==servR[0]&&RcvDt[i+t+1]==servR[1]){
                                    RcvDt.erase(i,2+t);
                                    break;
                                }
                            }
                        }
                        /*if(RcvDt[i]==servR[0]&&RcvDt[i+1]==servR[1]
                            &&RcvDt[i+6]==servR[0]&&RcvDt[i+7]==servR[1]
                            &&(std::isxdigit(RcvDt[i+2],loc))){
                            RcvDt.erase(i,8);
                        }
                        else if(RcvDt[i]==servR[0]&&RcvDt[i+1]==servR[1]
                            &&RcvDt[i+5]==servR[0]&&RcvDt[i+6]==servR[1])){
                            RcvDt.erase(i,7);
                        }*/
                        
                    }
                    
                    for(int i=0;i<RcvDt.size()-8;i++){//Elimina banderas falsas
                        if(RcvDt[i]==servR[0]&&RcvDt[i+1]==servR[1]){
                            RcvDt.erase(i,2);
                        }
                        
                    }
                    
                   /* while((found!=std::string::npos)&&(found<(RcvDt.size()-8))){// Elimina las banderas intercaladas en los datos. WHILE no se está en el final del archivo y no se soncuentra antes del final entonces:
                        std::locale loc;
                        found = RcvDt.find(servR);//marca la posición inicial de nueva bandera de linea recibida
                        found2 = RcvDt.find(servR,found+servR.size());//marca la posición final de nueva bandera de linea recibida
                        
                        if((found<(RcvDt.size()-8))&&(std::isxdigit(RcvDt[found+2],loc))/*&&((found2-found)>3)*//*){// Si encuentra la bandera en un sitio que no sea el final y despues de la bandera hay un hex
                            RcvDt.erase(found,8);//elimina la bandera y el hex
                            std::cout<<"FOUND ";
                        }

                    }*/
                    DATA+=RcvDt.c_str();
                    /*//DBG
                    std::cout<<"----------DATA------------"<<std::endl<<std::endl;
                    std::cout<<DATA;
                    //DBG*/
                    
                    //fid.write(RcvDt.c_str(), RcvDt.size());
                                       

                    // To close the downloaded file
                    //fid.close();
                    if(iRecv == -1){
                        std::cerr<<"SSL_read error"<<std::endl;
                        return;
                    }


                    SSL_free(ssl);
                    close(sockfd);
                    SSL_CTX_free(ctx);
                    

                    
			
		}
};

#endif /* QUANDL_H_ */
