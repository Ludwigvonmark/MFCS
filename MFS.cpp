#include "../LIB/GR/L64/AXL64.h"
#include "../LIB/GR/GR.h"

#include <math.h>
#include <algorithm> 
#include <unistd.h>
#include "vector"

#include <cstdio>
#include <clocale>
#include <fstream>
#include <iostream>
#include <string>
#include <iostream>
#include <experimental/filesystem>

#include "../LIB/LINUX/STGU.h"
#include "../LIB/LINUX/DPR.h"
#include "../LIB/CSV.hpp"
#include "../LIB/NN/NN.hpp"
#include "DBM.hpp"


//#include "TESTFEATURES.h"


int main(int argc, char* argv[]) {
   
	
	ENV ENV1;//Entorno 1
	CAM CAM1;//Camara orbital 1
	
	CRN CRONO;
        double elEv_1=0;//Cuenta el tiempo desde la ultima ejecución de la rutina 1
        const double ev_1=1000000;//Tiempo en segundos para ejecutar la rutina 1
	
        //Graphic resources initializer.
	TTF_Font *HelvRom_30 = TTF_OpenFont("../LIB/GR/RESOURCES/FONTS/HelveticaLTStd-Roman.otf", 30);
        TTF_Font *HelvRom_90 = TTF_OpenFont("../LIB/GR/RESOURCES/FONTS/HelveticaLTStd-Roman.otf", 90);
        
        const SDL_Color MFSgreen = { 60,220,100 };
        const SDL_Color MFSred = { 9,203,177 };
        const SDL_Color MFSyelow = { 3,137,216 };
        const SDL_Color MFSblue = { 176,202,3 };
	const SDL_Color MFSwhite = { 230,230,230 };
	const SDL_Color MFSgrey = { 120,120,120 };
	const SDL_Color MFSblack = { 5,5,5 };
        //Graphic resources initializer.
	
	CSV PRDB(5);
        std::vector<std::string> TO;
        std::vector<std::string> FROM;
        
        namespace fs = std::experimental::filesystem;
        int nF=0;
        std::string CFILE;
        std::vector<std::string> CCF;
        //std::string TO="TSLA";
        //std::string FROM="TSLA";
        std::string path = "C_PRD";
        for (auto & p : fs::directory_iterator(path)){//itera por todos los archivos en la carpeta de las configuraciones neuronales disponibles
            CFILE.clear();
        CFILE= p.path();
        CCF.push_back(CFILE);

           /* //std::cout << CFILE << std::endl;
            
            //File name parser
            std::size_t frs = CFILE.find(path);            
            CFILE= CFILE.substr (frs+path.size()+1+4); 

            //std::cout << CFILE << std::endl;            
            
            std::size_t scn = CFILE.find("_");
            std::size_t trd = CFILE.find(".");
            
            FROM.push_back( CFILE.substr (0,scn)); 
            
            TO.push_back(CFILE.substr(scn+1,trd-scn-1));
*/
        }
            //File name parser
        
        //Elementos gráficos
                GRAPH GR_RD;
                GRAPH GR;
                GRAPH R_STK;
                GRAPH P_STK;
                GRAPH PO_STK;
                
                TXT P_STK_N;
                
                TXT L_STK_RP;
                float stkCRP;
                TXT L_STK_PP;
                float stkCPP;
                TXT PP;
                float ppp;
                                
                TXT STAT;
                
                TXT EARNN;
                float EARN;                
                TXT PW;
                float PrW;
        
                TXT TIME_1;


                BTN PLUS;
                
                int BEL=0;//Bottom element positión sumatory 
        //Elementos gráficos
        
        //Update graphs
        //PRDB.LCSV("F_PRD/PRD_"+FROM[nF]+"_"+TO[nF]+".csv");
        PRDB.LCSV(CCF[nF]);
        
        stkCRP=PRDB.DATA[PRDB.DATA.size()-1][1];
        stkCPP=PRDB.DATA[PRDB.DATA.size()-1][2]+stkCRP;
        
        ppp=((stkCPP-stkCRP)/stkCPP)*100;

        R_STK.color[0]=0.4; R_STK.color[1]=0.4; R_STK.color[2]=0.4f;
        for(int i=0;i<PRDB.DATA.size();i+=1){
        R_STK.X.push_back(i);
        R_STK.Y.push_back(PRDB.DATA[i][1]);}
        R_STK.NDx();
        R_STK.NDy();
        
        P_STK.color[0]=0.011; P_STK.color[1]=0.793; P_STK.color[2]=0.6914f;
        for(int i=1;i<PRDB.DATA.size();i+=1){
        P_STK.X.push_back(i);
        P_STK.Y.push_back(PRDB.DATA[i-1][2]+PRDB.DATA[i][1]);}
        P_STK.NDx();        
        P_STK.NDy();      
        
        PO_STK.color[0]=0.851; PO_STK.color[1]=0.541; PO_STK.color[2]=0.1f;
        double ccpop=PRDB.DATA[0][1];
        for(int i=1;i<PRDB.DATA.size();i+=1){
        PO_STK.X.push_back(i);
        ccpop+=PRDB.DATA[i-1][2];
        PO_STK.Y.push_back(ccpop);}
        PO_STK.NDx();
        PO_STK.NDy();    
        
        //Group normalization
        double MXRGx= std::max(R_STK.RGx,P_STK.RGx); MXRGx=std::max(MXRGx,PO_STK.RGx);
        R_STK.RGx=MXRGx; P_STK.RGx=MXRGx; PO_STK.RGx=MXRGx;
        
        double MXRGy= std::max(R_STK.RGy,P_STK.RGy); MXRGy=std::max(MXRGy,PO_STK.RGy);
        R_STK.RGy=MXRGy; P_STK.RGy=MXRGy; PO_STK.RGy=MXRGy;
        
        double MXx= std::max(R_STK.MXx,P_STK.MXx); MXx=std::max(MXx,PO_STK.MXx);
        R_STK.MXx=MXx; P_STK.MXx=MXx; PO_STK.MXx=MXx;
        
        double MXy= std::max(R_STK.MXy,P_STK.MXy); MXy=std::max(MXy,PO_STK.MXy);
        R_STK.MXy=MXy; P_STK.MXy=MXy; PO_STK.MXy=MXy;
        
        double MNx= std::min(R_STK.MNx,P_STK.MNx); MNx=std::min(MNx,PO_STK.MNx);
        R_STK.MNx=MNx; P_STK.MNx=MNx; PO_STK.MNx=MNx;
        
        double MNy= std::min(R_STK.MNy,P_STK.MNy); MNy=std::min(MNy,PO_STK.MNy);
        R_STK.MNy=MNy; P_STK.MNy=MNy; PO_STK.MNy=MNy;
        
        double ZERx=-MNx/MXRGx;
        R_STK.ZxP=ZERx; P_STK.ZxP=ZERx; PO_STK.ZxP=ZERx;
        
        double ZERy=-MNy/MXRGy;
        R_STK.ZyP=ZERy; P_STK.ZyP=ZERy; PO_STK.ZyP=ZERy;
        
        R_STK.NGx(); P_STK.NGx(); PO_STK.NGx();
        R_STK.NGy(); P_STK.NGy(); PO_STK.NGy();
        //Group normalization
        

        GR.color[0]=0.011; GR.color[1]=0.793; GR.color[2]=0.6914f;
        for(int i=0;i<PRDB.DATA.size();i+=1){
        GR.X.push_back(i+1);
        GR.Y.push_back(PRDB.DATA[i][2]/**10*/);}
        GR.NDx();
        GR.NDy();        
        
        GR_RD.color[0]=0.4; GR_RD.color[1]=0.4; GR_RD.color[2]=0.4f;
        for(int i=1;i<PRDB.DATA.size();i+=1){
        GR_RD.X.push_back(i);
        GR_RD.Y.push_back(PRDB.DATA[i][1]-PRDB.DATA[i-1][1]);}
        GR_RD.NDx();
        GR_RD.NDy();
        
                //Group normalization
        MXRGx= std::max(GR.RGx,GR_RD.RGx);
        GR.RGx=MXRGx; GR_RD.RGx=MXRGx;
        
        MXRGy= std::max(GR.RGy,GR_RD.RGy);
        GR.RGy=MXRGy; GR_RD.RGy=MXRGy;
        
        MXx= std::max(GR.MXx,GR_RD.MXx);
        GR.MXx=MXx; GR_RD.MXx=MXx;
        
        MXy= std::max(GR.MXy,GR_RD.MXy);
        GR.MXy=MXy; GR_RD.MXy=MXy;
        
        MNx= std::min(GR.MNx,GR_RD.MNx);
        GR.MNx=MNx; GR_RD.MNx=MNx;
        
        MNy= std::min(GR.MNy,GR_RD.MNy);
        GR.MNy=MNy; GR_RD.MNy=MNy;
        
        ZERx=-MNx/MXRGx;
        GR.ZxP=ZERx; GR_RD.ZxP=ZERx;
        
        ZERy=-MNy/MXRGy;
        GR.ZyP=ZERy; GR_RD.ZyP=ZERy;
        
        GR.NGx(); GR_RD.NGx(); 
        GR.NGy(); GR_RD.NGy();
        //Group normalization
        //Update graphs
        
        //Statistics calculation
        EARN=0;
        for(int i=1;i<PRDB.DATA.size();i+=1){
            if(PRDB.DATA[i-1][2]>0){
                EARN+=PRDB.DATA[i][1]-PRDB.DATA[i-1][1];
            }
        }
        
        PrW=0;
        for(int i=1;i<PRDB.DATA.size();i+=1){
            if(((PRDB.DATA[i-1][2]>0)&&(PRDB.DATA[i][1]-PRDB.DATA[i-1][1]>0))
                ||((PRDB.DATA[i-1][2]<0)&&(PRDB.DATA[i][1]-PRDB.DATA[i-1][1]<0))){
                PrW+=1;
            }
        }
        PrW=PrW*100/PRDB.DATA.size();
        
        
        
        //Statistics calculation
        
        CRONO.STRT();


	while (ENV1.EXECUT()) {
		ENV1.EVE();//Eventos del entorno
                
                CRONO.STP();
                elEv_1+=CRONO.LPS();
                CRONO.STRT();
                
                if((elEv_1>=ev_1)||PLUS.CLICKL){//Rutina temporal 1
                    nF+=1;
                    if(nF>=CCF.size()){
                        nF=0;
                        CCF.clear();
                       /* FROM.clear();
                        TO.clear();*/
                        for (auto & p : fs::directory_iterator(path)){//itera por todos los archivos en la carpeta de las configuraciones neuronales disponibles
       
                            CFILE.clear();
                            CFILE= p.path();
                            CCF.push_back(CFILE);

            //std::cout << CFILE << std::endl;
            
            /*//File name parser
            std::size_t frs = CFILE.find(path);            
            CFILE= CFILE.substr (frs+path.size()+1+4); 

            //std::cout << CFILE << std::endl;            
            
            std::size_t scn = CFILE.find("_");
            std::size_t trd = CFILE.find(".");
            
            FROM.push_back( CFILE.substr (0,scn)); 
            
            TO.push_back(CFILE.substr(scn+1,trd-scn-1));*/

        }
            //File name parser
                        
                    }
                    elEv_1=0;
                    //Update graphs
                    //PRDB.LCSV("F_PRD/PRD_"+FROM[nF]+"_"+TO[nF]+".csv");
                    PRDB.LCSV(CCF[nF]);
        
                    stkCRP=PRDB.DATA[PRDB.DATA.size()-1][1];
                    stkCPP=PRDB.DATA[PRDB.DATA.size()-1][2]+stkCRP;
        
                    ppp=((stkCPP-stkCRP)/stkCPP)*100;
                    
                    R_STK.X.clear();
                    R_STK.Y.clear();
                    R_STK.color[0]=0.4; R_STK.color[1]=0.4; R_STK.color[2]=0.4f;
                    for(int i=0;i<PRDB.DATA.size();i+=1){
                    R_STK.X.push_back(i);
                    R_STK.Y.push_back(PRDB.DATA[i][1]);}                    
                    R_STK.NDx();
                    R_STK.NDy();
                    
                    P_STK.X.clear();
                    P_STK.Y.clear();        
                    P_STK.color[0]=0.011; P_STK.color[1]=0.793; P_STK.color[2]=0.6914f;
                    for(int i=1;i<PRDB.DATA.size();i+=1){
                    P_STK.X.push_back(i);
                    P_STK.Y.push_back(PRDB.DATA[i-1][2]+PRDB.DATA[i][1]);}
                    P_STK.NDx();
                    P_STK.NDy();
        
                    PO_STK.X.clear();
                    PO_STK.Y.clear();        
                    PO_STK.color[0]=0.851; PO_STK.color[1]=0.541; PO_STK.color[2]=0.1f;
                    double ccpop=PRDB.DATA[0][1];
                    for(int i=1;i<PRDB.DATA.size();i+=1){
                    PO_STK.X.push_back(i);
                    ccpop+=PRDB.DATA[i-1][2];
                    PO_STK.Y.push_back(ccpop);}
                    PO_STK.NDx();
                    PO_STK.NDy();
                    
                            //Group normalization
        MXRGx= std::max(R_STK.RGx,P_STK.RGx); MXRGx=std::max(MXRGx,PO_STK.RGx);
        R_STK.RGx=MXRGx; P_STK.RGx=MXRGx; PO_STK.RGx=MXRGx;
        
        MXRGy= std::max(R_STK.RGy,P_STK.RGy); MXRGy=std::max(MXRGy,PO_STK.RGy);
        R_STK.RGy=MXRGy; P_STK.RGy=MXRGy; PO_STK.RGy=MXRGy;
        
        MXx= std::max(R_STK.MXx,P_STK.MXx); MXx=std::max(MXx,PO_STK.MXx);
        R_STK.MXx=MXx; P_STK.MXx=MXx; PO_STK.MXx=MXx;
        
        MXy= std::max(R_STK.MXy,P_STK.MXy); MXy=std::max(MXy,PO_STK.MXy);
        R_STK.MXy=MXy; P_STK.MXy=MXy; PO_STK.MXy=MXy;
        
        MNx= std::min(R_STK.MNx,P_STK.MNx); MNx=std::min(MNx,PO_STK.MNx);
        R_STK.MNx=MNx; P_STK.MNx=MNx; PO_STK.MNx=MNx;
        
        MNy= std::min(R_STK.MNy,P_STK.MNy); MNy=std::min(MNy,PO_STK.MNy);
        R_STK.MNy=MNy; P_STK.MNy=MNy; PO_STK.MNy=MNy;
        
        ZERx=-MNx/MXRGx;
        R_STK.ZxP=ZERx; P_STK.ZxP=ZERx; PO_STK.ZxP=ZERx;
        
        ZERy=-MNy/MXRGy;
        R_STK.ZyP=ZERy; P_STK.ZyP=ZERy; PO_STK.ZyP=ZERy;
        
        R_STK.NGx(); P_STK.NGx(); PO_STK.NGx();
        R_STK.NGy(); P_STK.NGy(); PO_STK.NGy();
        //Group normalization
        
                    GR.X.clear();
                    GR.Y.clear();

                    GR.color[0]=0.011; GR.color[1]=0.793; GR.color[2]=0.6914f;
                    for(int i=0;i<PRDB.DATA.size();i+=1){
                    GR.X.push_back(i+1);
                    GR.Y.push_back(PRDB.DATA[i][2]/**10*/);}
                    GR.NDx();
                    GR.NDy();
        
                    GR_RD.X.clear();
                    GR_RD.Y.clear();        
        
                    GR_RD.color[0]=0.4; GR_RD.color[1]=0.4; GR_RD.color[2]=0.4f;
                    for(int i=1;i<PRDB.DATA.size();i+=1){
                    GR_RD.X.push_back(i);
                    GR_RD.Y.push_back(PRDB.DATA[i][1]-PRDB.DATA[i-1][1]);}
                    GR_RD.NDx();
                    GR_RD.NDy();
                    
                                    //Group normalization
        MXRGx= std::max(GR.RGx,GR_RD.RGx);
        GR.RGx=MXRGx; GR_RD.RGx=MXRGx;
        
        MXRGy= std::max(GR.RGy,GR_RD.RGy);
        GR.RGy=MXRGy; GR_RD.RGy=MXRGy;
        
        MXx= std::max(GR.MXx,GR_RD.MXx);
        GR.MXx=MXx; GR_RD.MXx=MXx;
        
        MXy= std::max(GR.MXy,GR_RD.MXy);
        GR.MXy=MXy; GR_RD.MXy=MXy;
        
        MNx= std::min(GR.MNx,GR_RD.MNx);
        GR.MNx=MNx; GR_RD.MNx=MNx;
        
        MNy= std::min(GR.MNy,GR_RD.MNy);
        GR.MNy=MNy; GR_RD.MNy=MNy;
        
        ZERx=-MNx/MXRGx;
        GR.ZxP=ZERx; GR_RD.ZxP=ZERx;
        
        ZERy=-MNy/MXRGy;
        GR.ZyP=ZERy; GR_RD.ZyP=ZERy;
        
        GR.NGx(); GR_RD.NGx(); 
        GR.NGy(); GR_RD.NGy();
        //Group normalization
        
                //Statistics calculation
        EARN=0;
        for(int i=1;i<PRDB.DATA.size();i+=1){
            if(PRDB.DATA[i-1][2]>0){
                EARN+=PRDB.DATA[i][1]-PRDB.DATA[i-1][1];
            }
        }
        
        PrW=0;
        for(int i=1;i<PRDB.DATA.size();i+=1){
            if(((PRDB.DATA[i-1][2]>0)&&(PRDB.DATA[i][1]-PRDB.DATA[i-1][1]>0))
                ||((PRDB.DATA[i-1][2]<0)&&(PRDB.DATA[i][1]-PRDB.DATA[i-1][1]<0))){
                PrW+=1;
            }
        }
        PrW=PrW*100/PRDB.DATA.size();
        
        
        
        //Statistics calculation
        
        //Update graphs
                    
                }//Rutina temporal 1
                
                
            if (ENV1.UPD_3D()) {//Actualizaíón de gráficos del entorno 
		
                        CAM1.U(ENV1.moscy, ENV1.mosdx, ENV1.mosdy, ENV1.mosx, ENV1.mosy, ENV1.rclick, ENV1.mclick);
                       
                        
                        
            }
            
            if (ENV1.DRW_3D()) {ENV1.R_DRW_3D();//Dibujado de gráficos del entorno 
		
		CAM1.D();
            }
                
                
                
                
		
            if (ENV1.UPD_I()) {//Actualización interfaz del entorno 
               
                
                PLUS.U(ENV1.moscx,ENV1.moscy, ENV1.mosdx,ENV1.mosdy, ENV1.mosx, ENV1.mosy, ENV1.rclick, ENV1.lclick);
                
                R_STK.CG({ENV1.mosx, ENV1.mosy, ENV1.mosdx, ENV1.mosdy, ENV1.moscx, ENV1.moscy},{ENV1.rclick,ENV1.lclick,ENV1.mclick,ENV1.released});
                GR_RD.CG({ENV1.mosx, ENV1.mosy, ENV1.mosdx, ENV1.mosdy, ENV1.moscx, ENV1.moscy},{ENV1.rclick,ENV1.lclick,ENV1.mclick,ENV1.released});
                
                //DBST.clear();
                //DBST+=std::to_string(PLUS.nLclicks);
                
			
	
            }
            
            if (ENV1.DRW_I()) {ENV1.R_DRW_I();//Dibujado de interfaz del entorno 
		

		//IMGEN.DMATRIX();
                glTranslatef(10, 0, 0); 
                //P_STK_N.D(HelvRom_90,MFSgrey,TO[nF]);
                
                //BEL=P_STK_N.bon[0]+10;
                BEL=0+10;
                
                glTranslatef(BEL, 10, 0);                 
                L_STK_RP.D(HelvRom_30,MFSwhite,std::to_string(stkCRP));
                glTranslatef(0, 15, 0);
                L_STK_PP.D(HelvRom_30,MFSblue,std::to_string(stkCPP));
                
                BEL+=L_STK_RP.bon[0]+20;
                
                glLoadIdentity();
                glTranslatef(BEL, 0, 0);
                if(ppp>0)
                PP.D(HelvRom_90,MFSblue,std::to_string(ppp)+"%");
                else
                PP.D(HelvRom_90,MFSyelow,std::to_string(ppp)+"%");
                
                BEL+=PP.bon[0]+15;
                
                glLoadIdentity();
                glTranslatef(BEL, 52, 0);
                STAT.D(HelvRom_30,MFSgrey,"Statistics");
                
                glTranslatef(0, -23, 0);
                EARNN.D(HelvRom_30,MFSwhite,"Er "+std::to_string(EARN)+"$");
                glTranslatef(0, -19, 0);
                PW.D(HelvRom_30,MFSwhite,"PW "+std::to_string(PrW)+"%");
                
		//TEXTO.D(HelvRom_30,AXwhite,TEXT);
                glLoadIdentity();
                glTranslatef(5,ENV1.SCREEN_HEIGHT-20, 0);
                TIME_1.D(HelvRom_30,MFSgrey,std::to_string(elEv_1));
                
                
               // glTranslatef(0, 40, 0);               
               // glColor3f(0.2,0.2,0.2);
               // DRect(ENV1.SCREEN_WIDTH, 120);
                
               // DBT.D(HelvRom_30,AXwhite,DBST);
                
              //  glPushMatrix();
             //   glTranslatef(0,-3, 0); 
                
             //   GRC.W=ENV1.SCREEN_WIDTH;
                
                       // 
             //   for (int y=0;y<10;y++){
                    
                   // for(int x=0;x<100;x++){
                        
                 //        glTranslatef(0,10, 0); 
                        
                //        GRC.D(HelvRom_30,AXblack,GRCT);
                   // }
                    
                //}
                
                
               // glPopMatrix();
                glLoadIdentity();
                glTranslatef(0, 80, 0);
                glScalef(ENV1.SCREEN_WIDTH,120,1);
                GR_RD.UB();
                GR_RD.D();
                GR.D();
                
                glLoadIdentity();
                glTranslatef(0, (GR.ZyP*120)+80, 0);
                glColor3f( 0.1f,0.1f,0.1f);
                glLineWidth(1);            
		glBegin(GL_LINE_STRIP);
                glVertex3f(0,0,0);
                glVertex3f(ENV1.SCREEN_WIDTH,0,0);
		glEnd();  
                

                glLoadIdentity();
                glTranslatef(0, 200, 0);
                glScalef(ENV1.SCREEN_WIDTH,120,1);
                
    
                R_STK.UB();
                R_STK.D();
                
                PO_STK.D();
                
                P_STK.D();
                 
                 
                 //Interfaz interactiva
                glLoadIdentity();
                glTranslatef(ENV1.SCREEN_WIDTH-40, 5, 0);
                 PLUS.D(HelvRom_90,MFSwhite,">");
                /* glLoadIdentity();
                 glTranslatef(PLUS.rDrag[0],PLUS.rDrag[1], 0);
                 
                 if(PLUS.over){ glColor3f(0.8,0.5,0.2);
                DRect(100,100);}
                
                       if(PLUS.clickl){ glColor3f(0.5,0.8,0.2);
                DRect(80,80);}
                
                 if(PLUS.CLICKL){ glColor3f(0.5,0.5,0.2);
                DRect(50,50);}
                
                 if((PLUS.nLclicks%2)!=0){ glColor3f(0.7,0.3,0.5);
                DRect(30,30);}
                
                if(PLUS.drag){ glColor3f(0.9,0.3,0.9);
                DRect(PLUS.rDrag[0],PLUS.rDrag[1]);}
                
                glColor4f(0.2,0.5,0.8,0.5);
                DRect(PLUS.bon[1]-PLUS.bon[0],PLUS.bon[3]-PLUS.bon[2]);
                //DRect(25,25);
                
                 */


            }

                CRONO.STP();
                elEv_1+=CRONO.LPS();
                CRONO.STRT();

		ENV1.SWAP();
	}

	return 0;}
