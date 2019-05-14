#ifndef CFM_HPP_INCLUDED
#define CFM_HPP_INCLUDED


#include <cstdio>
#include <clocale>
#include <fstream>
#include <iostream>
#include <string>
#include <experimental/filesystem>

#include <iomanip>
#include <cstring>
#include <sstream>
#include <stdlib.h>
#include <vector>

#include <algorithm>

#include "../LIB/LINUX/DPR.h"
#include "../LIB/MATH/MATH.h"
#include "VANTAGE_CSTM.h"

class FDAA{///Financial external sources database adquisitor and actualizer class
public:
    
    std::string FDBP="FDB";//Financial data base path
    
    vantage av;
    std::string aF = "TIME_SERIES_DAILY";//TIME_SERIES_INTRADAY,TIME_SERIES_DAILY,TIME_SERIES_DAILY_ADJUSTED,TIME_SERIES_WEEKLY,TIME_SERIES_WEEKLY_ADJUSTED,TIME_SERIES_MONTHLY,TIME_SERIES_MONTHLY_ADJUSTED
    std::string aI="15min";//1min,5min,15min,30min,60min only for time series intraday
    std::string aO="full";//compact

    //std::vector<std::string> quandlK={"qj5Rc-_sQuZBjN1SS8KT","Ly9gZRbgr3s9JSzN-B6U","pLa1HDFAFUNeuLFg2_6H","xy3z2nZDSTk4EzeTrzSM"};
    
    std::string IEX_KEY="luA53Jkl7G5";

    std::vector<std::string> vantageK={"BOBTQW93HBTTD0UL"/*,"JWYWRW408RJUYMWA","2D703CR5UA9U6TWG","ISHHA1DQJRQFZX0J","QDFILM22XZPSZ8S4"*/};
    int randomIndex=0;
    
    CSV SD;//Storaged data
    
    DATE DTS;//Time object to parse database dates
    DATE CT;//Current time
    

void DFD(std::string ticket,int dSs=1500){//Download financial data
        av=vantage();
        randomIndex = rand() % vantageK.size();
        av.function = aF;
        av.interv=aI;
        av.ouptsize=aO;
        av.auth(vantageK[randomIndex]); 
    
        av.get(ticket);
        SD.ADATA.clear();
        SD.DATA.clear();
        
        if(av.DATA.size()==0||av.DATA.empty()){//Bad data recived condition
            //DFD(ticket, recursion);//Recursion
            std::cout<<"Financial Server ERRORs"<<std::endl;
            DFD(ticket);
        }
        
        else{//Data recived is correct

        //Parsing and procesing data for training
        SD.CSVP(av.DATA);
        SD.ADATA.erase(SD.ADATA.begin());//Delete the data description
        SD.DATA.erase(SD.DATA.begin());//Delete the data description
        
        DTS.ST(SD.ADATA[0][0]);//Needed to initialize DTS //Posible bad behaviour
        
        for(int dbl=0;dbl<SD.ADATA.size();dbl++){//Date parser
            if(SD.ADATA[dbl].size()>0){
                DTS.ST(SD.ADATA[dbl][0]);
                SD.DATA[dbl][0]=DTS.RTE();
            }
        }
        
        std::vector<std::vector<double>> redV;
        for(int dbl=0;(dbl<dSs)&&(dbl<SD.DATA.size());dbl++){//Reduce the size of the data set if is bigger than dSs, big datasets gives low performance at 3 AGO 2018
            redV.push_back(SD.DATA[dbl]);
        }
        SD.DATA.clear();
        SD.DATA=redV;
            
            /*//dbg
            std::cout<<"PR Data recived from quandl "<<SD.DATA.size()<<"lines"<<std::endl;
            for (int i=0; i<SD.DATA.size();i++){
                for (int q=0;q<SD.DATA[i].size();q++){
                    std::cout<<SD.DATA[i][q]<<"\t";
                }
                std::cout<<std::endl;
            }
            //*/
            /*           std::cout<<"Data recived from quandl "<<SD.ADATA.size()<<"lines"<<std::endl;
            for (int i=0; i<SD.DATA.size();i++){
                for (int q=0;q<SD.ADATA[i].size();q++){
                    std::cout<<SD.ADATA[i][q]<<" ";
                }
                std::cout<<std::endl;
            }
            //dbg*/
        }
            
    }

bool OM(){//Open market checker, true if the market is open
    
}

bool FDUC(std::string ticket, long timelapse=86400){//Financial data up to date checker true is up tu date, false is old data
    SD.LCSV(FDBP+"/"+ticket+".csv");//Loads the current db
    
    if(SD.DATA.size()==0){//Is the data hasn't been sotoraged before
        std::cout<<"ADDING NEW DB"<<std::endl;
        //DFD(ticket);
        return false;
    }
    
    CT.GT();//Current times
    //std::cout<<"Current Time: "<<CT.RTE()-timelapse<<std::endl;
    //std::cout<<"Last DB UDDD: "<<(long)(SD.DATA[0][0]+timelapse)<<std::endl;
    long ctt=(long)ceil((CT.RTE()/timelapse));//Cicles to real time
    long cdt=(long)ceil((SD.DATA[0][0]/timelapse));//Cicles to database time 
    
   // std::cout<<"Current Time: "<<ctt<<std::endl;
   // std::cout<<"Last DB UDDD: "<<cdt<<std::endl;
    
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
  std::cout << "DAY: " << time_out.tm_wday <<" HOUR: "<<time_out.tm_hour<< "\n";
 // std::cout << "(Sunday is 0, Monday is 1, and so on...)\n";
  
  if((time_out.tm_wday==0)||(time_out.tm_wday==1)){//Checks weekend
      std::cout<<"Weekend DB delayed"<<std::endl;
      return true;
}
    if(time_out.tm_hour>14||time_out.tm_hour<23){//Checks open market
        std::cout<<"DB delayed OPEN MARKET US"<<std::endl;
        return true;
    }
  else{
  //Checks date us market
  
  
        
        std::cout<<"DB delayed"<<std::endl;
        //DFD(ticket);
        return false;
  }
    }
}

void FDSR(std::string ticket, bool storage=true){//Financial data server, puts on saved data CSV object the newest data avaliable
    if(FDUC(ticket)){//Data is up to date, SD is already loaded to verify the last date
        
    }
    else{//Data is not up to date
        SD.DATA.clear();
        SD.ADATA.clear();
        
        DFD(ticket);
        
        //SD.DATA=SD.DATA;//Puts on the storaged csv object the recived data csv object
        //SD.ADATA=SD.ADATA;
        if(storage){//Saves the db in case of storage=true;
            SD.SCSV(FDBP+"/"+ticket+".csv");//Saves the current data
            if(!FDUC(ticket)){//Cloud database is not up to date
                std::cout<<"CLOUD DB IS DEAYED"<<std::endl;
            }
        }
    }
}
///Financial database functions

};

class FPDM{//Financial prediction data manager
    
};


class FNC {//Financial normalizer and compositor for financial_nn use in pairs
public:

    CSV TP;//Data structure containing the data
    
    FDA DTMN;//Financial external sources database manager class

    std::vector<std::vector<double>> PrDt;//Processed data
   
    NRM NORMALIZER;//Normalizator object init
    std::vector<NRM> NORMS;//Normalizator vector to predict;
    
    DATE DTS;//Time object to parse database dates
    
    std::string Ticket;//Designación de la base de datos
    
    bool DSP=false;//Contains if data is suitable for predit from

    DNC(){}
        
    void DBG_DT(int i){
        std::cout<<"---------DCN_DBG_DT----------"<<std::endl;

        std::cout<<"---------DCN_DBG_DT----------"<<std::endl;
    }
    
    void Nrm(){       //Normalize the data with maximuns and minimuns

    //Normalizes de from, prediction vector

        PrNORMS.clear();
        std::vector<std::vector<double>> PrTRA=TRS(PrDt);//Traspose the matrix, neded for normalize
        //DBG_STD(PrDt);
        //DBG_STD(PrTRA);
        
        for(int dbl=0;dbl<PrTRA.size();dbl++){//iterates over all data types, columns on the data base
            PrNORMS.push_back(NRM());
            PrNORMS[dbl].ONP(PrTRA[dbl]);
        }
        //Change the maximun date for make room. This must have to be change on 2022
        PrNORMS[0].Nrt=1668470400;
        PrNORMS[0].Rgt=PrNORMS[0].Nrt-PrNORMS[0].Mnt;
        
        for(int dbl=0;dbl<PrTRA.size();dbl++){//iterates over all data types, columns on the data base
            PrTRA[dbl]=PrNORMS[dbl].RNV(PrTRA[dbl]);
        }
        
        PrDt=TRS(PrTRA);//Traspose the normalized vector

    //Normalizes de from, prediction vector


    //Normalize de from, training vector
        
        std::vector<std::vector<double>> TrTRA=TRS(TrDt);
        
        for(int dbl=0;dbl<TrTRA.size();dbl++){//iterates over all data types, columns on the data base
            TrTRA[dbl]=PrNORMS[dbl].RNV(TrTRA[dbl]);
        }
        
        TrDt=TRS(TrTRA);//Traspose the normalized vector
    //Normalize de from, training vector*/


    //Normalize de to, results vector
                
        RsNORMS.clear();
                 
        std::vector<std::vector<double>> RsTRA=TRS(RsDt);//Traspose the matrix, neded for normalize
        
        for(int dbl=0;dbl<RsTRA.size();dbl++){//iterates over all data types, columns on the data base
            RsNORMS.push_back(NRM());
            RsNORMS[dbl].ONP(RsTRA[dbl]);
        }
        
        for(int dbl=0;dbl<RsTRA.size();dbl++){//iterates over all data types, columns on the data base
            RsTRA[dbl]=RsNORMS[dbl].RNV(RsTRA[dbl]);
        }
        
        RsDt=TRS(RsTRA);//Traspose the normalized vector*/
    //Normalize de to, results vector
        
        
        
                 
        /*//DBG
    if(RsDt==RsDtX)std::cout<<"RSDT ECUAL"<<std::endl;
    else std::cout<<"RSDT NOT ECUAL"<<std::endl;
    RsDt=RsDtX;
    //DBG*/

        /*//DBG
 std::cout<<"NrTDt"<<std::endl;
        for(int dbl=0;dbl<PrNORMS.size();dbl++){
            
            //std::cout<<TrDt[dbl][0]<<std::endl;
            //std::cout<<TrDt[dbl][1]<<std::endl;
                std::cout<<PrNORMS[dbl].Nrt<<"\t"<<PrNORMS[dbl].Mnt<<"\t"<<PrNORMS[dbl].Rgt;

            std::cout<<std::endl;
        }//DBG*/
        
        /*//DBG
 std::cout<<"NrRDt"<<std::endl;
        for(int dbl=0;dbl<RsNORMS.size();dbl++){
            
            //std::cout<<TrDt[dbl][0]<<std::endl;
            //std::cout<<TrDt[dbl][1]<<std::endl;
                std::cout<<RsNORMS[dbl].Nrt<<"\t"<<RsNORMS[dbl].Mnt<<"\t"<<RsNORMS[dbl].Rgt;

            std::cout<<std::endl;
        }//DBG*/

    }
    
    
    
    void SeD(std::vector<int> selec, std::vector<std::vector<double>> Dv){//Selects the data wanted for data set

        std::vector<std::vector<double>> PrTRA=TRS(PrDt);//Traspose the matrix, neded for normalize
        
        
        
        
        
        
         //Sets the training vector to the require data size
    std::vector<std::vector<double>> DvT;
    for(int dbli=0;dbli<TrDt.size();dbli++){
        std::vector<double> DtT;
        for(int dbl=0;dbl<inputs.size();dbl++){
           DtT.push_back(TrDt[dbli][inputs[dbl]]);
    }   
    TrDtT.push_back(DtT);
    }
    TrDt.clear();
    TrDt=TrDtT;
    
//Sets the training vector to the require data size
    
    //Sets the results to the desired values
    std::vector<std::vector<double>> RsDtT;
    for(int dbli=0;dbli<RsDt.size();dbli++){
        std::vector<double> DtT;
        for(int dbl=0;dbl<outs.size();dbl++){
           DtT.push_back(RsDt[dbli][outs[dbl]]);
    }   
    RsDtT.push_back(DtT);
    }
    RsDt.clear();
    RsDt=RsDtT;
    //Sets the results to the desired values
    
    //Sets the prediction to the desired size
    std::vector<std::vector<double>> PrDtT;
    for(int dbli=0;dbli<PrDt.size();dbli++){
        std::vector<double> DtT;
        for(int dbl=0;dbl<inputs.size();dbl++){
           DtT.push_back(PrDt[dbli][inputs[dbl]]);
    }   
    PrDtT.push_back(DtT);
    }
    PrDt.clear();
    PrDt=PrDtT;
    
    //Sets the prediction to the desired size
    
    //Reverse the vectors to put the older value first    
    std::reverse(TrDt.begin(),TrDt.end());
    TrDt.pop_back();//Elimina el ultimo dia puesto que no se utiliza para el entrenamiento
    //std::reverse(RsDt.begin(),RsDt.end());
    std::reverse(PrDt.begin(),PrDt.end());
          //Reverse the vectors to put the older value first  

    //DBG_DT(RsDt.size()-1);
    }
    
    bool CompD(){//Ensures and compatilize the data in function of the date, dSs is the size of the data set,0 is for the máximun posible
        //Data compatibility 1 correct
        // std::cout<<"Date compatibility modifier;"<<std::endl;
            //     std::cout<<"DBG 1"<<std::endl;
        std::vector<double> PFdL;//Vector con espacio en posix time entre fechas
        //std::cout<<"DBG 2: "<<VR.DATA.size()<<std::endl;
        for(int i=1;i<VR.DATA.size();i++){//Recorre la base de datos a predecir desde
          //  std::cout<<"DBG 3: "<<i<<std::endl;
            PFdL.push_back(VR.DATA[i-1][0]-VR.DATA[i][0]);//Mete en el vector las diferencias entre todas las fechas
        }
        double PFMD=MEAN(PFdL);//Media de la distancia entre fechas
        double PFSD=ST_DEV(PFdL);//Desviación típica de las fechas de la bases de datos para predecir desde
        int PFS=VR.DATA.size();//Tamaño del set de datos para predecir desde
        long PFOD=VR.DATA[PFS-1][0];//Fecha mas antigua de los datos a predecir desde
        long PFND=VR.DATA[0][0];//Fecha mas nueva de los datos a predecir desde
        std::vector<std::vector<double>> tPFd;//Vectores de datos temporales para las transformaciones necesarias

        
        std::vector<double> TPdL;//Vector con estacio en posix time entre fechas
        for(int i=1;i<PR.DATA.size();i++){//Recorre la base de datos a predecir
            TPdL.push_back(PR.DATA[i-1][0]-PR.DATA[i][0]);//Mete en el vector las diferencias entre todas las fechas
        }
        double TPMD=MEAN(TPdL);
        double TPSD=ST_DEV(TPdL);//Desviación típica de las fechas de la bases de datos a predecir
        int TPS=PR.DATA.size();//Tamaño del set de datos a predecir
        long TPOD=PR.DATA[TPS-1][0];//Fecha mas antigua de los datos a predecir
        long TPND=PR.DATA[0][0];//Fecha mas nueva de los datos a predecir
        std::vector<std::vector<double>> tTPd;//Vectores de datos temporales para las transformaciones necesarias      
        
        /*std::cout<<"Date compatibility checker;"<<std::endl;
        std::cout<<"Date compatibility data:"<<std::endl;
        std::cout<<"PFMD: "<<PFMD<<std::endl;
        std::cout<<"PFSD: "<<PFSD<<std::endl;
        std::cout<<"PFS: "<<PFS<<std::endl;
        std::cout<<"PFOD: "<<PFOD<<std::endl;
        std::cout<<"PFND: "<<PFND<<std::endl;
        std::cout<<std::endl;
        std::cout<<"TPMD: "<<TPMD<<std::endl;
        std::cout<<"TPSD: "<<TPSD<<std::endl;
        std::cout<<"TPS: "<<TPS<<std::endl;
        std::cout<<"TPOD: "<<TPOD<<std::endl;
        std::cout<<"TPND: "<<TPND<<std::endl;
        //DBG*/ 
       if(PR.DATA.size()<=1||VR.DATA.size()<=1){//quandl server has a error
           //std::cout<<"PRID: "<<PRid<<"\tVRID: "<<VRid<<std::endl<<std::endl;
           std::cout<<"ALPHA V ERROR EN DNC obj"<<std::endl;
            DSP=false;
            return false;
        }
        
        if(TPND<=(PFND+(PFMD+PFSD*5))){//Si los datos a predecir desde estan actualizados hasta le fecha más nueva disponible de los datos a predecir, es viable la transformación para generar las predicciones
            std::cout<<"Date compatibility passed;"<<std::endl;
            
            //Busca para cada dato a predecir entre que dos o a cual dato desde el que predecir se encuentra.
            for(int i=0;i<PR.DATA.size();i++){//Bucle principal, 
                int OLDD=-1;
                int NEWD=-1;
                bool ecuali=false;

                for(int r=0;r<VR.DATA.size();r++){//Bucle por los datos a predecir desde por cada dato a predecir
                    
                    if((int)PR.DATA[i][0]==(int)VR.DATA[r][0]){//Las fechas son iguales
                        
                        //std::cout<<"SAME DATE"<<std::endl;
                        tPFd.push_back(VR.DATA[r]);
                        ecuali=true;
                        
                        break;
                    }
                    
                    else if(PR.DATA[i][0]<VR.DATA[r][0]){//Si la fecha de los datos a predecir desde es más nueva que los datos a predecir desde almacena la dirección en NEWD        
                    
                        NEWD=r;
                        //std::cout<<"NEWD:: "<<NEWD<<std::endl;                         
                    }
                    
                    else if(PR.DATA[i][0]>VR.DATA[r][0]){//Si la fecha de los datos a predecir desde es mas antigua guarda la direcciónen OLDD y termina el bucle porque recorre las fechas de nuevas a antiguas        
                    
                        OLDD=r;
                        //std::cout<<"OLDD:: "<<OLDD<<std::endl; 
                        break;
                    }

                }
               // std::cout<<"NEWD:: "<<NEWD<<std::endl; 
               // std::cout<<"OLDD:: "<<OLDD<<std::endl;
                    
                    std::vector<double> tVEC;//Vector para modificación de fechas e interpolación.
                    
                if(ecuali){//Los datos son iguales introduce los datos al vector
                   //std::cout<<"SAME DATE"<<std::endl; 

                }
                else if(NEWD==-1){//Si no existe un dato más nuevo en los datos a predecir desde guarda en el vector el mas nuevo disponible y le pone la fecha de los datos a predecir actual
                    //std::cout<<"NEWD"<<std::endl; 
                    tVEC.clear();
                    tVEC=VR.DATA[0];
                    tVEC[0]=PR.DATA[i][0];
                    tPFd.push_back(tVEC);
                }   
                else if(OLDD==-1){//Si no existe un dato mas antiguo en los datos a predecir desde elimina los datos a predecir mas antiguos, duplica el ultimo valor de los datos a predecir desde y termina la ejecución del bucle 
                    //std::cout<<"OLDD"<<std::endl; 
                    PR.DATA.erase(PR.DATA.begin()+i,PR.DATA.end());                    
                    break;
                }
                else{//La fecha de los datos a predecir se encuentra entre dos de los datos a predecir desde, interpola.
                    //std::cout<<"INTERP"<<std::endl; 
                    tVEC.clear();
                    tVEC.push_back(PR.DATA[i][0]);//Puts the newer date
                    for(int c=1;c<VR.DATA[i].size();c++){
                        tVEC.push_back(ILB(PR.DATA[i][0],VR.DATA[OLDD][0],VR.DATA[NEWD][0],VR.DATA[OLDD][c],VR.DATA[NEWD][c]));
                        //std::cout<<"Interp_VAL:"<<ILB(PR.DATA[i][0],VR.DATA[OLDD][0],VR.DATA[NEWD][0],VR.DATA[OLDD][c],VR.DATA[NEWD][c])<<std::endl;
                    }
                    
                    tPFd.push_back(tVEC);
                }
                
            }
            
            VR.DATA.clear();
            VR.DATA=tPFd;
            tPFd.clear();
            
            /*//dbg
            std::cout<<"PR "<<PR.DATA.size()<<"lines"<<std::endl;
            for (int i=0; i<PR.DATA.size();i++){
                for (int q=0;q<PR.DATA[i].size();q++){
                    std::cout<<PR.DATA[i][q]<<"\t";
                }
                std::cout<<std::endl;
            }
            
            //dbg
            std::cout<<"VR "<<VR.DATA.size()<<"lines"<<std::endl;
            for (int i=0; i<VR.DATA.size();i++){
                for (int q=0;q<VR.DATA[i].size();q++){
                    std::cout<<VR.DATA[i][q]<<"\t";
                }
                std::cout<<std::endl;
            }
           //*/
           
           //En este punto los datos a predecir y a predecir desde tienen el mismo tamaño, y las mismas fechas.
           
            //Create the data to predict from vector
            for(int r=0;r<VR.DATA.size();r++){
                TrDt.push_back(VR.DATA[r]);
            }
            //TrDt.pop_back();//Elimina el ultimo dia puesto que no se utiliza
            //Create the data to predict from vector
            
            //Create the results vector
            
            std::vector<std::vector<double>> SrD;
            std::reverse(PR.DATA.begin(),PR.DATA.end());
            for(int r=1;r<PR.DATA.size();r++){
                std::vector<double> Sr;
                for (int e=0;e<PR.DATA[r].size();e++){
                Sr.push_back(PR.DATA[r][e]-PR.DATA[r-1][e]);
                }
                SrD.push_back(Sr);
            }
            
            RsDt.clear();
            RsDt=SrD;
            std::reverse(PR.DATA.begin(),PR.DATA.end());
            
            //Elimina el primer dia puesto que no se utiliza dejando alineados lo que paso el dia anterior para meter como datos y lo que pasó el dia siguiente para comparar resultados
            //Create the results vector
                            
        //Set the first element on the vector data to make the prediction vector.
            
            for(int i=0;i<VR.DATA.size();i++){
                PrDt.push_back(VR.DATA[i]);
            }                  
            //Complete the vector data to make the prediction vector
                      
                      
            if(PR.DATA.size()==VR.DATA.size()){
                std::cout<<"Data sets are compatilized"<<std::endl;
                DSP=true;
                return true;
            }
            else{
                std::cout<<"BUG"<<std::endl;
                DSP=false;
                return false;
            }
            DBG_DT(0);
            
                    }
        else{//Las bases de datos no son compatibles para la predicción.
            std::cout<<"Data sets are not compatibles"<<std::endl;
            DSP=false;
            return false;
            
        }

        }
    
    void GPRd(){//Gets que data to predict

       
       // DTMN=DBM();//Financial data base manager
        
        PR.ADATA.clear();
        PR.DATA.clear();
        
        DTMN.SD.ADATA.clear();
        DTMN.SD.DATA.clear();
        
        DTMN.FDSR(PRid);
        PR.ADATA=DTMN.SD.ADATA;
        PR.DATA=DTMN.SD.DATA;
        
        qPR=false;
        
                   /* //dbg
            std::cout<<"PR Data recived from quandl "<<PR.DATA.size()<<"lines"<<std::endl;
            for (int i=0; i<PR.DATA.size();i++){
                for (int q=0;q<PR.DATA[i].size();q++){
                    std::cout<<PR.DATA[i][q]<<"\t";
                }
                std::cout<<std::endl;
            }
            //*/
            /*           std::cout<<"Data recived from quandl "<<PR.ADATA.size()<<"lines"<<std::endl;
            for (int i=0; i<PR.DATA.size();i++){
                for (int q=0;q<PR.ADATA[i].size();q++){
                    std::cout<<PR.ADATA[i][q]<<" ";
                }
                std::cout<<std::endl;
            }
            //dbg*/
        
    }
    
    void GVRd(){//Gets the data to predict from.
       
        //DTMN=DBM();//Financial data base manager
        
        VR.ADATA.clear();
        VR.DATA.clear();
        
        DTMN.SD.ADATA.clear();
        DTMN.SD.DATA.clear();
        
        DTMN.FDSR(VRid);
        VR.ADATA=DTMN.SD.ADATA;
        VR.DATA=DTMN.SD.DATA;
        
        qVR=false;
        
                   /* //dbg
           std::cout<<"VR Data recived from quandl "<<VR.DATA.size()<<"lines"<<std::endl;
            for (int i=0; i<VR.DATA.size();i++){
                for (int q=0;q<VR.DATA[i].size();q++){
                    std::cout<<VR.DATA[i][q]<<"\t";
                }
                std::cout<<std::endl;
            }
            ///*
                       std::cout<<"Data recived from quandl "<<VR.ADATA.size()<<"lines"<<std::endl;
            for (int i=0; i<VR.DATA.size();i++){
                for (int q=0;q<VR.ADATA[i].size();q++){
                    std::cout<<VR.ADATA[i][q]<<" ";
                }
                std::cout<<std::endl;
            }
            //dbg*/
        
    }
    

    
    void DPr(std::vector<int> outs, std::vector<int> inputs){//Process the database for nn porposes
       
//On this point the VR and PR vectors contains the data bases with all avaliable history of each stock values avaliable, with the dates converted to posix time.
       
       std::cout<<"PRID: "<<PRid<<"\tVRID: "<<VRid<<std::endl;
        PrDt.clear();    
        RsDt.clear();
        TrDt.clear();
        

                //Obtaining data from quandl
            GPRd();
           //FEr=DTS.errr;
            FEr=false;
        //   std::cout<<"OPR"<<std::endl;
            GVRd();
            //TEr=DTS.errr;
            TEr=false;
        //    std::cout<<"OVR"<<std::endl;
        /*   while(FcC || TcC ){//Ensures data integrity
                GPRd();
                FcC=DTS.errr;
                GVRd();
                TcC=DTS.errr;    
            }*/
                //Obtaining data from quandl
        
        


//Processing data for neural network
if(CompD()&&!FEr&&!TEr&&!qPR&&!qVR){
//Secciona los datos
SecD(outs,inputs);
//Secciona los datos
//Normalize the data with maximuns except time variables
Nrm();
//Normalize the data with maximus except time variables
//std::cout<<"QCR"<<std::endl;
}
        /*else if(FEr||TEr){//Quandl or sintaxis error
            DPr(outs,inputs);
        }*/
else{//Compositor error, data no compatible
    std::cout<<"QERR"<<std::endl;
    //DPr(outs,inputs);
}//Recursive error corrector
            
        }
        
///LEGACY CODE
    void DPr(std::vector<std::vector<double>>pr,std::vector<int> outs, std::vector<std::vector<double>>vr,std::vector<int> inputs){//Process the custom database for nn porposes
       
//On this point the VR and PR vectors contains the data bases with all avaliable history of each stock values avaliable, with the dates converted to posix time.
       
       
        PrDt.clear();    
        RsDt.clear();
        TrDt.clear();
       
        PR.DATA.clear();
        VR.DATA.clear();
        PR.DATA=pr;
        VR.DATA=vr;

            /*//dbg
            std::cout<<"PR "<<PR.DATA.size()<<"lines"<<std::endl;
            for (int i=0; i<PR.DATA.size();i++){
                for (int q=0;q<PR.DATA[i].size();q++){
                    std::cout<<PR.DATA[i][q]<<"\t";
                }
                std::cout<<std::endl;
            }
            
            //dbg
            std::cout<<"VR "<<VR.DATA.size()<<"lines"<<std::endl;
            for (int i=0; i<VR.DATA.size();i++){
                for (int q=0;q<VR.DATA[i].size();q++){
                    std::cout<<VR.DATA[i][q]<<"\t";
                }
                std::cout<<std::endl;
            }
           //*/
//Processing data for neural network
        //std::cout<<"DBG 1"<<std::endl;
if(CompD()){
        // std::cout<<"DBG 2"<<std::endl;
   
//Secciona los datos
SecD(outs,inputs);
//Secciona los datos

        //std::cout<<"DBG 3"<<std::endl;

//Normalize the data with maximuns except time variables
Nrm();
//Normalize the data with maximus except time variables
       // std::cout<<"DBG 4"<<std::endl;


        }

            
        }

///LEGACY CODE
        
	
};

class NDM {//Neural data manager
public:
    //All vector are in alphabetical order
std::string NCF="C_DS";//Neural configuration folder
std::string NCT="FNND_";//Neural configuration token

std::vector<std::string> NC;//Neural configs names

std::vector<std::vector<std::string>> CSNC;//Simple neural configs names
std::vector<std::vector<std::string>> BSNC;//Simple neural configs names
std::vector<std::vector<std::string>> VSNC;//Simple neural configs names

std::vector<std::string> PCSNC;//Simple neural configs path names
std::vector<std::string> PBSNC;//Simple neural configs path names
std::vector<std::string> PVSNC;//Simple neural configs path names

std::vector<std::string> CCRNC;//Cross referenced neural configs names
std::vector<std::string> BCRNC;//Cross referenced neural configs names
std::vector<std::string> VCRNC;//Cross referenced neural configs names

std::vector<std::string> PCCRNC;//Cross referenced neural configs path names
std::vector<std::string> PBCRNC;//Cross referenced neural configs path names
std::vector<std::string> PVCRNC;//Cross referenced neural configs path names
/*DBM (){
}
        
~DBM(){

}*/
///Common functions
int SDN(std::string filn){//Checks for number of stock simbols on the file name
    int i=0;
    int nt=0;
    std::string FLIN=filn;
    while(FLIN.size()>0){
        if(FLIN[i]=='_'){
           nt++; 
        }
        i++;
        FLIN.erase(0, 1);
        
    }
    return nt;
}

std::vector<int> TKP(std::string filn){//Devuelve un vector con las posiciones de todos los tokens "_ y ."
    
    std::vector<int> p;
    int i=1;

    std::string FLIN=filn;
    
    while((FLIN.find("_",i)!=std::string::npos)){
    std::size_t found = FLIN.find("_",i);
    p.push_back(found); 
    i=found+1;
    }
     std::size_t found = FLIN.find(".");
    p.push_back(found); 
    return p;
}

 void GFN(){//Get all file names on the forlders                              
    NC.clear();
                              
    for (auto & p : std::experimental::filesystem::directory_iterator(NCF)){//itera por todos los archivos en la carpeta de las NC
            std::string CFILE= p.path();
            NC.push_back(CFILE);
                              }
    sort(NC.begin(),NC.end());//Ordena alfabeticamente los nombres de los archivos
}
///Comon functions

///Neural database functions
void FNP(){//File name parser
    PVSNC.clear();
    VSNC.clear();
    PVCRNC.clear();
    VCRNC.clear();
    
    PBSNC.clear();
    BSNC.clear();
    PBCRNC.clear();
    BCRNC.clear();
    
    PCSNC.clear();
    CSNC.clear();
    PCCRNC.clear();
    CCRNC.clear();
    
    
    for(int i=0;i<NC.size();i++){//Itera por el nombre de todos los archivos
        
        if(NC[i].find(NCT)!=std::string::npos){//Encuentra el token buscado para eliminar otros posibles archivos
           
            if(!NC[i].compare(NCF.size()+1,1,"V")){//Si es del tipo V
                //std::size_t frs = NC[i].find(NCF);            
                //NC[i]= NC[i].substr(frs+NCF.size()+1+6);//Elimina la dirección y el token del string 
                
                //NC[i].erase(NC[i].size() - (1+3),3);//Deletes file extensión
                
                std::vector<int>pp=TKP(NC[i]);//Vector con las posiciones de todos los tokens
                
                if(pp.size()>=4){//Si existen dos o mas identificadores de STOK
                    PVSNC.push_back(NC[i]);
                    std::vector<std::string> tmpsv;
                    for(int e=1;e<pp.size()-1;e++){//Itera por los tokens excepto el (primero,segundo) y el ultimo
                        tmpsv.push_back(NC[i].substr(pp[e]+1,pp[e+1]-(pp[e]+1)));//Cuts and puts on the temp vector the STOK names.   
                    }
                    VSNC.push_back(tmpsv);
                    tmpsv.clear();
                    
                }
                else if(pp.size()==3){//Si existe un unico identificador de STOK
                    PVCRNC.push_back(NC[i]);
                        VCRNC.push_back(NC[i].substr(pp[1]+1,pp[2]-(pp[1]+1)));//Cuts and puts on the temp vector the STOK names.   
                }
                
            }
            else if(!NC[i].compare(NCF.size()+1,1,"B")){//Si es del tipo B
                //std::size_t frs = NC[i].find(NCF);            
                //NC[i]= NC[i].substr(frs+NCF.size()+1+6);//Elimina la dirección y el token del string 
                
                //NC[i].erase(NC[i].size() - (1+3),3);//Deletes file extensión
                
                std::vector<int>pp=TKP(NC[i]);//Vector con las posiciones de todos los tokens
                
                if(pp.size()>=4){//Si existen dos o mas identificadores de STOK
                    PBSNC.push_back(NC[i]);
                    std::vector<std::string> tmpsv;
                    for(int e=1;e<pp.size()-1;e++){//Itera por los tokens excepto el (primero,segundo) y el ultimo
                        tmpsv.push_back(NC[i].substr(pp[e]+1,pp[e+1]-(pp[e]+1)));//Cuts and puts on the temp vector the STOK names.   
                    }
                    BSNC.push_back(tmpsv);
                    tmpsv.clear();
                    
                }
                else if(pp.size()==3){//Si existe un unico identificador de STOK
                    PBCRNC.push_back(NC[i]);
                        BCRNC.push_back(NC[i].substr(pp[1]+1,pp[2]-(pp[1]+1)));//Cuts and puts on the temp vector the STOK names.   
                }
                
            }
            else if(!NC[i].compare(NCF.size()+1,1,"C")){//Si es del tipo C
                //std::size_t frs = NC[i].find(NCF);            
                //NC[i]= NC[i].substr(frs+NCF.size()+1+6);//Elimina la dirección y el token del string 
                
                //NC[i].erase(NC[i].size() - (1+3),3);//Deletes file extensión
                
                std::vector<int>pp=TKP(NC[i]);//Vector con las posiciones de todos los tokens
                
                if(pp.size()>=4){//Si existen dos o mas identificadores de STOK
                    PCSNC.push_back(NC[i]);
                    std::vector<std::string> tmpsv;
                    for(int e=1;e<pp.size()-1;e++){//Itera por los tokens excepto el (primero,segundo) y el ultimo
                        tmpsv.push_back(NC[i].substr(pp[e]+1,pp[e+1]-(pp[e]+1)));//Cuts and puts on the temp vector the STOK names.   
                    }
                    CSNC.push_back(tmpsv);
                    tmpsv.clear();
                    
                }
                else if(pp.size()==3){//Si existe un unico identificador de STOK
                    PCCRNC.push_back(NC[i]);
                        CCRNC.push_back(NC[i].substr(pp[1]+1,pp[2]-(pp[1]+1)));//Cuts and puts on the temp vector the STOK names.   
                }
                
            }
        }
    }
}

void DLF(std::string path){//Deletes the file given the path
        std::uintmax_t n = std::experimental::filesystem::remove(path);
    std::cout << "Deleted " << n << " files or directories\n";
}
///Neural database functions

};

class PDM {//Predictions data manager
public:
    //All vector are in alphabetical order
std::string PDBF="C_PRD";//Prediction data base folder
std::string PDBT="PRD_";//Prediction data base token
std::vector<std::string> P;//Prediction db names

std::vector<std::vector<std::string>> SP;//Simple prediction db names
std::vector<std::string> PSP;//Simple prediction db names paths

std::vector<std::string> CRP;//Cross referenced prediction db names
std::vector<std::string> PCRP;//Cross referenced prediction db names paths

std::string STPF="C_ST";//Statistical predictión path

std::string FDBP="FDB";//Financial data base path
std::vector<std::string> FADT;//Financial avaliable data tokens
std::vector<std::string> PFADT;//Path to financial avaliable data tokens


/*DBM (){
}
        
~DBM(){

}*/
///Common functions
int SDN(std::string filn){//Checks for number of stock simbols on the file name
    int i=0;
    int nt=0;
    std::string FLIN=filn;
    while(FLIN.size()>0){
        if(FLIN[i]=='_'){
           nt++; 
        }
        i++;
        FLIN.erase(0, 1);
        
    }
    return nt;
}

std::vector<int> TKP(std::string filn){//Devuelve un vector con las posiciones de todos los tokens "_ y ."
    
    std::vector<int> p;
    int i=1;

    std::string FLIN=filn;
    
    while((FLIN.find("_",i)!=std::string::npos)){
    std::size_t found = FLIN.find("_",i);
    p.push_back(found); 
    i=found+1;
    }
     std::size_t found = FLIN.find(".");
    p.push_back(found); 
    return p;
}

 void GFN(){//Get all file names on the forlders
     //std::cout<<"DBG2"<<std::endl; 
     P.clear();
     //std::cout<<"DBG3"<<std::endl; 
     
     for (auto & p : std::experimental::filesystem::directory_iterator(PDBF)){//itera por todos los archivos en la carpeta de las DB
            std::string CFILE= p.path();
            
            P.push_back(CFILE);
                              }
    sort(P.begin(),P.end());//Ordena alfabeticamente los nombres de los archivos
                              
}
///Comon functions

///Neural database functions

void FNP(){//File name parser
    PSP.clear();
    SP.clear();
    
    PCRP.clear();
    CRP.clear();
    
    for(int i=0;i<P.size();i++){//Itera por el nombre de todos los archivos
        
        if(P[i].find(PDBT)!=std::string::npos){//Encuentra el token buscado para eliminar otros posibles archivos
           
                //std::size_t frs = NC[i].find(NCF);            
                //NC[i]= NC[i].substr(frs+NCF.size()+1+6);//Elimina la dirección y el token del string 
                
                //NC[i].erase(NC[i].size() - (1+3),3);//Deletes file extensión
                
                std::vector<int>pp=TKP(P[i]);//Vector con las posiciones de todos los tokens
                
                if(pp.size()>=4){//Si existen dos o mas identificadores de STOK
                    PSP.push_back(P[i]);
                    std::vector<std::string> tmpsv;
                    for(int e=1;e<pp.size()-1;e++){//Itera por los tokens excepto el (primero,segundo) y el ultimo
                        tmpsv.push_back(P[i].substr(pp[e]+1,pp[e+1]-(pp[e]+1)));//Cuts and puts on the temp vector the STOK names.   
                    }
                    SP.push_back(tmpsv);
                    tmpsv.clear();
                    
                }
                else if((pp.size()==3)&&P[i].size()){//Si existe un unico identificador de STOK
                    PCRP.push_back(P[i]);
                        CRP.push_back(P[i].substr(pp[1]+1,pp[2]-(pp[1]+1)));//Cuts and puts on the temp vector the STOK names.   
                }
                
            }

        
    }
}
       
void DLF(std::string path){//Deletes the file given the path
        std::uintmax_t n = std::experimental::filesystem::remove(path);
    std::cout << "Deleted " << n << " files or directories\n";
}

///Database functions

};

class FDM{//Financial data manager
    
};







//LEGACY BAD CODE

class DNC {//Data normalizer and compositor for financial_nn use in pairs
public:

    CSV PR;//Data structure containing data to predict
    CSV VR;//Data structure containing data to predict from
    
    FDA DTMN;//Financial external sources database manager class
    
    std::vector<std::vector<double>> TrDt;//Training data vector "Data to predict from for training"
    std::vector<std::vector<double>> RsDt;//Results data vector "Data to predict for training"
    std::vector<std::vector<double>> PrDt;//Data for pedict from "Data to predic from for prediction"
   
    NRM NORMALIZER;//Normalizator object init
    std::vector<NRM> PrNORMS;//Normalizator vector to predict;
    std::vector<NRM> RsNORMS;//Normalizator vector to predict from;
    
    DATE DTS;//Time object to parse database dates
    
    std::string PRid=/*"WIKI/*/"GOOG";//Designación de la base de datos a predecir
    std::string VRid=/*"WIKI/*/"GOOG";//Designación de la base de datos apartir de la que se intentará predecir.    
    
    bool DSP=false;//Contains if data is suitable for predit from
        
    bool FEr=false;//Bool for quandl server error
    bool TEr=false;//Bool for quandl server error
    
    bool qPR=false;//Bool for quandl server error
    bool qVR=false;//Bool for quandl server error
    
    std::string STOK;

    DNC(std::string VRID,std::string PRID){
        PRid=PRID;
        VRid=VRID;
        
    }
    DNC(){}
    
    void PINIT(std::string VRID,std::string PRID){
        PRid=PRID;
        VRid=VRID;
    }
    
    void DBG_DT(int i){
        std::cout<<"---------DCN_DBG_DT----------"<<std::endl;
        
        std::cout<<"TrDt:";
         //for(int c=0;c<TrDt.size();c++){
            for(int r=0;r<TrDt[i].size();r++){
                std::cout<<TrDt[i][r]<<" ";
            }
            std::cout<<std::endl;            
        //}
        
        std::cout<<"PrDt:";
         //for(int c=0;c<PrDt.size();c++){
            for(int r=0;r<PrDt[i].size();r++){
                std::cout<<PrDt[i][r]<<" ";
            }
            std::cout<<std::endl;            
       // }
        
        std::cout<<"RsDt:";
         //for(int c=0;c<RrDt.size();c++){
            for(int r=0;r<RsDt[i].size();r++){
                std::cout<<RsDt[i][r]<<" ";
            }
            std::cout<<std::endl;            
        //}
        
        std::cout<<"---------DCN_DBG_DT----------"<<std::endl;
    }
    
    void Nrm(){       //Normalize the data with maximuns and minimuns

    //Normalizes de from, prediction vector

        PrNORMS.clear();
        std::vector<std::vector<double>> PrTRA=TRS(PrDt);//Traspose the matrix, neded for normalize
        //DBG_STD(PrDt);
        //DBG_STD(PrTRA);
        
        for(int dbl=0;dbl<PrTRA.size();dbl++){//iterates over all data types, columns on the data base
            PrNORMS.push_back(NRM());
            PrNORMS[dbl].ONP(PrTRA[dbl]);
        }
        //Change the maximun date for make room. This must have to be change on 2022
        PrNORMS[0].Nrt=1668470400;
        PrNORMS[0].Rgt=PrNORMS[0].Nrt-PrNORMS[0].Mnt;
        
        for(int dbl=0;dbl<PrTRA.size();dbl++){//iterates over all data types, columns on the data base
            PrTRA[dbl]=PrNORMS[dbl].RNV(PrTRA[dbl]);
        }
        
        PrDt=TRS(PrTRA);//Traspose the normalized vector

    //Normalizes de from, prediction vector


    //Normalize de from, training vector
        
        std::vector<std::vector<double>> TrTRA=TRS(TrDt);
        
        for(int dbl=0;dbl<TrTRA.size();dbl++){//iterates over all data types, columns on the data base
            TrTRA[dbl]=PrNORMS[dbl].RNV(TrTRA[dbl]);
        }
        
        TrDt=TRS(TrTRA);//Traspose the normalized vector
    //Normalize de from, training vector*/


    //Normalize de to, results vector
                
        RsNORMS.clear();
                 
        std::vector<std::vector<double>> RsTRA=TRS(RsDt);//Traspose the matrix, neded for normalize
        
        for(int dbl=0;dbl<RsTRA.size();dbl++){//iterates over all data types, columns on the data base
            RsNORMS.push_back(NRM());
            RsNORMS[dbl].ONP(RsTRA[dbl]);
        }
        
        for(int dbl=0;dbl<RsTRA.size();dbl++){//iterates over all data types, columns on the data base
            RsTRA[dbl]=RsNORMS[dbl].RNV(RsTRA[dbl]);
        }
        
        RsDt=TRS(RsTRA);//Traspose the normalized vector*/
    //Normalize de to, results vector
        
        
        
                 
        /*//DBG
    if(RsDt==RsDtX)std::cout<<"RSDT ECUAL"<<std::endl;
    else std::cout<<"RSDT NOT ECUAL"<<std::endl;
    RsDt=RsDtX;
    //DBG*/

        /*//DBG
 std::cout<<"NrTDt"<<std::endl;
        for(int dbl=0;dbl<PrNORMS.size();dbl++){
            
            //std::cout<<TrDt[dbl][0]<<std::endl;
            //std::cout<<TrDt[dbl][1]<<std::endl;
                std::cout<<PrNORMS[dbl].Nrt<<"\t"<<PrNORMS[dbl].Mnt<<"\t"<<PrNORMS[dbl].Rgt;

            std::cout<<std::endl;
        }//DBG*/
        
        /*//DBG
 std::cout<<"NrRDt"<<std::endl;
        for(int dbl=0;dbl<RsNORMS.size();dbl++){
            
            //std::cout<<TrDt[dbl][0]<<std::endl;
            //std::cout<<TrDt[dbl][1]<<std::endl;
                std::cout<<RsNORMS[dbl].Nrt<<"\t"<<RsNORMS[dbl].Mnt<<"\t"<<RsNORMS[dbl].Rgt;

            std::cout<<std::endl;
        }//DBG*/

    }
    
    
    
    void SecD(std::vector<int> outs, std::vector<int> inputs){//Secciona la base de datos, cogiendo solo los datos relevantes itroducidos como variables de función.
        
        
        
        
        
        
        
        
        
        
        
        
        
        
         //Sets the training vector to the require data size
    std::vector<std::vector<double>> TrDtT;
    for(int dbli=0;dbli<TrDt.size();dbli++){
        std::vector<double> DtT;
        for(int dbl=0;dbl<inputs.size();dbl++){
           DtT.push_back(TrDt[dbli][inputs[dbl]]);
    }   
    TrDtT.push_back(DtT);
    }
    TrDt.clear();
    TrDt=TrDtT;
    
//Sets the training vector to the require data size
    
    //Sets the results to the desired values
    std::vector<std::vector<double>> RsDtT;
    for(int dbli=0;dbli<RsDt.size();dbli++){
        std::vector<double> DtT;
        for(int dbl=0;dbl<outs.size();dbl++){
           DtT.push_back(RsDt[dbli][outs[dbl]]);
    }   
    RsDtT.push_back(DtT);
    }
    RsDt.clear();
    RsDt=RsDtT;
    //Sets the results to the desired values
    
    //Sets the prediction to the desired size
    std::vector<std::vector<double>> PrDtT;
    for(int dbli=0;dbli<PrDt.size();dbli++){
        std::vector<double> DtT;
        for(int dbl=0;dbl<inputs.size();dbl++){
           DtT.push_back(PrDt[dbli][inputs[dbl]]);
    }   
    PrDtT.push_back(DtT);
    }
    PrDt.clear();
    PrDt=PrDtT;
    
    //Sets the prediction to the desired size
    
    //Reverse the vectors to put the older value first    
    std::reverse(TrDt.begin(),TrDt.end());
    TrDt.pop_back();//Elimina el ultimo dia puesto que no se utiliza para el entrenamiento
    //std::reverse(RsDt.begin(),RsDt.end());
    std::reverse(PrDt.begin(),PrDt.end());
          //Reverse the vectors to put the older value first  

    //DBG_DT(RsDt.size()-1);
    }
    
    bool CompD(){//Ensures and compatilize the data in function of the date, dSs is the size of the data set,0 is for the máximun posible
        //Data compatibility 1 correct
        // std::cout<<"Date compatibility modifier;"<<std::endl;
            //     std::cout<<"DBG 1"<<std::endl;
        std::vector<double> PFdL;//Vector con espacio en posix time entre fechas
        //std::cout<<"DBG 2: "<<VR.DATA.size()<<std::endl;
        for(int i=1;i<VR.DATA.size();i++){//Recorre la base de datos a predecir desde
          //  std::cout<<"DBG 3: "<<i<<std::endl;
            PFdL.push_back(VR.DATA[i-1][0]-VR.DATA[i][0]);//Mete en el vector las diferencias entre todas las fechas
        }
        double PFMD=MEAN(PFdL);//Media de la distancia entre fechas
        double PFSD=ST_DEV(PFdL);//Desviación típica de las fechas de la bases de datos para predecir desde
        int PFS=VR.DATA.size();//Tamaño del set de datos para predecir desde
        long PFOD=VR.DATA[PFS-1][0];//Fecha mas antigua de los datos a predecir desde
        long PFND=VR.DATA[0][0];//Fecha mas nueva de los datos a predecir desde
        std::vector<std::vector<double>> tPFd;//Vectores de datos temporales para las transformaciones necesarias

        
        std::vector<double> TPdL;//Vector con estacio en posix time entre fechas
        for(int i=1;i<PR.DATA.size();i++){//Recorre la base de datos a predecir
            TPdL.push_back(PR.DATA[i-1][0]-PR.DATA[i][0]);//Mete en el vector las diferencias entre todas las fechas
        }
        double TPMD=MEAN(TPdL);
        double TPSD=ST_DEV(TPdL);//Desviación típica de las fechas de la bases de datos a predecir
        int TPS=PR.DATA.size();//Tamaño del set de datos a predecir
        long TPOD=PR.DATA[TPS-1][0];//Fecha mas antigua de los datos a predecir
        long TPND=PR.DATA[0][0];//Fecha mas nueva de los datos a predecir
        std::vector<std::vector<double>> tTPd;//Vectores de datos temporales para las transformaciones necesarias      
        
        /*std::cout<<"Date compatibility checker;"<<std::endl;
        std::cout<<"Date compatibility data:"<<std::endl;
        std::cout<<"PFMD: "<<PFMD<<std::endl;
        std::cout<<"PFSD: "<<PFSD<<std::endl;
        std::cout<<"PFS: "<<PFS<<std::endl;
        std::cout<<"PFOD: "<<PFOD<<std::endl;
        std::cout<<"PFND: "<<PFND<<std::endl;
        std::cout<<std::endl;
        std::cout<<"TPMD: "<<TPMD<<std::endl;
        std::cout<<"TPSD: "<<TPSD<<std::endl;
        std::cout<<"TPS: "<<TPS<<std::endl;
        std::cout<<"TPOD: "<<TPOD<<std::endl;
        std::cout<<"TPND: "<<TPND<<std::endl;
        //DBG*/ 
       if(PR.DATA.size()<=1||VR.DATA.size()<=1){//quandl server has a error
           //std::cout<<"PRID: "<<PRid<<"\tVRID: "<<VRid<<std::endl<<std::endl;
           std::cout<<"ALPHA V ERROR EN DNC obj"<<std::endl;
            DSP=false;
            return false;
        }
        
        if(TPND<=(PFND+(PFMD+PFSD*5))){//Si los datos a predecir desde estan actualizados hasta le fecha más nueva disponible de los datos a predecir, es viable la transformación para generar las predicciones
            std::cout<<"Date compatibility passed;"<<std::endl;
            
            //Busca para cada dato a predecir entre que dos o a cual dato desde el que predecir se encuentra.
            for(int i=0;i<PR.DATA.size();i++){//Bucle principal, 
                int OLDD=-1;
                int NEWD=-1;
                bool ecuali=false;

                for(int r=0;r<VR.DATA.size();r++){//Bucle por los datos a predecir desde por cada dato a predecir
                    
                    if((int)PR.DATA[i][0]==(int)VR.DATA[r][0]){//Las fechas son iguales
                        
                        //std::cout<<"SAME DATE"<<std::endl;
                        tPFd.push_back(VR.DATA[r]);
                        ecuali=true;
                        
                        break;
                    }
                    
                    else if(PR.DATA[i][0]<VR.DATA[r][0]){//Si la fecha de los datos a predecir desde es más nueva que los datos a predecir desde almacena la dirección en NEWD        
                    
                        NEWD=r;
                        //std::cout<<"NEWD:: "<<NEWD<<std::endl;                         
                    }
                    
                    else if(PR.DATA[i][0]>VR.DATA[r][0]){//Si la fecha de los datos a predecir desde es mas antigua guarda la direcciónen OLDD y termina el bucle porque recorre las fechas de nuevas a antiguas        
                    
                        OLDD=r;
                        //std::cout<<"OLDD:: "<<OLDD<<std::endl; 
                        break;
                    }

                }
               // std::cout<<"NEWD:: "<<NEWD<<std::endl; 
               // std::cout<<"OLDD:: "<<OLDD<<std::endl;
                    
                    std::vector<double> tVEC;//Vector para modificación de fechas e interpolación.
                    
                if(ecuali){//Los datos son iguales introduce los datos al vector
                   //std::cout<<"SAME DATE"<<std::endl; 

                }
                else if(NEWD==-1){//Si no existe un dato más nuevo en los datos a predecir desde guarda en el vector el mas nuevo disponible y le pone la fecha de los datos a predecir actual
                    //std::cout<<"NEWD"<<std::endl; 
                    tVEC.clear();
                    tVEC=VR.DATA[0];
                    tVEC[0]=PR.DATA[i][0];
                    tPFd.push_back(tVEC);
                }   
                else if(OLDD==-1){//Si no existe un dato mas antiguo en los datos a predecir desde elimina los datos a predecir mas antiguos, duplica el ultimo valor de los datos a predecir desde y termina la ejecución del bucle 
                    //std::cout<<"OLDD"<<std::endl; 
                    PR.DATA.erase(PR.DATA.begin()+i,PR.DATA.end());                    
                    break;
                }
                else{//La fecha de los datos a predecir se encuentra entre dos de los datos a predecir desde, interpola.
                    //std::cout<<"INTERP"<<std::endl; 
                    tVEC.clear();
                    tVEC.push_back(PR.DATA[i][0]);//Puts the newer date
                    for(int c=1;c<VR.DATA[i].size();c++){
                        tVEC.push_back(ILB(PR.DATA[i][0],VR.DATA[OLDD][0],VR.DATA[NEWD][0],VR.DATA[OLDD][c],VR.DATA[NEWD][c]));
                        //std::cout<<"Interp_VAL:"<<ILB(PR.DATA[i][0],VR.DATA[OLDD][0],VR.DATA[NEWD][0],VR.DATA[OLDD][c],VR.DATA[NEWD][c])<<std::endl;
                    }
                    
                    tPFd.push_back(tVEC);
                }
                
            }
            
            VR.DATA.clear();
            VR.DATA=tPFd;
            tPFd.clear();
            
            /*//dbg
            std::cout<<"PR "<<PR.DATA.size()<<"lines"<<std::endl;
            for (int i=0; i<PR.DATA.size();i++){
                for (int q=0;q<PR.DATA[i].size();q++){
                    std::cout<<PR.DATA[i][q]<<"\t";
                }
                std::cout<<std::endl;
            }
            
            //dbg
            std::cout<<"VR "<<VR.DATA.size()<<"lines"<<std::endl;
            for (int i=0; i<VR.DATA.size();i++){
                for (int q=0;q<VR.DATA[i].size();q++){
                    std::cout<<VR.DATA[i][q]<<"\t";
                }
                std::cout<<std::endl;
            }
           //*/
           
           //En este punto los datos a predecir y a predecir desde tienen el mismo tamaño, y las mismas fechas.
           
            //Create the data to predict from vector
            for(int r=0;r<VR.DATA.size();r++){
                TrDt.push_back(VR.DATA[r]);
            }
            //TrDt.pop_back();//Elimina el ultimo dia puesto que no se utiliza
            //Create the data to predict from vector
            
            //Create the results vector
            
            std::vector<std::vector<double>> SrD;
            std::reverse(PR.DATA.begin(),PR.DATA.end());
            for(int r=1;r<PR.DATA.size();r++){
                std::vector<double> Sr;
                for (int e=0;e<PR.DATA[r].size();e++){
                Sr.push_back(PR.DATA[r][e]-PR.DATA[r-1][e]);
                }
                SrD.push_back(Sr);
            }
            
            RsDt.clear();
            RsDt=SrD;
            std::reverse(PR.DATA.begin(),PR.DATA.end());
            
            //Elimina el primer dia puesto que no se utiliza dejando alineados lo que paso el dia anterior para meter como datos y lo que pasó el dia siguiente para comparar resultados
            //Create the results vector
                            
        //Set the first element on the vector data to make the prediction vector.
            
            for(int i=0;i<VR.DATA.size();i++){
                PrDt.push_back(VR.DATA[i]);
            }                  
            //Complete the vector data to make the prediction vector
                      
                      
            if(PR.DATA.size()==VR.DATA.size()){
                std::cout<<"Data sets are compatilized"<<std::endl;
                DSP=true;
                return true;
            }
            else{
                std::cout<<"BUG"<<std::endl;
                DSP=false;
                return false;
            }
            DBG_DT(0);
            
                    }
        else{//Las bases de datos no son compatibles para la predicción.
            std::cout<<"Data sets are not compatibles"<<std::endl;
            DSP=false;
            return false;
            
        }

        }
    
    void GPRd(){//Gets que data to predict

       
       // DTMN=DBM();//Financial data base manager
        
        PR.ADATA.clear();
        PR.DATA.clear();
        
        DTMN.SD.ADATA.clear();
        DTMN.SD.DATA.clear();
        
        DTMN.FDSR(PRid);
        PR.ADATA=DTMN.SD.ADATA;
        PR.DATA=DTMN.SD.DATA;
        
        qPR=false;
        
                   /* //dbg
            std::cout<<"PR Data recived from quandl "<<PR.DATA.size()<<"lines"<<std::endl;
            for (int i=0; i<PR.DATA.size();i++){
                for (int q=0;q<PR.DATA[i].size();q++){
                    std::cout<<PR.DATA[i][q]<<"\t";
                }
                std::cout<<std::endl;
            }
            //*/
            /*           std::cout<<"Data recived from quandl "<<PR.ADATA.size()<<"lines"<<std::endl;
            for (int i=0; i<PR.DATA.size();i++){
                for (int q=0;q<PR.ADATA[i].size();q++){
                    std::cout<<PR.ADATA[i][q]<<" ";
                }
                std::cout<<std::endl;
            }
            //dbg*/
        
    }
    
    void GVRd(){//Gets the data to predict from.
       
        //DTMN=DBM();//Financial data base manager
        
        VR.ADATA.clear();
        VR.DATA.clear();
        
        DTMN.SD.ADATA.clear();
        DTMN.SD.DATA.clear();
        
        DTMN.FDSR(VRid);
        VR.ADATA=DTMN.SD.ADATA;
        VR.DATA=DTMN.SD.DATA;
        
        qVR=false;
        
                   /* //dbg
           std::cout<<"VR Data recived from quandl "<<VR.DATA.size()<<"lines"<<std::endl;
            for (int i=0; i<VR.DATA.size();i++){
                for (int q=0;q<VR.DATA[i].size();q++){
                    std::cout<<VR.DATA[i][q]<<"\t";
                }
                std::cout<<std::endl;
            }
            ///*
                       std::cout<<"Data recived from quandl "<<VR.ADATA.size()<<"lines"<<std::endl;
            for (int i=0; i<VR.DATA.size();i++){
                for (int q=0;q<VR.ADATA[i].size();q++){
                    std::cout<<VR.ADATA[i][q]<<" ";
                }
                std::cout<<std::endl;
            }
            //dbg*/
        
    }
    

    
    void DPr(std::vector<int> outs, std::vector<int> inputs){//Process the database for nn porposes
       
//On this point the VR and PR vectors contains the data bases with all avaliable history of each stock values avaliable, with the dates converted to posix time.
       
       std::cout<<"PRID: "<<PRid<<"\tVRID: "<<VRid<<std::endl;
        PrDt.clear();    
        RsDt.clear();
        TrDt.clear();
        

                //Obtaining data from quandl
            GPRd();
           //FEr=DTS.errr;
            FEr=false;
        //   std::cout<<"OPR"<<std::endl;
            GVRd();
            //TEr=DTS.errr;
            TEr=false;
        //    std::cout<<"OVR"<<std::endl;
        /*   while(FcC || TcC ){//Ensures data integrity
                GPRd();
                FcC=DTS.errr;
                GVRd();
                TcC=DTS.errr;    
            }*/
                //Obtaining data from quandl
        
        


//Processing data for neural network
if(CompD()&&!FEr&&!TEr&&!qPR&&!qVR){
//Secciona los datos
SecD(outs,inputs);
//Secciona los datos
//Normalize the data with maximuns except time variables
Nrm();
//Normalize the data with maximus except time variables
//std::cout<<"QCR"<<std::endl;
}
        /*else if(FEr||TEr){//Quandl or sintaxis error
            DPr(outs,inputs);
        }*/
else{//Compositor error, data no compatible
    std::cout<<"QERR"<<std::endl;
    //DPr(outs,inputs);
}//Recursive error corrector
            
        }
        
///LEGACY CODE
    void DPr(std::vector<std::vector<double>>pr,std::vector<int> outs, std::vector<std::vector<double>>vr,std::vector<int> inputs){//Process the custom database for nn porposes
       
//On this point the VR and PR vectors contains the data bases with all avaliable history of each stock values avaliable, with the dates converted to posix time.
       
       
        PrDt.clear();    
        RsDt.clear();
        TrDt.clear();
       
        PR.DATA.clear();
        VR.DATA.clear();
        PR.DATA=pr;
        VR.DATA=vr;

            /*//dbg
            std::cout<<"PR "<<PR.DATA.size()<<"lines"<<std::endl;
            for (int i=0; i<PR.DATA.size();i++){
                for (int q=0;q<PR.DATA[i].size();q++){
                    std::cout<<PR.DATA[i][q]<<"\t";
                }
                std::cout<<std::endl;
            }
            
            //dbg
            std::cout<<"VR "<<VR.DATA.size()<<"lines"<<std::endl;
            for (int i=0; i<VR.DATA.size();i++){
                for (int q=0;q<VR.DATA[i].size();q++){
                    std::cout<<VR.DATA[i][q]<<"\t";
                }
                std::cout<<std::endl;
            }
           //*/
//Processing data for neural network
        //std::cout<<"DBG 1"<<std::endl;
if(CompD()){
        // std::cout<<"DBG 2"<<std::endl;
   
//Secciona los datos
SecD(outs,inputs);
//Secciona los datos

        //std::cout<<"DBG 3"<<std::endl;

//Normalize the data with maximuns except time variables
Nrm();
//Normalize the data with maximus except time variables
       // std::cout<<"DBG 4"<<std::endl;


        }

            
        }

///LEGACY CODE
        
	
};

//LEGACY BAD CODE

#endif
