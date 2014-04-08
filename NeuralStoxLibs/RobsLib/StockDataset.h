#include <cstdio>
#include "StockClass.h"
#include <istream>
#include <iostream>
#include <fstream>
#include <sstream>
//
// Created by Robert on 3/18/14.
// Copyright (c) 2014 ___ROBERTBERKES___. All rights reserved.
//

#include "StockClass.h"
#include "/Users/rberkes/Dropbox/Programs/Appcode/NeuralStox_GetListPrevDayPluses/NeuralStox_GetListPrevDayPluses/Neurons.h"

class StockDS {
public:
    int value=0;
    int skipCount=0;
    StockClass *stockList = new StockClass;
    StockClass *Alist = NULL;  //used in find_stock_dated to improve search time
    Neurons *matchList = new Neurons;
    StockClass *origin = stockList;
    string outfname = "/Users/rberkes/Dropbox/Data/y09h01e2.csv";
    string tenabfname = "/Users/rberkes/Dropbox/y09h01.csv";
public:
    StockDS();
    ~StockDS();
    int readFromFile();
    int writeToFile();
    int writeToTenABFile();
    bool find_prev_close(StockClass *crec);
    void score_prev_day(string WatchedSymbol);
    void writeMatchesToFile(string CurSymbol);
    StockClass& find_unset_dx(StockClass *ptr);

private:
    void init_neuron_list();
    void init_pricepoints(string CurSymbol);
    int search_Alist(string Symbol, string datetofind);
    StockClass* find_stock_begin(string SymbolToFind);
    int find_stock_dated(string Symbol,string datetofind);
};
StockDS::StockDS(){
    this->readFromFile();
}
StockDS::~StockDS() {
//    delete this->matchList;
//    delete this->stockList;
}
int StockDS::search_Alist(string Symbol, string datetofind) {
    StockClass* alist = this->Alist;
    bool alistDone = false;
    while ( (!alistDone) && (alist!=NULL) &&
            ((alist->symbol!=Symbol)||(alist->date!=datetofind))) {

        alist=alist->next;
        if (alist==NULL){
            alistDone=true;
            alist=this->stockList;
        }
    }
    if ((alist!=NULL) && (alist->close > alist->pclose)){
        this->Alist = alist;
/*        delete slist;
        delete alist;*/
        return 1;
    }
    else if ((alist!=NULL) && (alist->close < alist->pclose)) {
        this->Alist = alist;
/*        delete slist;
        delete alist;*/
        return -1;
    }

return 0;
}
int StockDS::find_stock_dated(string Symbol, string datetofind){
    StockClass* slist = this->stockList;


    int retval = 0;
    retval=search_Alist(Symbol,datetofind);
    if (retval==1)
        return 1;
    else if (retval==-1)
        return -1;

    while ( (slist!=NULL) &&
            ((slist->symbol!=Symbol) || (slist->date!=datetofind))) {
        slist=slist->next;
    }
    if (slist==NULL)
        return 0;
    else if (slist->close > slist->pclose) {
        this->Alist = slist;
     //   delete slist;
        return 1;
    }
    else if (slist->close < slist->pclose) {
        this->Alist = slist;
     //   delete slist;
        return -1;

    }

    return 0;
}
void StockDS::writeMatchesToFile(string CurSymbol) {
    std::fstream file;
    string ofname = "/Users/rberkes/Dropbox/Data/CurSymbol."+CurSymbol+".lossday";
    file.open(ofname,std::fstream::out);
    Neurons *mlist = this->matchList;
    while (mlist!=NULL){
        file << "Symbol: ";
        file << mlist->symbol;
        file << " vStUp: ";
        file << mlist->wasUp;
        mlist->wasUp=0;
        file << "  vStDown: ";
        file << mlist->wasDown;
        mlist->wasDown=0;
        file << " totUp: ";
        file << mlist->daygains;
        file << " totDown: ";
        file << mlist->daylosses;
        mlist = mlist->next;
        file << "\n";
    }
    file.close();
}
StockClass* StockDS::find_stock_begin(string SymbolToFind) {
    StockClass *ptr = this->stockList;
    while ((ptr!=NULL) && (ptr->symbol.compare(SymbolToFind)!=0)){
        ptr=ptr->next;
    }
    if (ptr!=NULL)
        return ptr;
    return NULL;
}
void StockDS::init_pricepoints(string CurSymbol) {
    StockClass *ptr = this->stockList;
    Neurons *neuron = this->matchList;
    while (neuron->symbol.compare(CurSymbol)!=0){
        neuron=neuron->next;
    }
    while (ptr->symbol.compare(CurSymbol)!=0){
        ptr=ptr->next;
    }
    while ((ptr!=NULL)&&(ptr->symbol.compare(CurSymbol)==0)){
        if (ptr->close >= 10)
            neuron->tenab++;
        else if (ptr->close < 10)
            neuron->tenbe++;
        ptr=ptr->next;
    }
 //   delete ptr;
 //   delete neuron;

}
void StockDS::init_neuron_list() {
    Neurons *neuronlist = this->matchList;
    StockClass *ptr = this->stockList;
    neuronlist->symbol = ptr->symbol;
    while (ptr!=NULL){
        if (neuronlist->symbol.compare(ptr->symbol)!=0){
            neuronlist->next = new Neurons;
            neuronlist = neuronlist->next;
            neuronlist->symbol = ptr->symbol;
        }
        ptr=ptr->next;
    }
    Neurons *neurons = this->matchList;
    while ((neurons!=NULL) && (neurons->symbol.compare("0")!=0)){
        StockClass* stocks = this->stockList;
        while (stocks->symbol!=neurons->symbol)
            stocks=stocks->next;
        if (stocks!=NULL){
            while ((stocks!=NULL) && (stocks->symbol.compare(neurons[0].symbol)==0)){

                if (stocks->close > stocks->pclose)
                    neurons->daygains++;
                else if (stocks->close < stocks->pclose)
                    neurons->daylosses++;
                stocks=stocks->next;
            }
            neurons=neurons->next;
//        delete stocks;
        }
//    delete neuronlist;
//    delete ptr;
        }

}


void StockDS::score_prev_day(string WatchedSymbol)
 {
    StockClass *ptr = this->find_stock_begin(WatchedSymbol);
    int day = 0;

    while (ptr->symbol.compare(WatchedSymbol)==0) {  //cycle: every day of a stock report for symbol
       // if (ptr->close > ptr->pclose) { //Matches a 'gain' day, now record symbols prev day...
          if (ptr->close < ptr->pclose) {  //matches a 'loss' day
            Neurons *symptr = this->matchList;

            while (symptr!=NULL){   //cycle: neurons
                while (symptr->symbol.compare(WatchedSymbol)==0){
                    symptr=symptr->next;
                } //skip WatchedSymbol
                string curSymbol = symptr->symbol; //for clarity's sake

                if ((ptr!=NULL)&&(ptr!=0)){
                    int retcode=this->find_stock_dated(curSymbol,ptr->pdate);

                    if (retcode==1)
                        symptr->wasUp++;
                    else if (retcode==-1)
                        symptr->wasDown++;
                }

                symptr=symptr->next;
            }
           // delete symptr;
        }
        ptr=ptr->next;
        day++;
        cout << "\nWatched: ";
        cout << WatchedSymbol;
        cout << " Day: ";
        cout << day;
    }
    // delete ptr;
}


bool StockDS::find_prev_close(StockClass *crec) {
    bool found = false;
    for (int day = crec->day - 1;day > 0;day--){
        StockClass *ptr = crec;
        while ((ptr!=NULL) && (found==false)) {
            if ( \
            (day == ptr->day) && (day < crec->day) && (ptr->month==crec->month) && (ptr->symbol==crec->symbol) \
            && (found==false)) {
                crec->pclose = ptr->close;
                crec->pdate = ptr->date;
                found=true;
            }
            ptr=ptr->next;
        }
//        delete ptr;
    }
    if (found == false) {
        for (int day = crec->day - 1;day > 0;day--){
            StockClass *ptr = this->stockList;
            while ((ptr!=NULL) && (found==false)) {
                if (\
                (day == ptr->day) && (day < crec->day) && (ptr->month==crec->month) && (ptr->symbol==crec->symbol) && \
                (found==false)) {
                    crec->pclose = ptr->close;
                    crec->pdate = ptr->date;
                    found=true;
                }
                ptr=ptr->next;
            }
//            delete ptr;
        }
    }

    if (found==false){
        int month=crec->month-1;
        if (month > 0){
            for (int day=31;day>0;day--){
                StockClass *ptr = this->stockList;
                while ((ptr!=NULL) && (found==false)) {
                    if (  \
                    (ptr->day == day) && (ptr->month == month) && (ptr->symbol == crec->symbol) \
                    ){
                        crec->pclose = ptr->close;
                        crec->pdate = ptr->date;
                        found=true;
                    }
                    ptr=ptr->next;
                }

//                delete ptr;
            }
        }
    }
    return found;
}
StockClass& StockDS::find_unset_dx(StockClass *ptr){
    while (ptr->pclose != 0) {
        ptr = ptr->next;
    }
    return *ptr;
}
int StockDS::writeToTenABFile() {
    std::fstream file;
    StockClass *stockPrices = this->stockList;
    file.open(this->tenabfname,std::fstream::out);
    int count=0;
    while (stockPrices!=NULL){
        Neurons *neuron = this->matchList;
        while ((neuron!=NULL) && (neuron->symbol.compare(stockPrices->symbol))) {
            neuron=neuron->next;
        }
        if ((neuron->tenab > neuron->tenbe) && (stockPrices->pdate.compare("0")!=0)){
            file << stockPrices->symbol;
            file << ",";
            file << stockPrices->date;
            file << ",";
            file << stockPrices->close;
            file << ",";
            file << stockPrices->pclose;
            file << ",";
            file << stockPrices->pdate;
            file << "\n";
            count++;
        }
        stockPrices= stockPrices->next;
    }
    file.close();
    return count;
}
int StockDS::writeToFile(){
    std::fstream file;
    StockClass *ptr = this->stockList;
    file.open(this->outfname,std::fstream::out);
    int count=0;
    while (ptr!= NULL){
        string line;
        line=ptr->symbol+','+ptr->date+',';
        file << line;
        file << ptr->close;
        file << ',';
        file << ptr->pclose;
        file << ',';
        file << ptr->pdate;
        file << '\n';
        ptr=ptr->next;
        count++;
    }
    file.close();
    return count;
}
int StockDS::readFromFile()
    {
        ifstream   file (this->outfname);
        string value;
        string sdate;
        string close;
        string change;
        string pdate;
        const char DELIMITER = '-';
        int count=0;
        StockClass *ptr;
        ptr=this->stockList;
        while (file.good()) {
            getline(file,value,',');
            ptr->symbol = value;
            getline(file,sdate,',');
            ptr->date = sdate;
            std::istringstream stm(sdate);
            std::string day,month,year;
            getline(stm,year,DELIMITER);
            getline(stm,month,DELIMITER);
            getline(stm,day);
            ptr->year = ::atoi(year.c_str());
            ptr->month = ::atoi(month.c_str());
            ptr->day = ::atoi(day.c_str());
            getline(file,close,',');
            ptr->close = ::atof(close.c_str());
            getline(file,change,',');
            ptr->pclose = ::atof(change.c_str());
            getline(file,pdate,'\n');
            ptr->pdate = pdate;
            ptr->next = new StockClass;
            ptr=ptr->next;
            count++;
        }
        this->init_neuron_list();
//        delete ptr;
    this->Alist = this->stockList;
    return count;
}
