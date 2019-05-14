#ifndef FIR_HPP_INCLUDED
#define FIR_HPP_INCLUDED
//Financial invesment rules.

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

#include "../LIB/PARSR/CSV.hpp"//CSV parser
#include "../LIB/FDR.hpp"//File door

class FIR {//Financial invesment rule, for simulation, eficiency analisis and operations
public:
    long TPS=60*60*24;//Time in seconds of the intended prediction.
    
    std::string STKN;
    
    std::vector <double> LKP;//Last known price
    std::vector <double> LKA;//Last known maximun price
    std::vector <double> LKI;//Last known minimun price
    
    std::vector <double> LPP;//Last prediction file
    std::vector <double> LPA;//Last prediction maximun price
    std::vector <double> LPI;//Last prediction minimun price
    
    //prediction and known refeers to the same day. 
    
    double intrs=0.2;//Interest percentage, lost per operatión.

    /*double S=0;//Sell performance
    double B=0;//Buy performance
    double SL=0;//Sell losses
    double BL=0;//Buy losses
    double SE=0;//Sell earnings
    double BE=0;//Buy earnings
    double MXSE=0;//Maximun sell earnings
    double MXBE=0;//Maximun buy earnings
    
    double E=0;//Earnings
    double MXE=0;//Maximun posible earnings*/
    
    std::vector<CSV> FPRD;//financial prediction results + real data loaded "RAW DATA"
    
    //Modified data sets
    std::vector<CSV> RPRE;//Real data prediction encoded. Use for get error of prediction;
    CSV PRER;//Prediction errors;
    std::vector <double> MEER;//medium error
    std::vector <double> STDV;//standar deviation
    
    std::vector<CSV> RSCE;//Real data sectioned, used to get PRRE and PRDE;
    
    CSV PRRE;//Prediction data with real data encode, pseudo prediction integral,use for overprice or under price detection
    CSV PRDE;//Prediction data with decision data encode, combined real predition data for direct investment.
    //Modified data sets
    
    //Temporal data structures
    std::vector<CSV> TMP;//Prediction errors;
    //Temporal data structures
    
    /*double SAT=0;//Sell at price
    double SATSD=0;//Sell at price standar deviation.
    double BAT=0;//Buy at price op
    double BATSD=0;//Buy at price standar deviation.
    double CAT=0;//Cloase at price, buy operation as well as sell operation
    double CATSD=0;//Close at price standar deviation.*/
    
    FIR(){}
    
    CSV CONC(std::vector<CSV> conca){//Concatenates real data given
        //Compose a vector with all real data entries concatenated
        CSV TMPRDV;//Temporal real data vector
        if(conca.size()&&conca[0].DATA.size()&&conca[0].DATA[0].size()){
        
        for(int h=0;h<conca[0].DATA.size();h++){
            std::vector<double> tempdr;
            for(int g=0;g<conca.size()-1;g++){
                for(int s=0;s<conca[0].DATA[0].size();s++){
                    tempdr.push_back(conca[g].DATA[h][s]);
                }
            }
            TMPRDV.DATA.push_back(tempdr);
            
        }
        }
        else{
            std::cout<<"FIR_CONC ERROR"<<std::endl;
        }
        return TMPRDV;
        //DBG_STD(TMPRDV.DATA,"TMPRDV.DATA;");
        //Compose a vector with all real data entries concatenated
    }
    
    void GPP(FPM &PRED, FXM &RELD, int oss){//Get prediction files path
        PRED.GFN();
        PRED.PFNP();
    }
    
    void LPXD(FPM &PRED, FXM &RELD,int j,int oss){//Load preediction and external db
        if(j<PRED.PTN.size()){

            RELD.FRD.clear();//Deletes prior external loaded data
            for(int u=PRED.PTN[j].size()-oss;u<PRED.PTN[j].size();u++){//First load real data
                RELD.FDSR(PRED.PTN[j][u]);
                FPRD.push_back(RELD.TMPD);
                //std::cout<<"FIR_LPXD PRED.PTN[j][u]:"<<PRED.PTN[j][u]<<std::endl;
            }
            
            PRED.LPF(PRED.PTN[j]);//Loads [j] prediction file
            //DBG_STD(PRED.PTN[j],"FIR_LPXD PRED.PTN[j][u]:");
            FPRD.push_back(PRED.TMPD);
            //DBG_STD(PRRD.DATA,"PRRD.DATA");
            
           
        }
        else{//Error
            std::cout<<"FIR_LPXD j_ERROR"<<std::endl;
        }
    } 
    
    void EQDSTS(long epoch){//Equalize date of all data sets and creates the date vector;
        FPRD=EQLM(FPRD);
        FPRD=EQDP(FPRD,epoch);
    }
    
    void SRDS(){//Sorts data sets to be organized from old to new
        for(int k=0;k<FPRD.size();k++){
            if(ascO(FPRD[k].DATA)){//Prediction file with order old first
            }
            else{
                FPRD[k].DATA=Oasc(FPRD[k].DATA);
            }
            //DBG_STD(FPRD[k].DATA,"FPRD[k].DATA:");
        }
    }
    
    void DISECTOR(std::vector<int> outST){//Disects the data sets to the required size
        //RPRE;
        for(int h=0;h<FPRD.size()-1;h++){//Disects external datasets, and saves it on the pertinetn vectors, no date is saved
            RPRE.push_back(DSCT(FPRD[h],outST));
        }
        
        RPRE.push_back(DSCT(FPRD[FPRD.size()-1],{1,2,3}));//Saves the result vector 
        //DBG_STD(RPRE[i].DATA,"FIR_RPRE[i];");
        RSCE=RPRE;
        
        /*for(int k=0;k<RSCE.size()-1;k++){
             DBG_STD(RSCE[k].DATA[RSCE[k].DATA.size()-1],"FIR_RSCE[k];");
        }*/
        
    }
    
    void CRPRE(){//Compute and construct real data prediction encoding. Use for get error of prediction;
        TMP.clear();
        for(int i=0;i<RPRE.size()-1;i++){//Modifies the exteral data sets.
            TMP.push_back(CSV());
            for(int is=1;is<RPRE[i].DATA.size();is++){
                std::vector <double>temp;
                for(int ins=0;ins<RPRE[i].DATA[is].size();ins++){
                    temp.push_back(RPRE[i].DATA[is][ins]-RPRE[i].DATA[is-1][ins]);
                }
                
                TMP[i].DATA.push_back(temp);
                
            }
            
            
        }
        
        for(int k=0;k<TMP.size();k++){
                RPRE[k]=TMP[k];
            }
            TMP.clear();
        
        /*for(int k=0;k<RPRE.size()-1;k++){
             DBG_STD(RPRE[k].DATA[RPRE[k].DATA.size()-1],"FIR_RPRE[k];");
        }*/
            
    }

    void CPRER(){//Calculates prediction error and statistical info
        //Compose a vector with all real data entries concatenated
        if(RPRE.size()&&RPRE[0].DATA.size()&&RPRE[0].DATA[0].size()){
        CSV TMPRDV=CONC(RPRE);//Temporal real data vector
        
        //DBG_STD(TMPRDV.DATA,"TMPRDV.DATA;");
        //Compose a vector with all real data entries concatenated
        
        //Calculates diference
        for(int d=0;d<RPRE[RPRE.size()-1].DATA.size()-1;d++){//Iterates over all data entries on the result vector
            std::vector<double> tempd;
            for(int r=0;r<RPRE[RPRE.size()-1].DATA[d].size();r++){//Iterates over all data points on the current data entry
                tempd.push_back(RPRE[RPRE.size()-1].DATA[d][r]-TMPRDV.DATA[d][r]);//Distance from prediction to real.
            }
            PRER.DATA.push_back(tempd);
        }
        //DBG_STD(PRER.DATA,"PRER.DATA;");
        //Calculates diference*/
        }
        else{
            std::cout<<"FIR_CPRER ERROR"<<std::endl;
        }
        }
        
        
    
    void ERREST(){//Calculates statistical info about predition error
        if(PRER.DATA.size()&&PRER.DATA[0].size()){
            MEER.clear();
            STDV.clear();
            for(int h=0;h<PRER.DATA[0].size();h++){
                std::vector<double> erdt;
                for(int w=0;w<PRER.DATA.size();w++){
                    erdt.push_back(PRER.DATA[w][h]);
                }
                MEER.push_back(MEAN(erdt));
                STDV.push_back(ST_DEV(erdt));
                
            }
            //DBG_STD(MEER,"MEER:");
            //DBG_STD(STDV,"STDV:");
        }
    }
    
    void CRDRV(){//Creates prediction real data encoder blind result vector
        PRRE.DATA.clear();
        
       if(RSCE.size()&&RSCE[RSCE.size()-1].DATA.size()&&RSCE[RSCE.size()-1].DATA[0].size()){
           
            CSV TMPRDV=CONC(RSCE);//Temporal real data vector
            //DBG_STD(TMPRDV.DATA,"TMPRDV.DATA:");
            
            std::vector<double> tmped;
            for(int g=0;g<RSCE[RSCE.size()-1].DATA[0].size();g++){//Initialize series
                tmped.push_back(TMPRDV.DATA[0][g]+RSCE[RSCE.size()-1].DATA[0][g]);
            }
            PRRE.DATA.push_back(tmped);//Initialize series
            
            for(int h=0;h<RSCE[RSCE.size()-1].DATA.size()-1;h++){
                tmped.clear();
                for(int g=0;g<RSCE[RSCE.size()-1].DATA[h].size();g++){                    
                    tmped.push_back(PRRE.DATA[h][g]+RSCE[RSCE.size()-1].DATA[h+1][g]);
                }
                PRRE.DATA.push_back(tmped);
            }
            //DBG_STD(PRRE.DATA[PRRE.DATA.size()-1],"PRRE.DATA:");
        }
        else{
            std::cout<<"FIR_CRDRV ERROR"<<std::endl;
        }
    }
    
        void CRRV(){//Creates prediction real data encoder result vector
        PRDE.DATA.clear();
        
       if(RSCE.size()&&RSCE[RSCE.size()-1].DATA.size()&&RSCE[RSCE.size()-1].DATA[0].size()){
            CSV TMPRDV=CONC(RSCE);//Temporal real data vector
            //DBG_STD(TMPRDV.DATA,"TMPRDV.DATA:");
            
            std::vector<double> tmped;
            for(int h=0;h<RSCE[RSCE.size()-1].DATA.size();h++){
                tmped.clear();
                for(int g=0;g<RSCE[RSCE.size()-1].DATA[h].size();g++){                    
                    tmped.push_back(TMPRDV.DATA[h][g]+RSCE[RSCE.size()-1].DATA[h][g]);
                }
                PRDE.DATA.push_back(tmped);
            }
            //DBG_STD(PRDE.DATA[PRDE.DATA.size()-1],"PRDE.DATA:");
        }
        else{
            std::cout<<"FIR_CRDRV ERROR"<<std::endl;
        }
    }
    
    void OPCL(){//Operation calculator
       /* CSV TMPRDV=CONC(RSCE);
        for(int i=0;i<TMPRDV.DATA.size();i++){
            for(int g=0;g<TMPRDV.DATA[i].size())
            if(){
                
            }
        }*/
    }
    
    void LOG(){
        std::cout<<BWHT("                             ");
        std::cout<<std::endl;
        
        DBG_STD(MEER,"MEER:");
        DBG_STD(STDV,"STDV:");
        std::cout<<std::endl;
        
        for(int k=0;k<RSCE.size()-1;k++){
             DBG_STD(RSCE[k].DATA[RSCE[k].DATA.size()-1],"FIR_RSCE[k];");
        }
        DBG_STD(PRRE.DATA[PRRE.DATA.size()-1],"PRRE.DATA:");
        DBG_STD(PRDE.DATA[PRDE.DATA.size()-1],"PRDE.DATA:");
        
        std::cout<<BWHT("                             ");
        std::cout<<std::endl;
      /*  std::cout<<BWHT("   ");
        std::cout<<BWHT(STKN);
        std::cout<<BWHT("                             ");
        std::cout<<std::endl;
            std::cout<<FBLU("BUY FITNESS: ");
        std::cout<<FBLU(B);
        std::cout<<std::endl;
            std::cout<<FMAG("MX BUY FITNESS: ");
        std::cout<<FMAG(MXBE);
        std::cout<<std::endl<<std::endl;
            std::cout<<FBLU("SELL FITNESS: ");
        std::cout<<FBLU(S);
        std::cout<<std::endl;
            std::cout<<FMAG("MX SELL FITNESS: ");
        std::cout<<FMAG(MXSE);
        std::cout<<std::endl<<std::endl;
            std::cout<<FCYN("CLOSE AT: ");
        std::cout<<FCYN(CAT);
        std::cout<<FCYN(" +- ");
        std::cout<<FCYN(CATSD);
        std::cout<<FCYN("[$]");
        std::cout<<std::endl;
            std::cout<<FGRN("BUY AT: ");
        std::cout<<FGRN(BAT);
        std::cout<<FGRN(" +- ");
        std::cout<<FGRN(BATSD);
        std::cout<<FGRN("[$]");
        std::cout<<std::endl;
            std::cout<<FRED("SELL AT: ");
        std::cout<<FRED(SAT);
        std::cout<<FRED(" +- ");
        std::cout<<FRED(SATSD);
        std::cout<<FRED("[$]");
        std::cout<<std::endl;
        std::cout<<BWHT("                                    ");
        std::cout<<std::endl;
 */   }
    
    void PRFIR(FPM &PRED, FXM &RELD,int oss,long epoch,std::vector<int> outST){//Print FIR
        GPP(PRED,RELD,oss);
        //std::cout<<"FIR_GPP(); DONE"<<std::endl;
        for(int j=0;j<PRED.PTN.size();j++){//iterates for all prediction files    
            LPXD(PRED,RELD,j,oss);
            //std::cout<<"FIR_LPXD(); DONE"<<std::endl;
            EQDSTS(epoch);
           // std::cout<<"FIR_EQDSTS(); DONE"<<std::endl;
            SRDS();
           // std::cout<<"FIR_SRDS(); DONE"<<std::endl;
            DISECTOR(outST);
           // std::cout<<"FIR_DISECTOR(); DONE"<<std::endl;
            CRPRE();
            CPRER();
            ERREST();
            
            CRDRV();
            CRRV();
            LOG();
            
        }
    }
    
   
    
    

};

#endif
