#ifndef FSA_HPP_INCLUDED
#define FSA_HPP_INCLUDED
//Financial sistem analisis.

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

#include <iostream> //cout,cin

//Analisis sub routines
std::vector<std::string> ALPSRT(std::vector<std::string> unptick, int oss){//Alphabetic soting stock names considering to predict structure
        std::vector<std::vector<std::string>>sttin;
        for(int u=0;u<unptick.size()-oss;u++){
            sttin.push_back({unptick[u],std::to_string(u)});
        }
        std::sort(sttin.begin(), sttin.end(), [](const std::vector< std::string >& a, const std::vector< std::string >& b){ return a[0] < b[0]; } ); 
        //Creates the position vector for inputs
        
        //Creates the position vector for outputs
        std::vector<std::vector<std::string>>sttou;
        for(int u=unptick.size()-oss;u<unptick.size();u++){
            sttou.push_back({unptick[u],std::to_string(u)});
        }
        std::sort(sttou.begin(), sttou.end(), [](const std::vector< std::string >& a, const std::vector< std::string >& b){ return a[0] < b[0]; } );
        //DBG_STD(sttou,"sttou");
        //Creates the position vector for outputs

        std::vector<std::string> TEMPSTK;
        for(int i=0;i<sttin.size();i++){
            TEMPSTK.push_back(sttin[i][0]);
        }
        for(int i=0;i<sttou.size();i++){
            TEMPSTK.push_back(sttou[i][0]);
        }
        return TEMPSTK;
}

std::vector<std::vector<std::string>> ALPSRT(std::vector<std::vector<std::string>> unptick, int oss){//Alphabetic soting stock names considering to predict structure
    std::vector<std::vector<std::string>>RESSU;
    for(int u=0;u<unptick.size();u++){
    RESSU.push_back(ALPSRT(unptick[u],oss));
    }
    return RESSU;
}



//Análisis sub routines



class FSA {//Financial sistem análisis, checks the state of the sistem and selects the current operation as well as give a apoximate number of flops necesary to process it.
    //the operations are the simplesto posible, like stock to predict from, stock to predict, folder, etc...
public:
   
    FSA(){}
    
    void LALL(){//Loads all existent files on all folders
       
    } 
    
    void LOG(){//Shows the data consider to make the decisión
        
    }

    std::string GNO(){//Gets the next op to execute based on the sistem state
       
    } 
    
};

#endif
