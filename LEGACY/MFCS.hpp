#ifndef MFCS_HPP_INCLUDED
#define MFCS_HPP_INCLUDED

//Marq financial compute sistem principal library

//#include "../FINANCIAL/MRFS.hpp"//Marq financial sistem
#include "FS.hpp"//Financial sistem
#include "FSA.hpp"//Financial sistem analisis
#include "FIR.hpp"//Financial investment rules

#include "../LIB/LINUX/MPR.hpp"//Multi process
#include "../LIB/NET/CLD.hpp"//File and process CLOUD



#include "../LIB/DBGU.hpp"//Debug utilities

#include "../LIB/GR/L64/AXL64.h"
#include "../LIB/GR/GR.h"

//Base software routines

    void MODEEXEC(std::vector<bool>mode,MRFS & fs){//Execution of the modes
        //Parameters modifier
        if(fs.STOK.size()==1){//Params for CCNN
        // fs.PRit=250;
        }
        else{//Params for SNN
        // fs.PRit=250;
        }

        //Parameters modifier
    
        if(mode[1]){//Learning MODE
            fs.LR(mode[0]);
        }
        if(mode[2]){//Predict mode
            fs.PR();
        }
    }
    
    //Split with token function pack
    #ifndef SPLIT
    #define SPLIT
    template<typename Out>
    void split(const std::string &s, char delim, Out result) {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            *(result++) = item;
        }
    }
    std::vector<std::string> split(const std::string &s, char delim) {
        std::vector<std::string> elems;
        split(s, delim, std::back_inserter(elems));
        return elems;
    }
    #endif
    //Split with token function pack
    
    class OM{//Operartion mode class, takes care of executing mode variables and sobroutines, its modifications, and assures the correct combination of operations, as well as give the pertinent information.
        //std::string command;
        std::vector<std::string> arG;
        
        std::vector<bool>MODE={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//Mode storage
        std::vector<std::vector<std::string>> STOCK;//Stock names from comands, last one on each line is the one to predict
        
        int nPr=1;//Number of threads for compute
        
        
    public:
        
        bool EX=true; //Principal loop execution condition
        
        void MP(std::vector<std::string> arG){//Mode parser
            std::vector<std::string> STOKt;//Stock names from comands, last one to predict
        
            std::vector<std::string>  comandos;

            for (int i = 0; i < arG.size(); ++i){//Argumentos del programa
        
                comandos.push_back(arG[i]);
            
                std::cout<<comandos[i]<<std::endl;
            
                //*MODE initializator, all modes can be able at the same time, server mode is recomended to be alone in the thread, multple server instances are alowed
                //No args give instructions and all avaliable Financial data ids
            
                if((comandos[i]==("-E"))||(comandos[i]==("-e"))){
                    MODE[0]=1;
                    //Evolutive mode
                }
                else if((comandos[i]==("-L"))||(comandos[i]==("-l"))){
                    MODE[1]=1;
                    //Learning mode
                }
                else if((comandos[i]==("-P"))||(comandos[i]==("-p"))){
                    MODE[2]=1;
                    //Prediction mode
                }
                else if((comandos[i]==("-S"))||(comandos[i]==("-s"))){
                    MODE[3]=1;
                    //Server mode
                }
                else if((comandos[i]==("-C"))||(comandos[i]==("-c"))){
                    MODE[4]=1;
                    //Client mode, salve
                }
                else if((comandos[i]==("-w"))||(comandos[i]==("-W"))){
                    MODE[5]=1;
                    //Watch mode, shows best actual prediction data
                }
                else if((comandos[i]==("-f"))||(comandos[i]==("-F"))){
                    MODE[6]=1;
                    comandos.push_back(arG[i+1]);
                    nPr = atoi(comandos[i+1].c_str());//Sets the number os treads, number after -f command
                    i++;
                //Fork mode, creates multi process structure
                }
                else if((comandos[i]==("-dbg"))||(comandos[i]==("-DBG"))){
                    MODE[7]=1;
                    //DBG MODE
                }
                else if((comandos[i]==("-g"))||(comandos[i]==("-G"))){
                    MODE[8]=1;
                    //GLOBAL MODE
                }
                else if((comandos[i]==("-x"))||(comandos[i]==("-X"))){
                    MODE[9]=1;
                    //experimental mode
                }
                else if((comandos[i]==("-r"))||(comandos[i]==("-R"))){
                    MODE[10]=1;
                    //Worst mode, selects the current worst neural predictión
                }
                else if((comandos[i]==("-cc"))||(comandos[i]==("-CC"))){
                    MODE[11]=1;
                    //Coretational mode, only coders the corelative predictions on non specified stoks tikets
                }
                else if((comandos[i]==("-ss"))||(comandos[i]==("-SS"))){
                    MODE[12]=1;
                    //Simple mode, only considers the simple predictions on non specified stoks tikets
                }
                else if((comandos[i]==("-q"))||(comandos[i]==("-Q"))){
                    MODE[13]=1;
                    //Focus on only one stok prediction considering all simple and correlative, it gives error if the stok arg is diferent than one 
                }
                else if((comandos[i]==("-i"))||(comandos[i]==("-I"))){
                    MODE[14]=1;
                    //Intra-day *MODE 
                }
                else if((comandos[i]==("-max"))||(comandos[i]==("-MAX"))){
                    MODE[15]=1;
                    //MAX mode 
                }
                else if((comandos[i]==("-min"))||(comandos[i]==("-MIN"))){
                    MODE[16]=1;
                    //MIN mode 
                }
                else if((comandos[i]==("-gr"))||(comandos[i]==("-GR"))){
                    MODE[17]=1;
                    //Global research mode 
                }
                else if((comandos[i]==("-a"))||(comandos[i]==("-A"))){
                    MODE[18]=1;
                    //Global research mode 
                }
                else if((comandos[i]==(":"))&&(STOKt.size()>=1)){
                    STOCK.push_back(STOKt);
                    STOKt.clear();
                    //Constructs the STOK vector
                }
                else{
                    //Stocks names involved, last one TO predict, 1 name trains CR, 2 names trains SR, no names trainis all secuential. secuances of names separater by :
                    STOKt.push_back(comandos[i]);
                }    
            }
            if((STOKt.size()>=1)){
                STOCK.push_back(STOKt);
                STOKt.clear(); 
            }
        }
        
        void MP(std::string command){//Mode parser
                    std::vector<std::string> arG = split(command, ' ');
                    MP(arG);
        }

        void DBG_MODE(){//Prints the detected mode of operation
            
            if(MODE[0]){//Evolutive mode
                 std::cout<<"EM"<<std::endl;
            }
            if(MODE[1]){//Learning mode
                 std::cout<<"LM"<<std::endl;     
            }   
            if(MODE[2]){    //Prediction mode
                 std::cout<<"PM"<<std::endl;     
            }
            if(MODE[3]){//Server mode
                 std::cout<<"SM"<<std::endl;     
            }
            if(MODE[4]){//Client_slave
                 std::cout<<"CM"<<std::endl;     
            }
            if(MODE[5]){//Watch mode
                 std::cout<<"WM"<<std::endl;     
            }
            if(MODE[6]){//Fork
                 std::cout<<"FM"<<std::endl;     
            }
            if(MODE[7]){//Debug mode
                 std::cout<<"DBG"<<std::endl;
            }
            if(MODE[8]){//Global mode
                 std::cout<<"GLB"<<std::endl;     
            }   
            if(MODE[9]){//Experimental mode
                 std::cout<<"EX"<<std::endl;     
            }
            if(MODE[10]){//Worst mode
                 std::cout<<"WR"<<std::endl;     
            }
            if(MODE[11]){//Correlational mode
                 std::cout<<"CC"<<std::endl;     
            }
            if(MODE[12]){//Simple mode
                 std::cout<<"SS"<<std::endl;     
            }
            if(MODE[13]){//Stock focus mode
                 std::cout<<"SFM q"<<std::endl;
            }
            if(MODE[14]){//Intraday mode
                 std::cout<<"ID"<<std::endl;     
            }   
            if(MODE[15]){//Max mode
                 std::cout<<"MAX"<<std::endl;     
            }
            if(MODE[16]){//Min mode
                 std::cout<<"MIN"<<std::endl;     
            }
            if(MODE[17]){//Global researcher
                 std::cout<<"GR"<<std::endl;     
            }
            if(MODE[18]){//Auto mode
                 std::cout<<"AU"<<std::endl;     
            }
        }
        
        void INSTRUCTIONS(){
            //Instructions of use
            std::cout<<"INSTRUCTIONS------------------"<<std::endl;
            
            std::cout<<"-a\tAuto mode"<<std::endl;
            std::cout<<"-e\tEvolutive mode"<<std::endl;
            std::cout<<"-l\tLearning mode"<<std::endl;
            std::cout<<"-p\tPrediction mode"<<std::endl;
            std::cout<<"-s\tServer mode"<<std::endl;
            std::cout<<"-c\tClient mode"<<std::endl;
            std::cout<<"-e\tEvolutive mode"<<std::endl;
            std::cout<<"-w\tPredictión watch mode"<<std::endl;
            std::cout<<"-f int\tMultitread mode, int=number of treads"<<std::endl;
            std::cout<<"-g\tGlobal selector mode"<<std::endl;
            std::cout<<"-x\tExperimental folder selector"<<std::endl;
            std::cout<<"-r\tWorst neural selector"<<std::endl;
            std::cout<<"-cc\tCorelative networks selector"<<std::endl;
            std::cout<<"-ss\tSimple network selector"<<std::endl;
            std::cout<<"-min\tMinimun selector"<<std::endl;
            std::cout<<"-max\tMaximun selector"<<std::endl;
            std::cout<<"-q\tFocus on only one Stok, correlative and simple selector"<<std::endl;
            std::cout<<"-i\tIntra-day mode"<<std::endl;
            std::cout<<"STOKpf STOKtp : STOKtp \tStock tiket input"<<std::endl;
    
            EX=false;
        }
        
        //Mode dependent initialization routines in_ executed at the begining of the execution of the mode.
        
        void in_evolu_sr(MRFS & FS, MPR & FMPE){
            if(MODE[0]){//Evolutive mode
            std::cout<<"EM"<<std::endl;
            }
        }
        
        void in_learn_sr(MRFS & FS, MPR & FMPE){
            if(MODE[1]){//Learning mode
                 std::cout<<"LM"<<std::endl;     
            }
        }
        
        void in_predict_sr(MRFS & FS, MPR & FMPE){
           if(MODE[2]){//Prediction mode
                 std::cout<<"PM"<<std::endl;     
            } 
        }

        void in_server_sr(MRFS & FS, MPR & FMPE){
            if(MODE[3]){//Server mode
                 std::cout<<"SM"<<std::endl;     
                 
            }
        }

        void in_client_sr(MRFS & FS, MPR & FMPE){
            if(MODE[4]){//Client_slave
                 std::cout<<"CM"<<std::endl;     
                 
            }
        }

        void in_multiproc_sr(MRFS & FS, MPR & FMPE){
            if(MODE[6]){//Fork
                 std::cout<<"FM_"<<nPr<<std::endl;     
            }
        }

        void in_globalcorr_sr(MRFS & FS, MPR & FMPE){
            if(MODE[8]&&STOCK.size()==1){//Global correlation database selection
                // EX=false;
                FS.NNDM.NCF="G_DS";
                FS.PRDDM.PDBF="G_PRD";
                FS.PRit=50; 
                FS.LRit=50;
                FS.AQFS();
                //1º//
                STOCK=FS.GSC(STOCK[0][0]);//test all posibilities 1º
                //2º//
                
            //FS.DTSL(49,STOCK[0][0],"G_DS","C_DS",FS.GBNC());//Saves the n best configs to neural configuration for prediction folder 2º
   
                //return 0;
            }
        }
        
        void in_globrese_sr(MRFS & FS, MPR & FMPE){
            if(MODE[17]){//Global rersearch database selection
                FS.NNDM.NCF="GR_DS";
                FS.PRDDM.PDBF="GS_PRD";
                FS.PRit=50; 
                FS.LRit=50;
                FS.AQFS();
                STOCK=FS.GRSC();
                //MODE[1]=1;//Learning mode activation
            }//Global correlation database selection
        }

        void in_experi_sr(MRFS & FS, MPR & FMPE){
            if(MODE[9]){//Experimental mode
                FS.NNDM.NCF="X_DS";
                FS.PRDDM.PDBF="X_PRD";
                FS.PRDDM.STPF="X_ST";
                std::cout<<"EX"<<std::endl;
            }
        }

        void in_worst_sr(MRFS & FS, MPR & FMPE){
           if(MODE[10]){//Worst mode
                 std::cout<<"WM"<<std::endl;     
            } 
        }

        void in_correla_sr(MRFS & FS, MPR & FMPE){
            if(MODE[11]){//Corelative mode
                 std::cout<<"CRM"<<std::endl;     
            }
        }
        
        void in_simpl_sr(MRFS & FS, MPR & FMPE){
            if(MODE[12]){//Simple mode
                 std::cout<<"SMM"<<std::endl;     
            }
        }

        void in_focus_sr(MRFS & FS, MPR & FMPE){
            if(MODE[13]){//Focus mode
                std::cout<<"FCM"<<std::endl; 
                if(STOCK.size()!=1||STOCK[0].size()!=1){
                    std::cout<<"FCM ERROR"<<std::endl; 
                    EX=false;
                }
                FS.AQFS();
                std::vector<std::vector<std::string>> STOCKtt;
     
                if(MODE[12]||!MODE[11])//If simple mode or not corelative mode
                    for(int s=0;FS.NNDM.VSNC.size()>s;s++){//Iterates for all SNNC composing the focus STOCK ticket vector
                    //std::cout<<"DBG "<<FS.DBFN.VSNC[s][0]<<" : "<<FS.DBFN.VSNC[s][FS.DBFN.VSNC[s].size()-1]<<"<<>>"<<STOCK[0][0]<<std::endl;
                    if(FS.NNDM.VSNC[s][FS.NNDM.VSNC[s].size()-1]==STOCK[0][0]){//Focus mode condition
                    STOCKtt.push_back(FS.NNDM.VSNC[s]);
                    }
                }
                if(MODE[11]||!MODE[12])//If corelative mode or not simple mode
                    for(int s=0;FS.NNDM.VCRNC.size()>s;s++){//Iterates for all CCNNC composing the focus STOCK ticket vector
                    //std::cout<<"DBG "<<FS.DBFN.VSNC[s][0]<<" : "<<FS.DBFN.VSNC[s][FS.DBFN.VSNC[s].size()-1]<<"<<>>"<<STOCK[0][0]<<std::endl;
                        if(FS.NNDM.VCRNC[s]==STOCK[0][0]){//Focus mode condition
                        STOCKtt.push_back({FS.NNDM.VCRNC[s]});
                        }
        
                    }
                STOCK=STOCKtt;
            }
        }

        void in_intrad_sr(MRFS & FS, MPR & FMPE){
            if(MODE[14]){//Intra day mode
                std::cout<<"I_D"<<std::endl; 
                FS.NNDM.NCF="RM_DS";
                FS.PRDDM.PDBF="R_PRD";
                FS.PRDDM.STPF="R_ST";
                FS.CLO.DTMN.qF="TIME_SERIES_INTRADAY";
                FS.CLO.DTMN.qI="15min";//1min,5min,15min,30min,60min only for time series intraday
                FS.TPS=15*60;
            }
        }

        void in_max_sr(MRFS & FS, MPR & FMPE){
            if(MODE[15]){//MAX mode
                 std::cout<<"MAX"<<std::endl; 
                FS.NNDM.NCF="A_DS";
                FS.PRDDM.PDBF="A_PRD";
                FS.PRDDM.STPF="A_ST";
                FS.TOP={2};
            }
        }

        void in_min_sr(MRFS & FS, MPR & FMPE){
            if(MODE[16]){//MIN mode
                std::cout<<"MIN"<<std::endl; 
                FS.NNDM.NCF="I_DS";
                FS.PRDDM.PDBF="I_PRD";
                FS.PRDDM.STPF="I_ST";
                FS.TOP={3};
            }
        }

        void in_dgb_sr(MRFS & FS, MPR & FMPE){
            if(MODE[7]){//Debug or testing executed the first cicle
                //DBM FLS;
                //FLS.GFN();
                //FLS.NCFNP();
                //FLS.PFNP();
                //FLS.SDBN();
                EX=false;
                //FS.AQFS();//First data adq, need for program to not block
                //FS.DBFN.CCNC();//Checks and repair corrupted configs
                //FS.DBFN.FDSR("GOOG");
    
                DBG_STD(FS.GBNC(),"Best config vector:",true);
    
            }
        }

        void in_stk_sr(MRFS & FS, MPR & FMPE){
            if(STOCK.size()>0){
                //DBG
                for(int i=0;i<STOCK.size();i++){
                    for(int e=0;e<STOCK[i].size();e++){
                        std::cout<<STOCK[i][e]<<"\t";
                    }
                    std::cout<<std::endl;
                }
            //DBG
            }
        }

        void in_watch_sr(MRFS & FS, MPR & FMPE){
            if(MODE[5]){//Watch
                std::cout<<"WM"<<std::endl;                  
                FIR RULE;
                FS.AQFS();
                  
                for(int y=0;y<FS.PRDDM.PCRP.size();y++){//Iterates over all corelative prediction files
                     
                    RULE.LPRD(FS.PRDDM.CRP[y]);
                    RULE.PROb();
                    RULE.PROs();
                    RULE.LOG();
                     
                    FS.PRR.LCSV(FS.PRDDM.PCRP[y]);
                    FS.STSC();
                    FS.STOK={FS.PRDDM.CRP[y]};
                    FS.PLOG();
                 }
    
                 EX=false;
            }
        }
        
        void in_autom_sr(MRFS & FS, MPR & FMPE){//Automatic mode, analizes the current state of the file sistem, and predictions to give the next instruction.
            if(MODE[18]){
                
            }
        }

        
        void GMIR(MRFS & FS, MPR & FMPE){//Global initialization of mode routines
            
            //DBG_MODE();
            
            in_evolu_sr(FS,FMPE);
            in_learn_sr(FS,FMPE);
            in_predict_sr(FS,FMPE);
            in_server_sr(FS,FMPE);
            in_client_sr(FS,FMPE);
            in_multiproc_sr(FS,FMPE);
            in_globalcorr_sr(FS,FMPE);
            in_globrese_sr(FS,FMPE);
            in_experi_sr(FS,FMPE);
            in_worst_sr(FS,FMPE);
            in_correla_sr(FS,FMPE);
            in_simpl_sr(FS,FMPE);
            in_focus_sr(FS,FMPE);
            in_intrad_sr(FS,FMPE);
            in_max_sr(FS,FMPE);
            in_min_sr(FS,FMPE);
            in_dgb_sr(FS,FMPE);

            in_watch_sr(FS,FMPE);
            in_autom_sr(FS,FMPE);
            
            FS.AQFS();//First data adq, need for program to not block
        }
        //Mode dependent initialization routines
        
        //Mode dependent loop execution routines
        
        void ex_evolu_sr(MRFS & FS, MPR & FMPE){
            if(MODE[0]){//Evolutive mode
            
            }
        }
        
        void ex_learn_sr(MRFS & FS, MPR & FMPE){
            if(MODE[1]){//Learning mode
                     
            }
        }
        
        void ex_predict_sr(MRFS & FS, MPR & FMPE){
           if(MODE[2]){//Prediction mode
                    
            } 
        }

        void ex_server_sr(MRFS & FS, MPR & FMPE){
            if(MODE[3]){//Server mode
                     
            }
        }

        void ex_client_sr(MRFS & FS, MPR & FMPE){
            if(MODE[4]){//Client_slave
                      
            }
        }

        void ex_multiproc_sr(MRFS & FS, MPR & FMPE){
            if(MODE[6]){//Fork
               // std::cout<<"FM_EX"<<std::endl;
                FMPE.CSPS(nPr);
                //std::cout<<"FM_EC_CR"<<std::endl;
                if(FMPE.PRN()!=-1){//If the current proces is a child
                    EX=false;//Prevent the children to create more childrens
                }
            std::cout<<"Multi thread setup correct"<<std::endl;
            }
        }

        void ex_globalcorr_sr(MRFS & FS, MPR & FMPE){
            if(MODE[8]&&STOCK.size()==1){//Global correlation database selection

            }
        }
        
        void ex_globrese_sr(MRFS & FS, MPR & FMPE){//Global rersearch database selection
            if(MODE[17]){

            }
        }

        void ex_experi_sr(MRFS & FS, MPR & FMPE){
            if(MODE[9]){//Experimental mode

            }
        }

        void ex_worst_sr(MRFS & FS, MPR & FMPE){//Worst mode, selects the worst neural net and put it on the STOCK string
    
            if(MODE[10]){
                STOCK.clear();
                if(FMPE.PRN()!=-1){//If the current procesor is not the master
                    for(int bg=0;bg<FMPE.PRN();bg++)STOCK.push_back({"0"});//Nedeed for the multitread to compute the current solicitation
                    STOCK.push_back(FS.NWNNC(FMPE.PRN()));
                }

            }
        }

        void ex_correla_sr(MRFS & FS, MPR & FMPE){
            if(!(STOCK.size()>0)){//Corelative mode
                if(MODE[11]||!MODE[12]){//If corelative mode or not simple mode
                    if(FMPE.PRN()!=-1){
                        for(int s=FMPE.PRN();FS.NNDM.VCRNC.size()>s;s=s+nPr){//Iterates for all CCNNC
        
                            std::cout<<"Thread: "<<FMPE.PRN()<<std::endl;
                            FS.AQFS();
                            FS.STOK={FS.NNDM.VCRNC[s]};
                            if(FS.INITNNS()){
                                MODEEXEC(MODE,FS);
                            }
                        }
                    }
                }
            }
        }
        
        void ex_simpl_sr(MRFS & FS, MPR & FMPE){
            if(!(STOCK.size()>0)){//if dont exist especifided stock tickets on arguments
                 if(MODE[12]||!MODE[11]){//If simple mode or not corelative mode
                     if(FMPE.PRN()!=-1){
                        for(int s=FMPE.PRN();FS.NNDM.VSNC.size()>s;s=s+nPr){//Iterates for all SNNC
                            //std::cout<<"DBG "<<FS.DBFN.VSNC[s][0]<<" : "<<FS.DBFN.VSNC[s][FS.DBFN.VSNC[s].size()-1]<<"<<>>"<<STOK[0][0]<<std::endl;
            
                            //std::cout<<"DBG FS.AQFS"<<std::endl;
                            std::cout<<"Thread: "<<FMPE.PRN()<<"/"<<nPr<<std::endl;
                            FS.AQFS();
                            //std::cout<<"DBG FS.AQFS"<<std::endl;
                            FS.STOK=FS.NNDM.VSNC[s];
                            if(FS.INITNNS()){
                                MODEEXEC(MODE,FS);
                
                            }
                        }  
                    }
                }
            }
        }

        void ex_focus_sr(MRFS & FS, MPR & FMPE){
            if(MODE[13]){//Focus mode
            
            }
        }

        void ex_intrad_sr(MRFS & FS, MPR & FMPE){
            if(MODE[14]){//Intra day mode

            }
        }

        void ex_max_sr(MRFS & FS, MPR & FMPE){
            if(MODE[15]){//MAX mode

            }
        }

        void ex_min_sr(MRFS & FS, MPR & FMPE){
            if(MODE[16]){//MIN mode

            }
        }

        void ex_dgb_sr(MRFS & FS, MPR & FMPE){
            if(MODE[7]){//Debug or testing executed the first cicle
    
            }
        }

        void ex_stk_sr(MRFS & FS, MPR & FMPE){//Concrete tickets specified
            if(STOCK.size()>0){
                if(FMPE.PRN()!=-1){//If is a child
                    for(int s=FMPE.PRN();STOCK.size()>s;s=s+nPr){//Iterater over all NN especified
                        std::cout<<"Thread: "<<FMPE.PRN()<<std::endl;
                        FS.AQFS();
                        FS.STOK=STOCK[s];
                        if(FS.INITNNS()){
                            MODEEXEC(MODE,FS);
                        }
                    }
                }
                
            }
        }

        void ex_watch_sr(MRFS & FS, MPR & FMPE){
            if(MODE[5]){//Watch

            }
        }
        
        void ex_autom_sr(MRFS & FS, MPR & FMPE){//Automatic mode, analizes the current state of the file sistem, and predictions to give the next instruction.
            if(MODE[18]){
                
            }
        }
        
        void GMER(MRFS & FS, MPR & FMPE){//Global mode execution routines
            ex_evolu_sr(FS, FMPE);
            ex_learn_sr(FS, FMPE);
            ex_predict_sr(FS, FMPE);
            ex_server_sr(FS, FMPE);
            ex_client_sr(FS, FMPE);
            //std::cout<<"EX_STACK_1_EXECUTED"<<std::endl; 
            ex_globalcorr_sr(FS, FMPE);
            ex_globrese_sr(FS, FMPE);
            ex_experi_sr(FS, FMPE);
            ex_worst_sr(FS, FMPE);
            //std::cout<<"EX_STACK_2_EXECUTED"<<std::endl; 
            ex_focus_sr(FS, FMPE);
            ex_intrad_sr(FS, FMPE);
            ex_max_sr(FS, FMPE);
            ex_min_sr(FS, FMPE);
            ex_dgb_sr(FS, FMPE);
            //std::cout<<"EX_STACK_3_EXECUTED"<<std::endl; 
            ex_watch_sr(FS, FMPE);
            ex_autom_sr(FS, FMPE);
            //std::cout<<"EX_STACK_4_EXECUTED"<<std::endl; 
            ex_multiproc_sr(FS, FMPE);
            //std::cout<<"EX_STACK_4.1_EXECUTED"<<std::endl; 
            ex_stk_sr(FS, FMPE);
            //std::cout<<"EX_STACK_4.2_EXECUTED"<<std::endl; 
            ex_simpl_sr(FS, FMPE);
            //std::cout<<"EX_STACK_4.3_EXECUTED"<<std::endl; 
            ex_correla_sr(FS, FMPE);
            //std::cout<<"EX_STACK_5_EXECUTED"<<std::endl; 
            
            FS.AQFS();//Adquire the files for the next iteration
        }
    };
    

    
//Base software routines

//Graphic routines

//Graphic routines
#endif
