#ifndef FS_HPP_INCLUDED
#define FS_HPP_INCLUDED

//Financial prediction engine

#include <math.h>
#include <unistd.h>
#include "vector"
#include <limits>
#include <sys/wait.h>

#include <chrono>
#include <thread>

#include "../LIB/LINUX/STGU.h"
#include "../LIB/DBGU.hpp"
#include "../LIB/PARSR/CSV.hpp"


#include "FXM.hpp"//Financial external data manager
#include "FPM.hpp"//Financial prediction data manager
#include "FNM.hpp"//Financial neural data manager

#include "FSA.hpp"//Financial sistem analisis
#include "FIR.hpp"//Financial investment rules
#include "FPE.hpp"//Financial prediction engine


class FS {//Financial sistem, manages all data relative to a neural prediction sistem
public:
    //Configuration variables
    FCC CONF;
    //Configuration variables

    FXM RFD;//Real financial data
    FNM NFC;//Neural financial configurations
    FPM NNP;//Neural network predictions

    FPE NNST;//Neural network set

    FSA SA;//Sistem análisis object
    FIR IR;//Investment rules

    MPR FMPE;//Financial multy process executor
    

FS(){

}

//DBG FUNCTIONS
void DBG_SISTEM(){

}

//DBG FUNCTIONS

//Ticket parser
std::string FEEDBTICK(std::vector<std::string> tickets, int k){//Returns a vector with the tickets of the prediction files detected
    std::string FEEDBTICKS;
        if((tickets[k].find("%")!=std::string::npos)&&((tickets[k].find("[")==std::string::npos)&&(tickets[k].find("]")==std::string::npos))){//Uses the prediction with the same name as it self
            std::string feedbt="%";
            for(int q=0;q<tickets.size();q++){
                 if((tickets[q].find("%")==std::string::npos)||((tickets[q].find("[")!=std::string::npos)&&(tickets[q].find("]")!=std::string::npos))){//If the % is not present or finds "[" and "]"
                   feedbt=feedbt+"_"+tickets[q];  
                }
            }
            FEEDBTICKS=feedbt;
        }
        else{//No prediction tickets detected
            
        }
    //std::cout<<"FEEDBTICK: "<<FEEDBTICKS<<std::endl;
    return FEEDBTICKS;
}

std::string PREDTICK(std::vector<std::string> tickets, int k){//Returns a vector with the tickets of the prediction files detected
    std::string PREDTICKS;

        if((tickets[k].find("[")!=std::string::npos)&&(tickets[k].find("]")!=std::string::npos)){//Uses a prediction file ginven by name betwen []
            std::string predti=tickets[k];
            predti.erase(predti.begin());
            predti.erase(predti.end()-1);
            PREDTICKS=predti;
        }
        else{//No prediction tickets detected
            
        }
    //std::cout<<"PREDTICK: "<<PREDTICKS<<std::endl;
    return PREDTICKS;
}

std::string REALTICK(std::vector<std::string> tickets, int k){//Returns a vector with the tickets refered to a real databases
    std::string REALTICKS;

        if((tickets[k].find("%")==std::string::npos)&&(tickets[k].find("[")==std::string::npos)&&(tickets[k].find("]")==std::string::npos)){//A normal stock ticket is present
            std::string predti=tickets[k];
            REALTICKS=(predti);
        }
        else{//No normal stock tickets detected
            
        }

    //std::cout<<"REALTICK: "<<REALTICKS<<std::endl;
    return REALTICKS;
}
//Ticket parser

//FS initialization routines
void IAFD(){//Initialize avaliable financial data, gets file info about avaliable neural configurations
    NFC.NCF=CONF.NCF;//Sets the folder for get the neural config files
    NFC.GFN();
    NFC.NCFNP();
}


void IFPE(int o){//Initialize financial prediction engine, loading neural configurations and data for training and prediction
    NNST=FPE();//Set new prediction engine

    NNP.PDBF=CONF.PDBF;//Sets the folder for get the preditions files
    std::vector<CSV> NPD;//Neural data to pass to the prediction engine
    
    //Feed back push data
 
    for(int u=0;u<CONF.STOCK[o].size();u++){
        std::string feebb=FEEDBTICK(CONF.STOCK[o],u);
        std::string predd=PREDTICK(CONF.STOCK[o],u);
        std::string reall=REALTICK(CONF.STOCK[o],u);
        
        if(feebb.size()){//Prediction file identificator
            //STOCK[o][u].erase(STOCK[o][u].begin()+0);
            //std::cout<<"PREDTICK: "<<feebb<<std::endl;
            NNP.LPF(feebb);//Load the prediction file containing the prediction given for the stock set.
            NPD.push_back(NNP.TMPD);//The feedback data is placed the first one, and contains datasets whith the size of the output
            
        }
        else if(predd.size()){//Prediction file identificator
            //STOCK[o][u].erase(STOCK[o][u].begin()+0);
            //std::cout<<"PREDTICK: "<<predd<<std::endl;
            NNP.LPF(predd);//Load the prediction file containing the prediction given for the stock set.
            NPD.push_back(NNP.TMPD);//The feedback data is placed the first one, and contains datasets whith the size of the output
            
        }
        else if(reall.size()){//External sources file identificator
            //std::cout<<"REALTICK: "<<reall<<std::endl;
            RFD.FRD.clear();
            RFD.FDSR(reall);
            NPD.push_back(RFD.TMPD);
        }
    }
    
    //DBG_STD(STOCK,"STOCK_PROCESED:");
    //Neural network data for prediction/learning initialization
    NNST.FDTP=NTC(CONF.STOCK[o],NPD,CONF.OSS);
    NNST.FDTP.CTD();
    //Neural network data for prediction/learning initialization
    
    //Neural networks configurations initialization 
     NFC.LNNC(CONF.STOCK[o]);
    //Neural networks configurations initialization
    
    //FPE routines
    NNST.INN(NFC);
    
    //FPE routines
}
//FS initialization routines
void LEARN(int o,bool evv){
    NNST.LR(NFC,evv);
}

void PREDICT(int o){
    NNST.PR(NFC,NNP);
    for(int u=0;u<CONF.STOCK[o].size();u++){
        std::string feebb=FEEDBTICK(CONF.STOCK[o],u);
    
        if(feebb.size()){//Prediction file identificator
            //STOCK[o][u].erase(STOCK[o][u].begin()+0);
            //std::cout<<"PREDTICK: "<<feebb<<std::endl;
            NNP.SPF(feebb);//Load the prediction file containing the prediction given for the stock set.
            break;
        }
    }
    
    //NNST.PRDT.TMPD.DATA;
}

//FS execution routines
void FSER(bool evv){//Financial sistem execution routine
    //Checks existent neural networks configurations
    IAFD();
    //Checks existent neural networks configurations
    
    //Updates the financial databases and creates thr training data sets for all nn
    for(int o=0;o<CONF.STOCK.size();o++){
        IFPE(o);
        LEARN(o,evv);
        PREDICT(o);
    }
    //Updates the financial databases and creates thr training data sets for all nn
}
void FSPR(){//Financial sistem execution routine
    //Checks existent neural networks configurations
    IAFD();
    //Checks existent neural networks configurations
    
    //Updates the financial databases and creates thr training data sets for all nn
    for(int o=0;o<CONF.STOCK.size();o++){
        IFPE(o);
        PREDICT(o);
    }
    //Updates the financial databases and creates thr training data sets for all nn
}
//FS execution routines

//FS análsis tools
void WTCH(int o=0){//Watch routine, prints on screen the prediction files with statistical info.
        //std::cout<<"FS_WTCH(); INITLIALIZE"<<std::endl;
        NNP.PDBF=CONF.PDBF;//Sets the folder for get the preditions files
        IR.PRFIR(NNP,RFD,CONF.OSS,CONF.epoch,CONF.outST);
}
//FS análsis tools

};

#endif
