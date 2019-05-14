#ifndef FXM_HPP_INCLUDED
#define FXM_HPP_INCLUDED


#include <cstdio>
#include <clocale>
#include <fstream>
#include <iostream>
#include <string>

#include <iomanip>
#include <cstring>
#include <sstream>
#include <stdlib.h>
#include <vector>

#include <algorithm>

#include "../LIB/LINUX/DPR.h"
#include "../LIB/MATH/MATH.h"

#include "VANTAGE_CSTM.h"
#include "QUANDL_CSTM.h"


#include "../LIB/PARSR/CSV.hpp"
#include "../LIB/FDR.hpp"

#include "../LIB/DBGU.hpp"

#include "CMD.hpp"


class FXM{///Financial external manager
public:
    
    FDR FACC;//CLOUD FILE, if not server is found local file is selected, FACC.IP.size()==0
    std::string FDBP="FDB";//Financial data base path

    CSV TMPD;//Temporal financial data, used for load, analisis and modify functions
    
    std::vector<std::string>unptick;//Contain the stock ticket of the data in the same order as FRD and FMD
    std::vector<std::string>STOCK_PATH;//Contain the stock path of the data in the same order as FRD and FMD
    std::vector<CSV> FRD;//Financial real data
    
    vantage av;
    std::string aF = "TIME_SERIES_DAILY";//TIME_SERIES_INTRADAY,TIME_SERIES_DAILY,TIME_SERIES_DAILY_ADJUSTED,TIME_SERIES_WEEKLY,TIME_SERIES_WEEKLY_ADJUSTED,TIME_SERIES_MONTHLY,TIME_SERIES_MONTHLY_ADJUSTED
    std::string aI="15min";//1min,5min,15min,30min,60min only for time series intraday
    std::string aO="full";//compact

    std::vector<std::string> quandlK={"qj5Rc-_sQuZBjN1SS8KT","Ly9gZRbgr3s9JSzN-B6U","pLa1HDFAFUNeuLFg2_6H","xy3z2nZDSTk4EzeTrzSM"};
    std::string IEX_KEY="luA53Jkl7G5";
    std::vector<std::string> vantageK={"BOBTQW93HBTTD0UL"/*,"JWYWRW408RJUYMWA","2D703CR5UA9U6TWG","ISHHA1DQJRQFZX0J","QDFILM22XZPSZ8S4"*/};
    int randomIndex=0;

    DATE DTS;//Time object to parse database dates
    DATE CT;//Current time
    
    FXM(){
        
    }
    //Simple functions, internal usage
    int STMA(std::string ticket){//Stock tiket memory avaliable. Returns the id of a stock ticket, return -1 if the ticket is not loaded.
    for(int i=0;i<unptick.size();i++){
        if(ticket==unptick[i]){
            return i;
        }
    }
    return -1;
}
    
    void DBSL(int dSs=1500){//Data base size limiter, selects the dSs first db entries and delete the rest
        std::vector<std::vector<double>> redV;
        for(int dbl=0;(dbl<dSs)&&(dbl<TMPD.DATA.size());dbl++){//Reduce the size of the data set if is bigger than dSs, big datasets gives low performance at 3 AGO 2018
            redV.push_back(TMPD.DATA[dbl]);
        }
        TMPD.DATA.clear();
        TMPD.DATA=redV;
    }
    
    void DTPRS(){//Date parser, converts dates to epoch and substitutes that on the loaded db
        DTS.ST(TMPD.ADATA[0][0]);//Needed to initialize DTS //Posible bad behaviour
        
        for(int dbl=0;dbl<TMPD.ADATA.size();dbl++){//Date parser
            if(TMPD.ADATA[dbl].size()>0){
                
                //Sets hour, minute, second info on datasets that does not contains it
                
                if(aF=="TIME_SERIES_DAILY"||aF=="TIME_SERIES_DAILY_ADJUSTED"){
                    TMPD.ADATA[dbl][0]=TMPD.ADATA[dbl][0]+" 22:00:00";
                }
                else if(aF=="TIME_SERIES_WEEKLY"||aF=="TIME_SERIES_WEEKLY_ADJUSTED"){
                    TMPD.ADATA[dbl][0]=TMPD.ADATA[dbl][0]+" 22:00:00";
                }
                else if(aF=="TIME_SERIES_MONTHLY"||aF=="TIME_SERIES_MONTHLY_ADJUSTED"){
                    TMPD.ADATA[dbl][0]=TMPD.ADATA[dbl][0]+" 22:00:00";
                }
                
                DTS.ST(TMPD.ADATA[dbl][0]);
                TMPD.DATA[dbl][0]=DTS.RTE();
            }
        }
        
        ////Future work if the provided date dont contain minute and seconds add a default one to have a consistent date on all data.////
    }
 
///Financial real-data functions
void DFD(std::string ticket){//Download financial data
        av=vantage();
        randomIndex = rand() % vantageK.size();
        av.function = aF;
        av.interv=aI;
        av.ouptsize=aO;
        av.auth(vantageK[randomIndex]); 
    
        av.get(ticket);
        TMPD.ADATA.clear();
        TMPD.DATA.clear();
        
        if(av.DATA.size()==0||av.DATA.empty()){//Bad data recived condition
            //DFD(ticket, recursion);//Recursion
            std::cout<<"Financial Server ERRORs"<<std::endl;
            DFD(ticket);//Recursion tries to download it again
        }
        
        else{//Data recived is correct

        //Parsing and procesing data for training
        TMPD.DEC(av.DATA);//CSV parser decoder
        TMPD.ADATA.erase(TMPD.ADATA.begin());//Delete the data description
        TMPD.DATA.erase(TMPD.DATA.begin());//Delete the data description
        
        DTPRS();//Parse the date
        
        DBSL();//Limits the db size
            
            /*//dbg
            DBG_STD(TMPD.DATA,"Data recived from external source");
            //*/
            /*
            DBG_STD(TMPD.ADATA,"Adata recived from external source");
            //dbg*/
        }
}

bool FDUC(std::string ticket, long timelapse=86400){//Financial data up to date checker true is up to date, false is old data
    
    if(TMPD.DATA.size()==0){//If the data is not loaded
    FACC.LF(FDBP+"/"+ticket+".csv");//Loads the current db
    TMPD.DEC(FACC.DATA);//CSV DECODE FILE DATA;
    
        if(TMPD.DATA.size()==0){//Is the data hasn't been sotoraged before
        //std::cout<<"ADDING NEW DB"<<std::endl;
        return false;
        }
    }
    
    CT.GT();//Current date
    //std::cout<<"Current Time: "<<CT.RTE()-timelapse<<std::endl;
    //std::cout<<"Last DB UDDD: "<<(long)(TMPD.DATA[0][0]+timelapse)<<std::endl;
    long ctt=(long)ceil((CT.RTE()/timelapse));//Cicles to real time
    long cdt=(long)ceil((TMPD.DATA[0][0]/timelapse));//Cicles to database time 
    
    //std::cout<<"Current Time: "<<ctt<<std::endl;
    //std::cout<<"Last DB UDDD: "<<cdt<<std::endl;
    
    if(ctt<=cdt){
       // std::cout<<"DB up to date"<<std::endl;
        return true;
        
    }
    else{
        
        //Checks date us market

        std::time_t time_temp = std::time(0);

        //Note: Return value of localtime is not threadsafe, because it might be
        // (and will be) reused in subsequent calls to std::localtime!
        const std::tm  time_out = *std::localtime(&time_temp);

        //Sunday == 0, Monday == 1, and so on ...
        //std::cout << "DAY: " << time_out.tm_wday <<" HOUR: "<<time_out.tm_hour<< "\n";
        // std::cout << "(Sunday is 0, Monday is 1, and so on...)\n";
  
        if((time_out.tm_wday==0)||(time_out.tm_wday==1)){//Checks weekend
            //std::cout<<"Weekend DB delayed"<<std::endl;
            return true;
        }
        if(time_out.tm_hour>14&&time_out.tm_hour<23){//Checks open market
            //std::cout<<"DB delayed OPEN MARKET US"<<std::endl;
            return true;
        }
        else{
            //Checks date us market
            //std::cout<<"DB delayed"<<std::endl;
            //DFD(ticket);
            return false;
        }
    }
}


void FDSR(std::string ticket, bool storage=true){//Financial data server, puts on CSV object the newest data avaliable and on file if storage flag is true
    int stid=STMA(ticket);//Checks if the ticket is loaded on memory and if so where.

    if( ticket.find("_")==std::string::npos){
        unptick.push_back(ticket);
        STOCK_PATH.push_back(FDBP+"/"+ticket+".csv");
        //stid=unptick.size()-1;
        
        TMPD.DATA.clear();
        TMPD.ADATA.clear();

    
    if(FDUC(ticket)){//Data is up to date, TMPD is already loaded to verify the last date
        FRD.push_back(TMPD); 
    }
    else{//Data is not up to date
        DFD(ticket);//Download new FINANCIAL DB
        FRD.push_back(TMPD);    

        if(storage){//Saves the db in case of storage=true;
            FACC.SF(STOCK_PATH[STOCK_PATH.size()-1],FRD[FRD.size()-1].ENC());//Saves the current data to file

            if(!FDUC(ticket)){//External source database is not up to date
                //std::cout<<"EXTERNAL DB IS DEAYED"<<std::endl;
            }
        }
    }
    //DBG_STD(TMPD.DATA,"Data loaded");
    //DBG_STD(FRD[FRD.size()-1].DATA,"FRD.DATA");
    }
    
}

//Simple functions, internal usage

//Vector functions


void FVDL(std::vector<std::string> tickets, bool storage=true){//Financial vector data loader, loads all ¡¡neded!! TICKETS databases to FRD-ram storage, saving delayed databases on the process
    FRD.clear();//Cleans the memory for new load inst
    for(int g=0;g<tickets.size();g++){
        FDSR(tickets[g],storage);
    }
}

///Financial real-data functions

///Financial data-mod functions

///Financial data-mod functions
};

#endif
