#ifndef CMD_HPP_INCLUDED
#define CMD_HPP_INCLUDED

#include <limits>
//Compositor modificator functon set
//All this functions work whit 2D vectors, whith dates on epoch type dates on the 2DV[i][0] position.

std::vector<double> FW0(int size, double date ){//Fill with ceros, return a data set with a given date on the first position and ceroes on the rest of the vector size, used to fill var vectors whithout older data. size include date.
    std::vector<double>res;
    if (size>1){//Size needs date and at least one more double data on the vector
        res.push_back(date);
        for(int i=1;i<size;i++){
            res.push_back(0);
        }
    }
    else{
        //function error
        res.push_back(date);
    }
    return res;
}

std::vector<std::vector<double>> FW0(std::vector<std::vector<double>> sta){
    std::vector<std::vector<double>> var;
    if (sta.size()>0){//Size needs date and at least one more double data on the vector
        for(int i=0;i<sta.size();i++){
            std::vector<double> vr;
            vr.push_back(sta[i][0]);
            for(int c=1;c<sta[i].size();c++){
            vr.push_back(0);
            }
            var.push_back(vr);
        }

    }
    else{
        //function error
    }
    return var;
}

void DELOLD( int datE,std::vector<std::vector<double>> &var ){//Deletes the older dates to the given one
    if (var.size()>0){//Size needs date and at least one more double data on the vector
        std::vector<int> olderids;
        for(int i=0;i<var.size();i++){
            if((int)var[i][0]<datE){
                int is=olderids.size();
                olderids.push_back(i-is);
            }
        }
        for(int i=0;i<olderids.size();i++){
            var.erase (var.begin()+olderids[i]);
        }
    }
    else{
        //function error
    }
}

void DELNEW( int datE,std::vector<std::vector<double>> &var ){//Deletes the newer dates to the given one
    if (var.size()>0){//Size needs date and at least one more double data on the vector
        std::vector<int> newerids;
        for(int i=0;i<var.size();i++){
            if((int)var[i][0]>datE){
                int is=newerids.size();
                newerids.push_back(i-is);
            }
        }
        for(int i=0;i<newerids.size();i++){
            var.erase (var.begin()+newerids[i]);
        }
    }
    else{
        //function error
    }
}

int OSTID(std::vector<std::vector<double>> sta){//Returns the iddd of the oldest date avaliable, -1 if vector bad defined
    int iddd;
     if(sta.size()>0&&sta[0].size()>0){
         int min=sta[0][0];
         iddd=0;
         for(int i=1;i<sta.size();i++){
             if((min>sta[i][0])&&(sta[i][0]>0)){
                 min=sta[i][0];
                 iddd=i;
            }
        }
    }
    else{
        iddd=-1;
    }
    return iddd;
}

int NSTID(std::vector<std::vector<double>> sta){//Returns the iddd of the newest date avaliable, -1 if vector bad defined
    int iddd;
     if(sta.size()>0&&sta[0].size()>0){
         int max=sta[0][0];
         iddd=0;
         for(int i=1;i<sta.size();i++){
             if(max<sta[i][0]){
                 max=sta[i][0];
                 iddd=i;
            }
        }
    }
    else{
        iddd=-1;
    }
    return iddd;
}

bool ascO(std::vector<std::vector<double>> sta){//Return true if the date order is ascendent older first
    bool res;
    for(int o=1;o<sta.size();o++){
        if(sta[o-1][0]<sta[o][0]){
            res=true;
        }
        else{
            res=false;
            break;
        }
    }
    
    return res;
    
}

bool RascO(std::vector<std::vector<double>> sta){//Return true if the predominant date order is ascendent older first
    bool res;
    int asc=0;
    int des=0;
    for(int o=1;o<sta.size();o++){
        if(sta[o-1][0]<sta[o][0]){
            asc++;
        }
        else{
            des++;
        }
    }
    
    if(asc>des) return true;
    else return false;
}

bool desO(std::vector<std::vector<double>> sta){//Return true if the date order is descendent newer first
        bool res;
    for(int o=1;o<sta.size();o++){
        if(sta[o-1][0]>sta[o][0]){
            res=true;
        }
        else{
            res=false;
            break;
        }
    }
    
    return res;
}

bool RdesO(std::vector<std::vector<double>> sta){//Return true if the date order is descendent newer first
    return !RascO(sta);
}


std::vector<std::vector<double>> Oasc(std::vector<std::vector<double>> var){//Orders the financial data in ascendent order older first
    std::vector<std::vector<double>> oASC=var;
    
    std::sort(oASC.begin(), oASC.end(), [](const std::vector< double >& a, const std::vector< double >& b){ return a[0] < b[0]; } ); //If you want to sort in ascending order, then substitute > with <.
    
    return oASC;

}

std::vector<std::vector<double>> Odes(std::vector<std::vector<double>> var){//Orders the financial data in descendent order
    std::vector<std::vector<double>> oDES=var;
    
    std::sort(oDES.begin(), oDES.end(), [](const std::vector< double >& a, const std::vector< double >& b){ return a[0] > b[0]; } ); //If you want to sort in ascending order, then substitute > with <.
    
    return oDES;
}

int ODID(int stdID,std::vector<std::vector<double>> sta, std::vector<std::vector<double>> var){//Older date iddd. Returns the iddd of the var vector that contains previos date to the given one by sta vector date. if a older date isn't present it returns -1, if the dates are ecual it returns the iddd where ecual.-2 is when given vectors whith wrong dimension
    int iddd=-2;
    int mindif=std::numeric_limits<int>::max();//older data gets values of -n
    if(sta.size()>0&&var.size()>0&&sta[0].size()>0&&var[0].size()>0){
        iddd=0;
        
    
        for(int p=0;p<var.size();p++){
            if(sta[stdID][0]==var[p][0]){
                iddd=p;
                break;
            }   
            else if((mindif>=sta[stdID][0]-var[p][0])&&(sta[stdID][0]-var[p][0]<0)){
                iddd=p;
                mindif=sta[stdID][0]-var[p][0];
                
            }
            
        }
    }
    else{
        //Error
        std::cout<<"CMD_HPP_ODID_VECTOR_ERROR"<<std::endl;
    }
    if(mindif>0){//if mindif is greater than 0 no older date has been found;
        iddd=-1;
        std::cout<<"CMD_HPP_ODID_NEWER_ERROR"<<std::endl;
    }
    return iddd;
}

int ODID(long datE, std::vector<std::vector<double>> var){//Older date iddd. Returns the iddd of the var vector that contains previos date to the given one by sta vector date. if a older date isn't present it returns -1, if the dates are ecual it returns the iddd where ecual.-2 is when given vectors whith wrong dimension
    int iddd=-2;
    int mindif=std::numeric_limits<int>::min();//older data gets values of -n
    if(var.size()>0&&var[0].size()>0){
        for(int p=0;p<var.size();p++){
            if(datE==(int)var[p][0]){
                iddd=p;
                return iddd;
            }   
            else if(((int)var[p][0]-datE)<0 && mindif<=((int)var[p][0]-datE)){
                iddd=p;
                mindif=((int)var[p][0]-datE);
            }
            
        }
    }
    else{
        //Error
    }
    if(mindif>0){//if mindif is greater than 0 no older date has been found;
        iddd=-1;
    }
    return iddd;
}

long ODATE(long datE,std::vector<std::vector<double>> var){//Return the date contain on the NDID idddentificator
    int iddd=0;
    iddd=ODID(datE,var);

    if(iddd==-2 || iddd==-1){
        //ERROR
        std::cout<<"CMD_HPP_ODATE_ERROR"<<std::endl;
        //exit(-1);
        return -1;
    }
    else{
        return var[iddd][0];
    }
}

int NDID(int stdID,std::vector<std::vector<double>> sta, std::vector<std::vector<double>> var){//Newer date iddd. Returns the iddd of the var vector that contains previos date to the given one by sta vector date. if a older date isn't present it returns -1, if the dates are ecual it returns the iddd where ecual.-2 is when given vectors whith wrong dimension
    int iddd=-2;
    int maxdif=std::numeric_limits<int>::min();//older data gets values of -n
    if(sta.size()>0&&var.size()>0&&sta[0].size()>0&&var[0].size()>0){
        iddd=0;
        
    
        for(int p=0;p<var.size();p++){
            if(sta[stdID][0]==var[p][0]){
                iddd=p;
                break;
            }   
            else if((maxdif<=sta[stdID][0]-var[p][0])&&(sta[stdID][0]-var[p][0]<0)){
                iddd=p;
                maxdif=sta[stdID][0]-var[p][0];
            }
        }
    }
    else{
        //Error
    }
    if(maxdif<0){//if mindif is greater than 0 no older date has been found;
        iddd=-1;
    }
    return iddd;
}

int NDID(long datE,std::vector<std::vector<double>> var){//Newer date iddd. Returns the iddd of the var vector that contains previos date to the given one by sta vector date. if a older date isn't present it returns -1, if the dates are ecual it returns the iddd where ecual.-2 is when given vectors whith wrong dimension
    int iddd=-2;
    
    //std::cout<<" var.size: "<<var.size()<<" var[0].size: "<<var[0].size()<<std::endl;
    
    long mindif=std::numeric_limits<long>::max();//older data gets values of -n
    if(var.size()>0&&var[0].size()>0){
        for(int p=0;p<var.size();p++){
            if(datE==(long)var[p][0]){
                iddd=p;
                return iddd;
            }   
            else if(((long)var[p][0]-datE)>0 && mindif>=((long)var[p][0]-datE)){
                iddd=p;
                mindif=((long)var[p][0]-datE);
            }

        }
    }
    else{
        //Error
        std::cout<<"CMD_HPP_NDID_VECTOR_ERROR"<<std::endl;
    }
    return iddd;
}

long NDATE(long datE,std::vector<std::vector<double>> var){//Return the date contain on the NDID idddentificator
    int iddd=0;
    iddd=NDID(datE,var);

    if(iddd==-2 || iddd==-1){
        //ERROR
        std::cout<<"CMD_HPP_NDATE_ERROR"<<std::endl;
        //exit(-1);
        return -1;
    }
    else{
        return var[iddd][0];
    }
}

    std::vector<CSV> EQLM(std::vector<CSV> real){//Ecualize de first and last date on all data sets involved
        std::vector<CSV> REAL;
        
        if(real.size()==0){
            std::cout<<"CMD_EQLM ERROR"<<std::endl;
        }
        else{
        REAL=real;
        //Gets the newer date on the data sets and in case that the data set isent updated to the lasto one creates the new dataset based on the older one
        int nwst=std::numeric_limits<int>::min();

        for(int t=0;t<REAL.size();t++){//Get the newest date present
            int nwstid=NSTID(REAL[t].DATA);
            if(nwst<=REAL[t].DATA[nwstid][0]){
                nwst=REAL[t].DATA[nwstid][0];
            }
        }

        for(int t=0;t<REAL.size();t++){//Set the newest date present on less updated databases
            int nwstid=NSTID(REAL[t].DATA);
            if(nwst>REAL[t].DATA[nwstid][0]){
                REAL[t].DATA.push_back(REAL[t].DATA[nwstid]);
                REAL[t].DATA[REAL[t].DATA.size()-1][0]=nwst;
            }
        }
        
        /*//DBG
        for(int t=0;t<REAL.size();t++){//Get the newest date present
            int nwstid=NSTID(REAL[t].DATA);
            std::cout<<"NWEST_"<<t<<": "<<REAL[t].DATA[nwstid][0]<<std::endl;
                
        }
        //DBG*/

        //Gets the less older of the data sets
        int oldid;
        int curoldid;
        int old=std::numeric_limits<int>::min();
        int curold;
        
        for(int t=0;t<REAL.size();t++){//Gets the less older of the output data sets 
           // std::cout<<"NTC_EQEX_1; NTC_REAL.DATA_SIZE: "<<REAL[t].DATA.size()<<std::endl;
            curoldid=OSTID(REAL[t].DATA);
           // std::cout<<"NTC_EQEX_2; curoldid: "<<curoldid<<std::endl;
            curold=REAL[t].DATA[curoldid][0];
           // std::cout<<"NTC_EQEX_3"<<std::endl;
            if((curold>old)&&(curold>0)){//if the date is less old saves the date and config
                old=curold;
                oldid=t;
            }
        }
       // std::cout<<"DB ID: "<<oldid<<" OLD: "<<old<<std::endl;
        //The data base with the less history is: REAL[oldid], and the less older date is: old;
        //Gets the less older of the data sets
        
        
        //Deletes the older dates to old, and sets the oldest date on all data sets to the same
        for(int t=0;t<REAL.size();t++){
            int ODI;
            ODI=ODID(old,REAL[t].DATA);
           // std::cout<<"REAL[]"<<t<<" ODI: "<<ODI<<std::endl;
            if(ODI<0){//ERROR
                
            }
            else{//Sets the date of the older set to the date neded, gade 0 interpolation, set and hold interp.
                REAL[t].DATA[ODI][0]=old;
            }
        }
        
        for(int t=0;t<REAL.size();t++){
             DELOLD(old,REAL[t].DATA);
        }
        //Deletes the older dates to old, and sets the old date on all data sets to the same

        
        
        /*//DBG
        for(int t=0;t<REAL.size();t++){//Get the newest date present
            int olstid=OSTID(REAL[t].DATA);
            std::cout<<"OLEST_"<<t<<": "<<REAL[t].DATA[olstid][0]<<std::endl;
                
        }
        
        for(int t=0;t<REAL.size();t++){
            DBG_STD(REAL[t].DATA,STOCK_TICKETS[t]);
        }
        //DBG*/
        }
        return REAL;
        
    }
    
        std::vector<CSV> EQDP(std::vector<CSV> real, long epoch){//Ecualize date points
        //Check the date lapse is ecual on all data bases used ERROR CHECKER
        std::vector<CSV>REAL=real;
        std::vector<int> dateLapses;
        for(int t=0;t<REAL.size();t++){
             int nwstid=NSTID(REAL[t].DATA);
             long nwst=REAL[t].DATA[nwstid][0];
             
             int olstid=OSTID(REAL[t].DATA);
             long olst=REAL[t].DATA[olstid][0];
             
            dateLapses.push_back(nwst-olst);
        }
        for(int u=0;u<dateLapses.size();u++){
            for(int l=0;l<dateLapses.size();l++){
                if(dateLapses[u]!=dateLapses[l]){//Error on EQEX not all date lapses are equal
                    std::cout<<"EQEX->EQSZ ERROR"<<std::endl;
                    exit(-1);
                }
            }
        }
        
        //Check the date lapse is ecual on all data bases used ERROR CHECKER
        
        //Select the dates on which all data bases used updates, creating a date vector to compose all data from
        //double dateLaps=dateLapses[0];//All time cover by history present
        int OSID=OSTID(REAL[0].DATA);
        long begDate=REAL[0].DATA[OSID][0];//oldest date present
        //std::cout<<"begDate: "<<begDate<<std::endl;
        int NSID=NSTID(REAL[0].DATA);
        long endDate=REAL[0].DATA[NSID][0];//newest date present
       // std::cout<<"endDate: "<<endDate<<std::endl;
        
        
        std::vector<double> dateStamp;//Dates to put on equalized date vectors
        dateStamp.push_back(begDate);//Creates the strart point, gets the older date of the firt database, all oldest dates suold be the seame on all data bases

        for(long dt=begDate;dt<=endDate;dt+=(epoch/2)){//Iterates for all posible dates for dateStampos
            long newerof=dateStamp[dateStamp.size()-1];
            
            std::vector<long> nwerupt;
            
            for(int t=0;t<REAL.size();t++){//Iterates for all data bases present
                nwerupt.push_back(NDATE(dt,REAL[t].DATA));//Gets the next date on wich the data bases are updated
            }
            std::sort(nwerupt.begin(), nwerupt.end(), [](const int & a, const  int & b){ return a < b; } );
            //DBG_STD(nwerupt,"nwerupt");
            
            if(nwerupt[(int)nwerupt.size()*0.25]!=dateStamp[dateStamp.size()-1]){//Select the point on where 25% os the data is updated
            dateStamp.push_back(nwerupt[(int)nwerupt.size()*0.25]);
            }
        }
        //DBG
        //std::cout<<"DATE_STAMP.size(): "<<dateStamp.size()<<std::endl;
        //DBG_STD(dateStamp,"DATE_STAMP");
        //std::cout<<"REAL.size(): "<<REAL.size()<<std::endl;
        //DBG
        //Select the dates on which all data bases used updates, creating a date vector to compose all data from
        
        //Construct de real vector withe all the data sets with the same dateStamps
        std::vector<CSV> OREAL;
        for(int y=0;y<REAL.size();y++){
            OREAL.push_back(CSV());
            for(int w=0;w<dateStamp.size();w++){
                //ODID(dateStamp[w],REAL[y].DATA);
                OREAL[OREAL.size()-1].DATA.push_back(REAL[y].DATA[ODID(dateStamp[w],REAL[y].DATA)]);//Sets the data older or ecual to the date
                OREAL[OREAL.size()-1].DATA[OREAL[OREAL.size()-1].DATA.size()-1][0]=dateStamp[w];//Sets the date to the defined one
            }
            
        }
        
        REAL.clear();
        for(int r=0;r<OREAL.size();r++){
            REAL.push_back(OREAL[r]);
            //DBG_STD(REAL[r].DATA,STOCK_TICKETS[r]);
        }
        return REAL;
        
        //Construct de real vector withe all the data sets with the same dateStamps
    }
    
    CSV DSCT(CSV real, std::vector<int> inST){//Disector, deletes data categories not used, returns CSV with the columns given ton inST vector
        CSV REAL=real;
        CSV OREAL;
        for(int is=0;is<REAL.DATA.size();is++){
                std::vector <double>temp;
                for(int ins=0;ins<inST.size();ins++){
                    if(REAL.DATA[is].size()<inST[ins]+1){//Condition of database smaler than the standar one, predition files, break the sectioning
                        break;
                    }
                    temp.push_back(REAL.DATA[is][inST[ins]]);
                }
                OREAL.DATA.push_back(temp);
            }
            return OREAL;
    }
    
    

#endif
