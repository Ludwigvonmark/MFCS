#ifndef NTC_HPP_INCLUDED
#define NTC_HPP_INCLUDED


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


class NTC{///Neural training compositor
public:
    std::vector<std::string>STOCK_TICKETS;//Contain the stock ticket of the data used

    std::vector<NRM> INORMS;//Normalizator, input data and functions.
    std::vector<NRM> ONORMS;//Normalizator, output data and functions.
    
    std::vector<CSV> REAL;//Real data, sufers modifications to acomodate diferent update rates, diferent update trigers and diferent history sizes.    
    std::vector<CSV> RESS;//Result data, modification for pertinent prediction data.    
    std::vector<CSV> MODI;//Modified data, modifications on last REAL to create the output vectors with striped data, and normalization modifiers
    
    std::vector<std::vector<double>> FTI;//Financial training input
    std::vector<std::vector<double>> FTO;//Financial training output
    std::vector<std::vector<double>> FTIR;//Financial training input reduzed, for training porpouses
    
    long epoch=60*60*24;//cicle epoch, time betwen data sets to construct nntd 
    int OSS=1;//Number of output data sets
    
    std::vector<int> inST={0,1,2,3,4,5};//Intput data set
    std::vector<int> outST={2,3,4};//Output data set. High, low, close
    
    bool ERR(){//Return true if an error is detected
        for(int t=0;t<REAL.size();t++){
            if(REAL[t].DATA.size()==0){
                std::cout<<"NTC_ERROR_REAL.DATA_EMPTY: "<<t+1<<"/"<<REAL.size()<<std::endl;
                return false;
            }
        }
        
        
        if(REAL.size()<(1+OSS)){
            std::cout<<"NTC_ERROR_REAL_SIZE: "<<REAL.size()<<std::endl;
            return true;//Error of incorrect configuration
        }
        else{
            return false;
        }
    }
    
    bool CHKR(std::vector<CSV> dtb){//Checker, checks problems on data sets, true if an error is detected
        
    }
    
    void PREP(){//Preprocessor
        if(ERR()){
            std::cout<<"NTC_DATA_PASSED_ERROR"<<std::endl;
            exit(-1);
            
        }
        else{//Susccsesful setup
        }
    }
    NTC(){}
    
    NTC(std::vector<std::string>stock_tickets,std::vector<CSV> real,int oss=1){
        STOCK_TICKETS=stock_tickets;
        REAL=real;
        OSS=oss;
        //DBG_STD(REAL[1].ADATA,"NTC_OREAL.DATA");
        //Preprocessor
        PREP();
        //Preprocessor
        
    }
    void ODS(){//Order the data sets alphabeticly, keepeng the input/output separated for consistent neural training data.
        std::vector<CSV> OREAL;
        //Creates the position vector for inputs
        //DBG_STD(REAL[0].DATA,STOCK_TICKETS[0]);
        
        std::vector<std::vector<std::string>>sttin;
        for(int u=0;u<STOCK_TICKETS.size()-OSS;u++){
            sttin.push_back({STOCK_TICKETS[u],std::to_string(u)});
        }
        std::sort(sttin.begin(), sttin.end(), [](const std::vector< std::string >& a, const std::vector< std::string >& b){ return a[0] < b[0]; } ); 
        
        //DBG_STD(sttin,"sttin");
        
        //Creates the position vector for inputs
        
        //Give alphabetical order to inputs
        for(int y=0;y<sttin.size();y++){
            //std::cout<<"sttin on for: "<<sttin[y][0]<<" "<<sttin[y][1]<<std::endl;
            OREAL.push_back(REAL[std::stoi(sttin[y][1])]);
            //OREAL[OREAL.size()-1].DATA=REAL[std::stoi(sttin[y][1])].DATA;
        }
        //DBG_STD(OREAL[0].DATA,STOCK_TICKETS[0]);
        //Give alphabetical order to inputs
        
        //Creates the position vector for outputs
        std::vector<std::vector<std::string>>sttou;
        for(int u=STOCK_TICKETS.size()-OSS;u<STOCK_TICKETS.size();u++){
            sttou.push_back({STOCK_TICKETS[u],std::to_string(u)});
        }
        std::sort(sttou.begin(), sttou.end(), [](const std::vector< std::string >& a, const std::vector< std::string >& b){ return a[0] < b[0]; } );
        //DBG_STD(sttou,"sttou");
        //Creates the position vector for outputs
        
        //Give alphabetical order to outputs
        for(int y=0;y<sttou.size();y++){
            OREAL.push_back(REAL[std::stoi(sttou[y][1])]);
           // OREAL[OREAL.size()-1].DATA=REAL[std::stoi(sttou[y][1])].DATA;
        }
        //Give alphabetical order to outputs
        REAL.clear();
        for(int r=0;r<OREAL.size();r++){
            REAL.push_back(OREAL[r]);
        }
        std::vector<std::string> TEMPSTK;
        for(int i=0;i<sttin.size();i++){
            TEMPSTK.push_back(sttin[i][0]);
        }
        for(int i=0;i<sttou.size();i++){
            TEMPSTK.push_back(sttou[i][0]);
        }
        STOCK_TICKETS=TEMPSTK;
        
        
        /*DBG_STD(STOCK_TICKETS,"STK:");
        std::cout<<"NTC_ODS_1; NTC_OREAL_SIZE: "<<REAL.size()<<std::endl;
        for(int rd=0;rd<REAL.size();rd++){
        DBG_STD(REAL[rd].DATA,STOCK_TICKETS[rd]);
        }*/
    }
    void PFPR(){//Prediction file preprocesor
        if(STOCK_TICKETS.size()!=REAL.size()){
            std::cout<<"PREDICTION AS INPUT DATA DETECTED"<<std::endl;
        }
        
    }
    
    void EQEX(){//Ecualize de first and last date on all data sets involved
       REAL=EQLM(REAL);
    }
    
    void EQSZ(){//Ecualize data sizes of all datasets with 0 grade interpolation
       REAL=EQDP(REAL,epoch);
    }
    
    void DIS(){//Disector, deletes data categories not used
        //Need for the data to be sort from older to newer, given the time constain nature of the financial predictions.
                for(int r=0;r<REAL.size();r++){
            REAL[r].DATA=Oasc(REAL[r].DATA);
        }
        
        std::vector<CSV> OREAL;
        //Input databases

        for(int i=0;i<REAL.size()-OSS;i++){
            OREAL.push_back(DSCT(REAL[i],inST));
            //DBG_STD(OREAL[i].DATA,STOCK_TICKETS[i]);
            }
            
        //Input databases
        
        //Output databases        
        for(int i=REAL.size()-OSS;i<REAL.size();i++){
            OREAL.push_back(DSCT(REAL[i],outST));
            //DBG_STD(OREAL[i].DATA,STOCK_TICKETS[i]);
        }
        //Output databases  
        
        REAL.clear();
        for(int r=0;r<OREAL.size();r++){
            REAL.push_back(OREAL[r]);
            //DBG_STD(REAL[r].DATA,STOCK_TICKETS[r]);
        }
        //On this point REAL contain the data secionated to the selected data, and with the exact same dates all entries, order from older to newer
        
    }
    
    void RVC(){//Result vector creator with selected investment rule
        //The current norm estipulates that the reaults to predict are the percentage changes from data set to data set 
        std::vector<CSV> OREAL;
        
        //Input databases modification
        for(int i=0;i<REAL.size()-OSS;i++){
            OREAL.push_back(REAL[i]);
             //DBG_STD(OREAL[i].DATA,STOCK_TICKETS[i]);
        }
        //Input databases modification*/
        
        //Output databases modification        
        for(int i=REAL.size()-OSS;i<REAL.size();i++){
            OREAL.push_back(CSV());
            for(int is=1;is<REAL[i].DATA.size();is++){
                std::vector <double>temp;
                for(int ins=0;ins<REAL[i].DATA[is].size();ins++){
                    temp.push_back(REAL[i].DATA[is][ins]-REAL[i].DATA[is-1][ins]);
                }
                OREAL[i].DATA.push_back(temp);
            }
            //DBG_STD(OREAL[i].DATA,STOCK_TICKETS[i]);
        }
        //Output databases modification 
        
        RESS.clear();
        for(int r=0;r<OREAL.size();r++){
            RESS.push_back(OREAL[r]);
            //DBG_STD(REAL[r].DATA,STOCK_TICKETS[r]);
        }
    }
    
    void NORM(){//Normalice datasets

        std::vector<CSV> MREAL;
        
        INORMS.clear();        
        //Normalize input vectors with date limiter
        
        //std::cout<<"RESS.size()-OSS= "<<REAL.size()-OSS<<std::endl;
        
        for(int r=0;r<RESS.size()-OSS;r++){
            MREAL.push_back(CSV());
            
           // DBG_STD(MREAL[r].DATA,STOCK_TICKETS[r]);
            MREAL[r].DATA=(TRS(RESS[r].DATA));
            //DBG_STD(MREAL[r].DATA,STOCK_TICKETS[r]);
            for(int dbl=0;dbl<MREAL[r].DATA.size();dbl++){//iterates over all data types, columns on the data base
                INORMS.push_back(NRM());
                INORMS[dbl].ONP(MREAL[r].DATA[dbl]);
            }
        //Change the maximun date for make room. This must have to be change on 2022
            INORMS[0].Nrt=1668470400;
            INORMS[0].Rgt=INORMS[0].Nrt-INORMS[0].Mnt;
        
            for(int dbl=0;dbl<MREAL[r].DATA.size();dbl++){//iterates over all data types, columns on the data base
                MREAL[r].DATA[dbl]=INORMS[dbl].RNV(MREAL[r].DATA[dbl]);
            }
        
            MREAL[r].DATA=TRS(MREAL[r].DATA);//Traspose the normalized vector
           
        }
        //Normalize input vectors with date limiter
        
        
        ONORMS.clear();   
        //Normalize output vectors
        for(int r=REAL.size()-OSS;r<REAL.size();r++){
           MREAL.push_back(CSV());
           MREAL[r].DATA=(TRS(RESS[r].DATA));
           // DBG_STD(REAL[r].DATA,STOCK_TICKETS[r]);
           for(int dbl=0;dbl<MREAL[r].DATA.size();dbl++){//iterates over all data types, columns on the data base
            ONORMS.push_back(NRM());
            ONORMS[dbl].ONP(MREAL[r].DATA[dbl]);
            //DBG
            //std::cout<<"ONORMS|| mx: "<<ONORMS[dbl].Nrt<<" mn: "<<ONORMS[dbl].Mnt<<" rg: "<<ONORMS[dbl].Rgt<<std::endl;
            //DBG
        }

        for(int dbl=0;dbl<MREAL[r].DATA.size();dbl++){//iterates over all data types, columns on the data base
            MREAL[r].DATA[dbl]=ONORMS[dbl].RNV(MREAL[r].DATA[dbl]);
        }
        
        MREAL[r].DATA=TRS(MREAL[r].DATA);//Traspose the normalized vector
           
        }
        //Normalize output vectors
        MODI.clear();
        for(int r=0;r<MREAL.size();r++){
            MODI.push_back(MREAL[r]);
            //DBG_STD(MODI[r].DATA,STOCK_TICKETS[r]);
        }

    }
    
    std::vector<std::vector<double>> RDNV(std::vector<std::vector<double>> nrdata){//Result denormalizer, using normalized configuration denormalizes given vector
    
        std::vector<std::vector<double>> ress;        
        std::vector<std::vector<double>> NRDATA=(TRS(nrdata));
        
        for(int r=0;r<ONORMS.size();r++){
            ress.push_back(ONORMS[r].RNV(NRDATA[r]));
        }
        
        ress=TRS(ress);
        return ress;
        
    }
    
        std::vector<double> RDN(std::vector<double> nrdata){//Result denormalizer, using normalized configuration denormalizes given vector
    
        std::vector<double> ress;        
        std::vector<double> NRDATA=nrdata;
        
        for(int r=0;r<ONORMS.size();r++){
            ress.push_back(ONORMS[r].RN(NRDATA[r]));
        }
        return ress;
        
    }
    
    void CTV(){//Compose training vector
        
        for(int r=0;r<MODI.size()-OSS;r++){
           // DBG_STD(REAL[r].DATA,STOCK_TICKETS[r]);
            MODI[r].DATA=(TRS(MODI[r].DATA));
           for(int dbl=0;dbl<MODI[r].DATA.size();dbl++){//iterates over all data types, columns on the data base
            if(r==0&&dbl==0){//Push bach dates
                FTI.push_back(MODI[r].DATA[dbl]);
            }
            else if(dbl!=0){
                FTI.push_back(MODI[r].DATA[dbl]);
            }
            
            }
        MODI[r].DATA=(TRS(MODI[r].DATA));
        }
        FTI=TRS(FTI);//Traspose the input vector
        //DBG_STD(FTI,"Input vector to nnt");

        FTIR=FTI;
        FTIR.pop_back();
        
        for(int r=MODI.size()-OSS;r<MODI.size();r++){
           // DBG_STD(REAL[r].DATA,STOCK_TICKETS[r]);
            MODI[r].DATA=(TRS(MODI[r].DATA));
           for(int dbl=0;dbl<MODI[r].DATA.size();dbl++){//iterates over all data types, columns on the data base

                FTO.push_back(MODI[r].DATA[dbl]);

            }
        MODI[r].DATA=(TRS(MODI[r].DATA));
        }
        FTO=TRS(FTO);//Traspose the output vector
        //DBG_STD(FTO,"Output vector to nnt");
    }
    
    void CTD(){//Composite training data, oss sets the number of stocks present on the output
        ODS();
        PFPR();
        EQEX();
        EQSZ();
        DIS();
        RVC();
        NORM();
        CTV();
        //DBG_STD(RDN(FTO),"DENORMALIZED RESULT: ");
    }
    
    
};

#endif
