#ifndef FC_HPP_INCLUDED
#define FC_HPP_INCLUDED

//Marq financial compute sistem principal library

#include "../LIB/LINUX/MPR.hpp"//Multi process
#include "../LIB/FDR.hpp"//File and process CLOUD

#include "../LIB/DBGU.hpp"//Debug utilities

#include "../LIB/GR/L64/AXL64.h"
#include "../LIB/GR/GR.h"

enum FCF{//Financial compute function
    LEARN,
    EVOLU,
    PREDI,
    FOCUS,
    SERVE,
    CLIEN,
    WATCH,
    MULTI,
    EPOCH,
    EXPER,
    GLOBA,
    AUTOM,
    DEBUG
    };

    struct FCC{//Financial compute config
        bool EX=true; //Principal loop execution condition
        
        std::string NCF="C_DS";//Neural configuration folder
        std::string PDBF="C_PRD";//Prediction data base folder
        
        std::vector<FCF>MODE;//Mode storage
        std::vector<std::vector<std::string>> STOCK;//All stock tickets on the 
        
        int nPr=1;//Number of threads for compute
        
        int OSS=1;//Number of output vector on the prediction side
        long epoch=60*60*24;
        
        std::vector<int> inST={0,1,2,3,4,5};//Intput data set
        std::vector<int> outST={2,3,4};//Output data set. High, low, close
        
        std::vector<int> preST={1,2,3};//Output data set. High, low, close
    };


#include "FS.hpp"//Financial sistem


//Base software routines

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
        
        public:
        std::vector<std::string> arG;
        FCC CONF;
        
        void MP(std::vector<std::string> arG){//Mode parser
            std::vector<std::string> STOKt;//Stock names from comands, last one to predict
        
            std::vector<std::string>  comandos;

            for (int i = 0; i < arG.size(); ++i){//Argumentos del programa
        
                comandos.push_back(arG[i]);
            
                std::cout<<comandos[i]<<std::endl;
            
                //*MODE initializator, all modes can be able at the same time, server mode is recomended to be alone in the thread, multple server instances are alowed
                //No args give instructions and all avaliable Financial data ids
            
                if((comandos[i]==("-E"))||(comandos[i]==("-e"))){
                    CONF.MODE.push_back(EVOLU);
                    //Evolutive mode
                }
                else if((comandos[i]==("-L"))||(comandos[i]==("-l"))){
                    CONF.MODE.push_back(LEARN);
                    //Learning mode
                }
                else if((comandos[i]==("-P"))||(comandos[i]==("-p"))){
                    CONF.MODE.push_back(PREDI);
                    //Prediction mode
                }
                else if((comandos[i]==("-q"))||(comandos[i]==("-Q"))){
                    CONF.MODE.push_back(FOCUS);
                    //Focus on only one stok prediction considering all simple and correlative, it gives error if the stok arg is diferent than one 
                }
                else if((comandos[i]==("-S"))||(comandos[i]==("-s"))){
                    CONF.MODE.push_back(SERVE);
                    FDR_LOCAL=true;//flag for use DAD-local file door
                    FDR_CLOUD=true;//Flag for cloud based file door
                    //Server mode
                }
                else if((comandos[i]==("-C"))||(comandos[i]==("-c"))){
                    CONF.MODE.push_back(CLIEN);
                    FDR_LOCAL=false;//flag for use DAD-local file door
                    FDR_CLOUD=true;//Flag for cloud based file door
                    //Client mode, salve                    
                    
                    if(arG.size()>=(i+1+1)){
                    comandos.push_back(arG[i+1]);
                    FDR_SRVIP = comandos[i+1].c_str();//Sets the IP
                    if(!ISIP(FDR_SRVIP)) FDR_SRVIP="";
                    i++;
                    }
                }
                else if((comandos[i]==("-w"))||(comandos[i]==("-W"))){
                    CONF.MODE.push_back(WATCH);
                    //Watch mode, shows best actual prediction data
                }
                else if((comandos[i]==("-f"))||(comandos[i]==("-F"))){
                    CONF.MODE.push_back(MULTI);
                    comandos.push_back(arG[i+1]);
                    CONF.nPr = atoi(comandos[i+1].c_str());//Sets the number os treads, number after -f command
                    i++;
                //Fork mode, creates multi process structure
                }
                else if((comandos[i]==("-i"))||(comandos[i]==("-I"))){
                    CONF.MODE.push_back(EPOCH);
                    comandos.push_back(arG[i+1]);
                    CONF.epoch = atoi(comandos[i+1].c_str());//Sets the number os treads, number after -f command
                    i++;
                    //modifies the epoch 
                }
                else if((comandos[i]==("-x"))||(comandos[i]==("-X"))){
                    CONF.MODE.push_back(EXPER);
                    //experimental mode
                }
                else if((comandos[i]==("-g"))||(comandos[i]==("-G"))){
                    CONF.MODE.push_back(GLOBA);
                    CONF.NCF="G_DS";//Neural configuration folder
                    CONF.PDBF="G_PRD";//Prediction data base folder
                    //GLOBAAL MODE
                }
                else if((comandos[i]==("-a"))||(comandos[i]==("-A"))){
                    CONF.MODE.push_back(AUTOM);
                    //Automatic mode 
                }
                else if((comandos[i]==(":"))&&(STOKt.size()>=1)){
                    CONF.STOCK.push_back(STOKt);
                    STOKt.clear();
                    //Constructs the STOK vector
                }
                else if((comandos[i]==("-dbg"))||(comandos[i]==("-DBG"))){
                    CONF.MODE.push_back(DEBUG);
                    //DBG MODE
                }
                else{
                    //Stocks names involved, last one TO predict, 1 name trains CR, 2 names trains SR, no names trainis all secuential. secuances of names separater by :
                    STOKt.push_back(comandos[i]);
                }    
            }
            if((STOKt.size()>=1)){
                CONF.STOCK.push_back(STOKt);
                STOKt.clear(); 
            }
        }
        
        void MP(std::string command){//Mode parser
                    std::vector<std::string> arG = split(command, ' ');
                    MP(arG);
        }

        void DBG_MODE(){//Prints the detected mode of operation
            for(int y=0;y<CONF.MODE.size();y++){
                if(CONF.MODE[y]==EVOLU){//Evolutive mode
                    std::cout<<"EM"<<std::endl;
                }
                else if(CONF.MODE[y]==LEARN){//Learning mode
                    std::cout<<"LM"<<std::endl;     
                }   
                else if(CONF.MODE[y]==PREDI){    //Prediction mode
                    std::cout<<"PM"<<std::endl;     
                }
                else if(CONF.MODE[y]==FOCUS){//Focus mode
                    std::cout<<"FM"<<std::endl;     
                }
                else if(CONF.MODE[y]==SERVE){//Server mode
                    std::cout<<"SM "<<std::endl;     
                }
                else if(CONF.MODE[y]==CLIEN){//Client mode
                    std::cout<<"CM"<<FDR_SRVIP<<std::endl;     
                }
                else if(CONF.MODE[y]==WATCH){//Watch mode
                    std::cout<<"WM"<<std::endl;     
                }
                else if(CONF.MODE[y]==MULTI){//Multi process 
                    std::cout<<"MP"<<std::endl;
                }
                else if(CONF.MODE[y]==EPOCH){//Epoch selector
                    std::cout<<"IM"<<std::endl;     
                }   
                else if(CONF.MODE[y]==EXPER){//Experimental mode
                    std::cout<<"EX"<<std::endl;     
                }
                else if(CONF.MODE[y]==GLOBA){//Global mode
                    std::cout<<"GM"<<std::endl;
                }  
                else if(CONF.MODE[y]==AUTOM){//Auto mode
                    std::cout<<"AM"<<std::endl;     
                }
                else if(CONF.MODE[y]==DEBUG){//Debug mode
                    std::cout<<"DBGM"<<std::endl;     
                }
            }
            if(CONF.STOCK.size()>0){
                    //DBG
                    DBG_STD(CONF.STOCK,"unptick:");
                    //DBG
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
            std::cout<<"-q\tFocus on only one Stok, correlative and simple selector"<<std::endl;
            std::cout<<"-i\tIntra-day mode"<<std::endl;
            std::cout<<"STOKpf STOKtp : STOKtp \tStock tiket input"<<std::endl;
    
            CONF.EX=false;
        }
        
        //Mode dependent initialization routines in_ executed at the begining of the execution of the mode.
        
       
    };
    
    class OEX{//Operation executor
        
    public:
        OM OPMAN;
        
        OEX(){
        
        };
        
        void in_stk_sr(FS & fs,OM & om){//Checks existence of stock tickets, if not loads avaliable
            if(om.CONF.STOCK.size()>0){//If specific stock tickets are passed
               
            }
            else{//If non existent stocks load all vnn present on current folder
                fs.NFC.GFN();
                fs.NFC.NCFNP();
                om.CONF.STOCK=fs.NFC.VNCN;
                
            }
            om.CONF.STOCK=ALPSRT(om.CONF.STOCK,om.CONF.OSS);
            //fs.STOCK=om.CONF.STOCK;//Puts on financial sistem the stock tickets to work whith
            DBG_STD(om.CONF.STOCK,"PRES_STK:"); 
            
        }
        
         void in_evolu_sr(FS & fs,OM & om, int o){
            if(om.CONF.MODE[o]==EVOLU){//Evolutive om.MODE
            
            }
        }
        
        void in_learn_sr(FS & fs,OM & om, int o){
            if(om.CONF.MODE[o]==LEARN){//Learning om.MODE
                     
            }
        }
        
        void in_predict_sr(FS & fs,OM & om, int o){
           if(om.CONF.MODE[o]==PREDI){//Prediction om.MODE
                    
            } 
        }
        
        void in_focus_sr(FS & fs,OM & om, int o){
            if(om.CONF.MODE[o]==FOCUS){//Focus om.MODE
                
            }
        }

        void in_server_sr(FS & fs,OM & om, int o){
            if(om.CONF.MODE[o]==SERVE){//Server om.MODE

            }
        }

        void in_client_sr(FS & fs,OM & om, int o){
            if(om.CONF.MODE[o]==CLIEN){//Client_slave
                 
                 
            }
        }

        void in_watch_sr(FS & fs,OM & om, int o){
            if(om.CONF.MODE[o]==WATCH){//Watch
                fs.WTCH();
                om.CONF.EX=false;
            }
        }
        
        void in_multiproc_sr(FS & fs,OM & om, int o){
            if(om.CONF.MODE[o]==MULTI){//Fork
            }
        }
        
        void in_intrad_sr(FS & fs,OM & om, int o){
            if(om.CONF.MODE[o]==EPOCH){//Epoch selector

            }
        }

        void in_experi_sr(FS & fs,OM & om, int o){
            if(om.CONF.MODE[o]==EXPER){//Experimental om.MODE
                om.CONF.NCF="X_DS";//Neural configuration folder
                om.CONF.PDBF="X_PRD";//Prediction data base folderfs.NFC.

            }
        }
        
        void in_globalcorr_sr(FS & fs,OM & om, int o){
            if(om.CONF.MODE[o]==GLOBA&&om.CONF.STOCK.size()==1){//Global database selection
                om.CONF.NCF="G_DS";//Neural configuration folder
                om.CONF.PDBF="G_PRD";//Prediction data base folderfs.NFC.
            }
        }

        void in_autom_sr(FS & fs,OM & om, int o){//Automatic om.MODE, analizes the current state of the file sistem, and predictions to give the next instruction.
            if(om.CONF.MODE[o]==AUTOM){
                
            }
        }
        
        void in_dgb_sr(FS & fs,OM & om, int o){
            if(om.CONF.MODE[o]==DEBUG){//Debug or testing executed the first cicle
                om.CONF.EX=false;
            }
        }

        void GMIR(FS & fs,OM & om){//Global initialization of om.MODE routines
            
            om.DBG_MODE();
            
            in_stk_sr(fs,om);
            for(int y=0;y<om.CONF.MODE.size();y++){//Folder and Stock modifiers
                in_autom_sr(fs,om,y);
                in_globalcorr_sr(fs,om,y);
                in_experi_sr(fs,om,y);
                in_focus_sr(fs,om,y);
                in_intrad_sr(fs,om,y);
            }
            fs.CONF=om.CONF;
            
            for(int y=0;y<om.CONF.MODE.size();y++){//Execution init
                in_evolu_sr(fs,om,y);
                in_learn_sr(fs,om,y);
                in_predict_sr(fs,om,y);
                in_server_sr(fs,om,y);
                in_client_sr(fs,om,y);
                in_multiproc_sr(fs,om,y);
                in_dgb_sr(fs,om,y);
                in_watch_sr(fs,om,y);
                
            }
        }
        //om.MODE dependent initialization routines
        
        //om.MODE dependent loop execution routines
        
        void ex_evolu_sr(FS & fs,OM & om, int o){
            if(om.CONF.MODE[o]==EVOLU){//Evolutive om.MODE
                fs.FSER(true);//Loads to memory the stock data passed
            }
        }
        
        void ex_learn_sr(FS & fs,OM & om, int o){
            if(om.CONF.MODE[o]==LEARN){//Learning om.MODE
                fs.FSER(false);//Loads to memory the stock data passed
            }
        }
        
        void ex_predict_sr(FS & fs,OM & om, int o){
            if(om.CONF.MODE[o]==PREDI){//Prediction om.MODE
                fs.FSPR();
            } 
        }
        
        void ex_focus_sr(FS & fs,OM & om, int o){
            if(om.CONF.MODE[o]==FOCUS){//Focus om.MODE
            
            }
        }

        void ex_server_sr(FS & fs,OM & om, int o){
            if(om.CONF.MODE[o]==SERVE){//Server om.MODE
                     
            }
        }

        void ex_client_sr(FS & fs,OM & om, int o){
            if(om.CONF.MODE[o]==CLIEN){//Client_slave
                      
            }
        }
        
        void ex_watch_sr(FS & fs,OM & om, int o){
            if(om.CONF.MODE[o]==WATCH){//Watch

            }
        }

        void ex_multiproc_sr(FS & fs,OM & om, int o){
            if(om.CONF.MODE[o]==MULTI){//Fork
               // std::cout<<"FM_EX"<<std::endl;
                fs.FMPE.CSPS(om.CONF.nPr);
                //std::cout<<"FM_EC_CR"<<std::endl;
                if(fs.FMPE.PRN()!=-1){//If the current proces is a child
                    om.CONF.EX=false;//Prevent the children to create more childrens
                }
            std::cout<<"Multi thread setup correct"<<std::endl;
            }
        }

        void ex_intrad_sr(FS & fs,OM & om, int o){
            if(om.CONF.MODE[o]==EPOCH){//Intra day om.MODE

            }
        }
        
        void ex_experi_sr(FS & fs,OM & om, int o){
            if(om.CONF.MODE[o]==EXPER){//Experimental om.MODE

            }
        }
        
        void ex_globalcorr_sr(FS & fs,OM & om, int o){
            if(om.CONF.MODE[o]==GLOBA&&om.CONF.STOCK.size()==1){//Global correlation database selection

            }
        }
        
        void ex_autom_sr(FS & fs,OM & om, int o){//Automatic om.MODE, analizes the current state of the file sistem, and predictions to give the next instruction.
            if(om.CONF.MODE[o]==AUTOM){
                
            }
        }

        void ex_dgb_sr(FS & fs,OM & om, int o){
            if(om.CONF.MODE[o]==DEBUG){//Debug or testing executed the first cicle
    
            }
        }

        void ex_stk_sr(FS & fs,OM & om, int o){//Concrete tickets specified
            if(om.CONF.STOCK.size()>0){
                
            }
        }

        void GMER(FS & fs,OM & om){//Global om.MODE execution routines
            
            for(int y=0;y<om.CONF.MODE.size();y++){
                ex_evolu_sr(fs,om,y);
                ex_learn_sr(fs,om,y);
                ex_predict_sr(fs,om,y);
                ex_server_sr(fs,om,y);
                ex_client_sr(fs,om,y);
                //std::cout<<"EX_STACK_1_EXECUTED"<<std::endl; 
                ex_globalcorr_sr(fs,om,y);
                ex_experi_sr(fs,om,y);
        
                //std::cout<<"EX_STACK_2_EXECUTED"<<std::endl; 
                ex_focus_sr(fs,om,y);
                ex_intrad_sr(fs,om,y);

                ex_dgb_sr(fs,om,y);
                //std::cout<<"EX_STACK_3_EXECUTED"<<std::endl; 
                ex_watch_sr(fs,om,y);
                ex_autom_sr(fs,om,y);
                //std::cout<<"EX_STACK_4_EXECUTED"<<std::endl; 
                ex_multiproc_sr(fs,om,y);
                //std::cout<<"EX_STACK_4.1_EXECUTED"<<std::endl; 
                ex_stk_sr(fs,om,y);
                //std::cout<<"EX_STACK_4.2_EXECUTED"<<std::endl; 
            }
        }
    };
    
//Base software routines

//Graphic routines

//Graphic routines
#endif
