#ifndef FPM_HPP_INCLUDED
#define FPM_HPP_INCLUDED


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

#include "CMD.hpp"

class FPM{///Financial prediction manager
public:
    
FDR FACC;//CLOUD FILE, if not server is found local file is selected, FACC.IP.size()==0

CSV TMPD;//Temporal prediction data, used for load, analisis and modify functions

std::vector<CSV> PD;//Prediction data
    

std::string PDBF="C_PRD";//Prediction data base folder
std::string PDBT="PRD_";//Prediction data base token
CSV P;//Prediction names

std::vector<std::vector<std::string>> PTN;//Prediction token names
std::vector<std::string> PTP;//Prediction token path

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
    FACC.PH(PDBF);
    
    P.DEC(FACC.DATA);
    std::sort(P.ADATA.begin(), P.ADATA.end(), [](const std::vector< std::string >& a, const std::vector< std::string >& b){ return a[0] < b[0]; } );

}
///Comon functions

void PFNP(){//Prediction file name parser
    PTN.clear();
    PTP.clear();

    for(int i=0;i<P.ADATA.size();i++){//Itera por el nombre de todos los archivos
        
        if(P.ADATA[i][0].find(PDBT)!=std::string::npos){//Encuentra el token buscado para eliminar otros posibles archivos
           
                //std::size_t frs = NC[i].find(NCF);            
                //NC[i]= NC[i].substr(frs+NCF.size()+1+6);//Elimina la dirección y el token del string 
                
                //NC[i].erase(NC[i].size() - (1+3),3);//Deletes file extensión
                
                std::vector<int>pp=TKP(P.ADATA[i][0]);//Vector con las posiciones de todos los tokens
                
                if(pp.size()>=3){//Si existen dos o mas identificadores de STOK
                    PTP.push_back(P.ADATA[i][0]);
                    std::vector<std::string> tmpsv;
                    for(int e=1;e<pp.size()-1;e++){//Itera por los tokens excepto el (primero,segundo) y el ultimo
                        tmpsv.push_back(P.ADATA[i][0].substr(pp[e]+1,pp[e+1]-(pp[e]+1)));//Cuts and puts on the temp vector the STOK names.   
                    }
                    PTN.push_back(tmpsv);
                    tmpsv.clear();
                    
                }
                
            }

        
    }
}

std::string PFN(std::vector<std::string> stksmb){//Make file name from stock tickets
    std::string filename=PDBF+"/"+PDBT;
    filename.erase(filename.end()-1);//Deletes the first "_" avoiding duplicates.
    for(int y=0;y<stksmb.size();y++){
        filename+="_"+stksmb[y];
    }
    filename+=".csv";
    return filename;
}

void LPF(std::vector<std::string> stksmb){//Load prediction data
    FACC.LF(PFN(stksmb));//Loads the current db
    TMPD.DEC(FACC.DATA);//CSV DECODE FILE DATA;
    
    if(TMPD.DATA.size()==0){
        std::cout<<"NON_EXISTENT_PRDT_FILE"<<std::endl;
    }
    //DBG_STD(TMPD.DATA,"FPM_TMPD");
}

void LPF(std::string stksmb){//Load prediction data
    FACC.LF(PDBF+"/"+PDBT+stksmb+".csv");//Loads the current db
    TMPD.DEC(FACC.DATA);//CSV DECODE FILE DATA;
    
    if(TMPD.DATA.size()==0){
        std::cout<<"NON_EXISTENT_PRDT_FILE"<<std::endl;
        TMPD.DATA.push_back({0,0,0,0});
        TMPD.DATA.push_back({1,0,0,0});
        TMPD.DATA.push_back({2,0,0,0});
        
        TMPD.ADATA.push_back({"0","0","0","0"});
        TMPD.ADATA.push_back({"1","0","0","0"});
        TMPD.ADATA.push_back({"2","0","0","0"});
    }
    //DBG_STD(TMPD.DATA,"FPM_TMPD");
}

void SPF(std::vector<std::string> stksmb){//Saves predition file
    //Exist a problem to load, save and hadle correlative predictions names 
    FACC.SF(PFN(stksmb),TMPD.ENC());
}
void SPF(std::string stksmb){//Saves predition file
    //Exist a problem to load, save and hadle correlative predictions names 
    FACC.SF(PDBF+"/"+PDBT+stksmb+".csv",TMPD.ENC());
}

};

#endif
