//g++ FINANCIAL.cpp -std=c++1z -lssl -lcrypto -o FINANCIAL -lstdc++fs

#include "FC.hpp"



int main (int argc, char* argv[]){

    //DBG                      
    std::cout<<std::endl;  
    std::cout<<"   `--`   .-.  ------`   -:/:.   -::-   "<<std::endl;
    std::cout<<"   +MMm  :MMN` MMMNNN:`sNMmdmMy-mMmdMy  "<<std::endl;
    std::cout<<"   dMMMo`mMMM: MMd///.mMm-   `-+MMs-.   "<<std::endl;
    std::cout<<"  `MMoNMhMydMy MMNddd/MMy       :ymMNs` "<<std::endl;
    std::cout<<"  /MM.+MMN`sMN MMy    yMMo-.-o+`s:.oMM: "<<std::endl;
    std::cout<<"  smd  hm/ :mm-mms     :ymMMNm/:dNMMmo  "<<std::endl;
    std::cout<<"  `` ``...  .-` `-`  `..` .    .``...`  "<<std::endl;
    std::cout<<"  so osy-:so-`+/y:+  oMoNyM-  ymh/hMo.  "<<std::endl;
    std::cout<<"  s-s-sy..so::s//-y-osM+/:Mo++NsN++M    "<<std::endl;
    std::cout<<"  ` ` ``  ` `````.  .`.   ....` ```.    "<<std::endl;
    std::cout<<std::endl;  
    
    //DBG
    std::cout<<std::endl<<"   MRQS.PLAT FINANCIAL COMPUTE SISTEMS"<<std::endl;
    std::cout<<"   MFCS V1.1 "<<(char)0xA9<<" MRQS 2018" <<std::endl<<std::endl;
    //DBG
    
    OM EMCO;//Execution mode container object  
    //std::cout<<"OM OBJECT CREATED" <<std::endl;
    
    OEX EXC;//Executor container
    //std::cout<<"OEX OBJECT CREATED" <<std::endl;

    //Program argument handler
    if (argc > 1) {//Existen argumentos
        std::vector<std::string> AARRGG;
        for (int i = 1; i < argc; ++i){//Argumentos del programa
            AARRGG.push_back(argv[i]);
            //std::cout<<"MFCS_1_0 argv: "<<argv[i]<<std::endl;
        }
            
        EMCO.MP(AARRGG);//Direct command parser, manages the arguments pased on the comon execution
    }
    
    //std::cout<<"OM.MP PROCESED" <<std::endl;
    
    if(!(argc > 1)){//no args
        EMCO.INSTRUCTIONS();
        return 0;
    }
    
    FS FCS;//Financial core sistem
    //std::cout<<"FS OBJECT CREATED" <<std::endl;

    EXC.GMIR(FCS,EMCO);//Global initialization of mode routines
    //std::cout<<"OEX.GMIR PROCESED" <<std::endl;


    //EMCO.EX=false;
    
    while(EMCO.CONF.EX){//EX loop
     //   CRONO.STRT();//Initialice crono measurment;

        EXC.GMER(FCS,EMCO);        
      //  CRONO.STP();//Stop crono measurment;

//    std::cout<<BBLU("          ");
 //   std::cout<<BBLU(CRONO.LPS()/1000);
 //   std::cout<<BBLU("          ")<<std::endl;
  
    }//EX loop

    return 0;
}
