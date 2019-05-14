#ifndef MFEF_HPP_INCLUDED
#define MFEF_HPP_INCLUDED
//MRQS.plat FINANCIAL EXCHANGE FORMAT parser

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

class MFEF{///Mrqs financial exchange format object parser/container
public:
    const std::vector<std::string> LABEL={"UPDT_TIME","START_PRICE","HIGH","LOW","END_PRICE","EXTENDED_PRICE","LAPSED_VOLUME","POPULARITY"};
    
    CSV MFEFDATA;
    
    MFEF(){
        
    }
    std::vector<std::string> FLABEL(std::vector<std::vector<std::string>>labelPOS,std::string target){
        for(int a=0;a<labelPOS.size();a++){
            if(target==labelPOS[a][0]){
                return labelPOD[a];
            }
        }
        return {std::to_string(0)};
    }
    
    void GLABEL(std::string rawdata){//Get the labels and the position
        MFEF.DEC(rawdata);
        if(MFEF.ADATA[0].size()>0){
            std::vector<std::vector<std::string>>labelPOS;
            for(int a=0;a<MFEF.ADATA[0].size();a++){
                labelPOS.push_back({MFEF.ADATA[0][a],std::to_string(a)});
            }
            
            for (int b=0;b<labelPOS.size();b++){
                switch(labelPOS[b][0]){
                    case "timestamp":
                        
                        break;
                        
                    case "date":
                        
                        break;
                    
                    case "open":
                        
                        break;
                        
                    case "high":
                        
                        break;
                        
                    case "low":
                        
                        break;
                        
                    case "close":
                        
                        break;
                        
                    case "adjusted_close":
                        
                        break;
                        
                    case "volume":
                        
                        break;
                        
                    case "dividend_amount":
                        
                        break;
                    
                    case "split_coefficient":
                        
                        break;
                        
                    case "vwap":
                        
                        break;

                        
                    default:
                        date,open,high,low,close,volume,unadjustedVolume,change,changePercent,vwap,label,changeOverTime
                        
                        date,minute,label,high,low,average,volume,notional,numberOfTrades,marketHigh,marketLow,marketAverage,marketVolume,marketNotional,marketNumberOfTrades,open,close,marketOpen,marketClose,changeOverTime,marketChangeOverTime
                }
            }
            
            
            
        }
    }
    
    void DEC(std::vector<std::string> rawdata){//Decode the given RAWDATA
        
    }
    
};

#endif
