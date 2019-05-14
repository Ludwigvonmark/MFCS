#ifndef FPE_HPP_INCLUDED
#define FPE_HPP_INCLUDED

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
#include "../LIB/NN/NN.hpp"

#include "NTC.hpp"
#include "FNM.hpp"
#include "FPM.hpp"//Financial prediction data manager

class FPE {//Financial prediction engine, neural network based
public:
    
uint LRit=25000;//Iterations for evolutive learning
uint LRrg=750000;//Relative gradient condition for evolutive learning


uint PRit=100;//Iterations for prediction learning
uint PRrg=100;//Relative gradient condition for evolutive learning

//Neural objects;
NNS NSC;
NNS NSB;
NNS NSV;
//Neural objects;

//Neural configuration data sets
CSV CNND;//Current neural network data
CSV BNND;//Best neural network data
CSV VNND;//Actual neural network data
//Neural configuration data sets

//Data for pass thougth neural networ
NTC FDTP;
//Data for pass thougth neural networ

double BMXE=std::numeric_limits<double>::max();//Best medium error, set to maximun to permit training
    
FPE(){

}
/*~MRFS{
    
}*/

void INN(FNM &NND){//Initialize the neural network

if(NND.BNC.DATA.size())//Corrobora que existe una red disponible para prediccion
BMXE=NND.BNC.DATA[0][1];
else BMXE=1e99;
//std::cout<<"DBG2.2"<<std::endl; 
NSC=NNS(FDTP.FTIR,FDTP.FTO);
if((FDTP.FTIR.size()&&NND.CNC.DATA.size())&&(FDTP.FTIR[0].size()==NND.CNC.DATA[1][0]))
NSC.SC.DATA=NND.CNC.DATA;//Puts the nnd recived fron the server
else{
    std::cout<<"DATA NOT NN COMPATIBLE"<<std::endl;
}
NSC.LND();//Load the nnd to the actual nn
NSC.INN();//Initialize de nn creating the nnd in case that is necesary

//std::cout<<"DBG2.3"<<std::endl; 

NSB=NNS(FDTP.FTIR,FDTP.FTO);
if((FDTP.FTIR.size())&&(NND.BNC.DATA.size())&&(FDTP.FTIR[0].size()==NND.BNC.DATA[1][0]))
NSB.SC.DATA=NND.BNC.DATA;//Puts the nnd recived fron the server
else{ //Si no existe red disponible compatible
    BMXE=1e99;
    std::cout<<"DATA NOT NN COMPATIBLE"<<std::endl;
}
NSB.LND();//Load the nnd to the actual nn
NSB.INN();//Initialize de nn creating the nnd in case that is necesary

//std::cout<<"DBG2.4"<<std::endl; 

NSV=NNS(FDTP.FTIR,FDTP.FTO);

if((FDTP.FTIR.size()&&NND.VNC.DATA.size())&&(FDTP.FTIR[0].size()==NND.VNC.DATA[1][0]))
NSV.SC.DATA=NND.VNC.DATA;//Puts the nnd recived fron the server
else {
    std::cout<<"DATA NOT NN COMPATIBLE"<<std::endl;
}
NSV.LND();//Load the nnd to the actual nn
NSV.INN();//Initialize de nn creating the nnd in case that is necesary
//std::cout<<"DBG2.5"<<std::endl; */
}

void LR(FNM &NND,bool evo=false){//Learning routines
        
    if(evo){ 
        NSC.EVO();
        NSC.DBG_NND();

        NSC.Lrn(1e-10,LRit,LRrg,1.001,0.05255);//Evolutive learning
                            std::cout<<std::setprecision(6);
        NSC.DBG_NND();
        std::cout<<FYEL("NSC.ME:");
        std::cout<<FYEL(NSC.ME);
        std::cout<<FYEL("< BMXE: ");
        std::cout<<FYEL(BMXE);
        std::cout<<std::endl;

        if((NSC.ME<BMXE)){//If the evolutive configuration is beter

            NSC.SND();
                    
            NND.BNC.DATA=NSC.SC.DATA;
            NND.VNC.DATA=NSC.SC.DATA;
                    
            NND.SNNC("V");
            NND.SNNC("B");

            BMXE=NSC.ME;    
            std::cout<<FGRN("BFNND SAVED");
            std::cout<<std::endl;
    
        }
            
        NSC.SND();
                    
        NND.CNC.DATA=NSC.SC.DATA;
        NND.SNNC("C");

    }
    else{
        //NLOG();
        NSV.DBG_NND();

        NSV.Lrn(1e-100,PRit,PRrg,1.001,0.05255);//Evolutive learning
                        std::cout<<std::setprecision(6);
       // NLOG();
        NSV.DBG_NND();

        //LLOG();
        NSV.SND();
                    
        NND.VNC.DATA=NSV.SC.DATA;
        NND.SNNC("V");
    }

}
void PR(FNM &NND,FPM &PRDT){//Prediction routines    
    std::cout<<std::setprecision(6);
    
    NSV.TDv=FDTP.FTI;
    PRDT.TMPD.DATA.clear();

        for (int sds=0;sds<NSV.TDv.size();sds++){
            NSV.UP(sds);
            NSV.Prd(sds);
            std::vector<double> tRPd=FDTP.RDN(NSV.NEURAL.NNOUT);
            std::vector<double> RPd;
            RPd.push_back(FDTP.RESS[0].DATA[sds][0]/*+FDTP.epoch*/);//Date of the data to make the predictión
            for(int osz=0;osz<tRPd.size();osz++){
                RPd.push_back(tRPd[osz]);
            }
            PRDT.TMPD.DATA.push_back(RPd);
        }
        //DBG_STD(PRDT.TMPD.DATA,"PRED:");

}

};

#endif
