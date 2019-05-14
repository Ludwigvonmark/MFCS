#ifndef FNM_HPP_INCLUDED
#define FNM_HPP_INCLUDED


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
#include "../LIB/PARSR/CSV.hpp"
#include "../LIB/FDR.hpp"

class FNM{///Financial neural manager
public:
    
FDR FACC;//CLOUD FILE, if not server is found local file is selected, FACC.IP.size()==0
    
std::string NCF="C_DS";//Neural configuration folder
std::string NCT="FNND_";//Neural configuration token

CSV NC;//Neural configs names

std::vector<std::vector<std::string>> CNCN;//Current neural configuration names
std::vector<std::vector<std::string>> BNCN;//Best neural configuration names
std::vector<std::vector<std::string>> VNCN;//Very best neural configuration names

std::vector<std::string> CNCP;//Current neural configuration path
std::vector<std::string> BNCP;//Best neural configuration path
std::vector<std::string> VNCP;//Very best neural configuration path


std::vector<std::string> CST;//Current stocks tickets presents on neural configuration loaded
CSV CNC;//Current neural config
CSV BNC;//Best neural config
CSV VNC;//Very best neural config. Trained best neural config

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
    FACC.PH(NCF);
    NC.DEC(FACC.DATA);
    std::sort(NC.ADATA.begin(), NC.ADATA.end(), [](const std::vector< std::string >& a, const std::vector< std::string >& b){ return a[0] < b[0]; } );

}
///Comon functions

///Neural database functions
void NCFNP(){//Neural configuration file name parser
    CNCN.clear();
    BNCN.clear();
    VNCN.clear();
    
    CNCP.clear();
    BNCP.clear();
    VNCP.clear();

    for(int i=0;i<NC.ADATA.size();i++){//Itera por el nombre de todos los archivos
        
        if(NC.ADATA[i][0].find(NCT)!=std::string::npos){//Encuentra el token buscado para eliminar otros posibles archivos
           
            if(!NC.ADATA[i][0].compare(NCF.size()+1,1,"B")){//Si es del tipo B
                
                std::vector<int>pp=TKP(NC.ADATA[i][0]);//Vector con las posiciones de todos los tokens
                
                if(pp.size()>=3){//Si existen uno o mas identificadores de STOK
                    BNCP.push_back(NC.ADATA[i][0]);
                    std::vector<std::string> tmpsv;
                        for(int e=1;e<pp.size()-1;e++){//Itera por los tokens excepto el (primero,segundo) y el ultimo
                            tmpsv.push_back(NC.ADATA[i][0].substr(pp[e]+1,pp[e+1]-(pp[e]+1)));//Cuts and puts on the temp vector the STOK names.   
                        }
                    BNCN.push_back(tmpsv);
                    tmpsv.clear();
                    
                }
                
            }
             
             
            else if(!NC.ADATA[i][0].compare(NCF.size()+1,1,"C")){//Si es del tipo C
                
                std::vector<int>pp=TKP(NC.ADATA[i][0]);//Vector con las posiciones de todos los tokens
                
                if(pp.size()>=3){//Si existen uno o mas identificadores de STOK
                    CNCP.push_back(NC.ADATA[i][0]);
                    std::vector<std::string> tmpsv;
                    for(int e=1;e<pp.size()-1;e++){//Itera por los tokens excepto el (primero,segundo) y el ultimo
                        tmpsv.push_back(NC.ADATA[i][0].substr(pp[e]+1,pp[e+1]-(pp[e]+1)));//Cuts and puts on the temp vector the STOK names.   
                    }
                    CNCN.push_back(tmpsv);
                    tmpsv.clear();
                    
                }
                
            }
            
            else if(!NC.ADATA[i][0].compare(NCF.size()+1,1,"V")){//Si es del tipo V
                
                std::vector<int>pp=TKP(NC.ADATA[i][0]);//Vector con las posiciones de todos los tokens
                
                if(pp.size()>=3){//Si existen uno o mas identificadores de STOK
                    VNCP.push_back(NC.ADATA[i][0]);
                    std::vector<std::string> tmpsv;
                    for(int e=1;e<pp.size()-1;e++){//Itera por los tokens excepto el (primero,segundo) y el ultimo
                        tmpsv.push_back(NC.ADATA[i][0].substr(pp[e]+1,pp[e+1]-(pp[e]+1)));//Cuts and puts on the temp vector the STOK names.   
                    }
                    VNCN.push_back(tmpsv);
                    tmpsv.clear();
                    
                }
                
            }
            
        }
    }
   // DBG_STD(VNCN,"VNCN");
   // DBG_STD(BNCN,"BNCN");
   // DBG_STD(CNCN,"CNCN");
}

std::string MFN(std::string type,std::vector<std::string> stksmb){//Make file name from stock tickets
    std::string filename=NCF+"/"+type+NCT;
    filename.erase(filename.end()-1);
    for(int y=0;y<stksmb.size();y++){
        filename+="_"+stksmb[y];
        //std::cout<<"FNM_NFM_stksmb: "<<stksmb[y]<<std::endl;
    }
    filename+=".csv";
    
    //std::cout<<"FNM_NFM_filename: "<<filename<<std::endl;
    return filename;
}

void LNNC(std::vector<std::string> stksmb){//Load neural network configuration
    CST=stksmb;
        
    FACC.LF(MFN("C",stksmb));//Loads the current db
    CNC=CSV(24);
    CNC.DEC(FACC.DATA);//CSV DECODE FILE DATA;
    
    FACC.LF(MFN("B",stksmb));//Loads the current db
    BNC=CSV(24);
    BNC.DEC(FACC.DATA);//CSV DECODE FILE DATA;
    
    FACC.LF(MFN("V",stksmb));//Loads the current db
    VNC=CSV(24);
    VNC.DEC(FACC.DATA);//CSV DECODE FILE DATA;
}

void SNNC(std::string type){//Save neural network configuration
    if(type=="V"){
    FACC.SF(MFN(type,CST),VNC.ENC());
    }
    else if(type=="B"){
    FACC.SF(MFN(type,CST),BNC.ENC());
    }
    else if(type=="C"){
    FACC.SF(MFN(type,CST),CNC.ENC());
    }
}


};

#endif
