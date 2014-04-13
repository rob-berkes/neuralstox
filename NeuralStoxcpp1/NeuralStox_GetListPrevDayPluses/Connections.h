
#include <fstream>
#include <istream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include "StockClass.h"

using namespace std;


class Connections {
public:
	string 	parSym;
	string	sym;
	double	watchDayPct;
	double  regularPct;
	int 	watchDayOccs;
	int 	totalOccs;
	double	strength;
	Connections	*next;
	Connections();	
	Connections(string ifname);
	Connections(string p, string s, double w, double r, int wdo, int toccs);
	void print();
	

};
void Connections::print(){
	cout << "\nParent: ";
	cout << this->parSym;
	cout << " sym: ";
	cout << this->sym;
	cout << " strength: ";
	cout << this->strength;
	cout << " watchDayPct: ";
	cout << this->watchDayPct;
	cout << " watchDayOccs: ";
	cout << this->watchDayOccs;
	cout << " regularPct: ";
	cout << this->regularPct;
	cout << " totalOccs: ";
	cout << this->totalOccs;

}

Connections::Connections(){
	}

Connections::Connections(string ifname){
	ifstream file(ifname.c_str());
	string symbol;
	string parsymbol;
	string strength;
	string pctOn;
	string numDaysOn;
	string pctNorm;
	string numDaysTot;
	Connections* ptr = this;

	while (file.good()) {
		getline(file,parsymbol,',');
		getline(file,symbol,',');
		getline(file,strength,',');
		getline(file,pctOn,',');
		getline(file,numDaysOn,',');
		getline(file,pctNorm,',');
		getline(file,numDaysTot,'\n');
		ptr->sym = symbol;
		ptr->parSym = parsymbol;
		ptr->watchDayPct = ::atof(pctOn.c_str());
		ptr->regularPct = ::atof(pctNorm.c_str());
		ptr->strength = ::atof(strength.c_str());
		ptr->watchDayOccs = ::atoi(numDaysOn.c_str());
		ptr->totalOccs = ::atoi(numDaysTot.c_str());
		ptr->next = new Connections;
	}
}

Connections::Connections(string p, string s, double w, double r, int wdo, int toccs){
	this->next=NULL;
	this->parSym = p;
	this->sym = s;
	this->watchDayPct = w;
	this->regularPct = r;
	this->watchDayOccs = wdo;
	this->totalOccs = toccs;
}
class ConnDaily {
public:
	string symbol;
	double close;
	double pclose;
	int close_matches;
	int close_total;
	ConnDaily * next;
	ConnDaily();
	ConnDaily(string sym);
	ConnDaily(string sym, double close, double pclose);
	void print();\
};

void ConnDaily::print(){
	cout << "\nsymbol: ";
	cout << this->symbol;
	cout << " close: ";
	cout << this->close;
	cout << " pclose: ";
	cout << this->pclose;
	
}
ConnDaily::ConnDaily(){
	this->symbol = '0';
	this->close = 0.0;
	this->close_matches = 0;
	this->close_total = 0;
	this->pclose = 0.0;
	this->next = NULL;
}
ConnDaily::ConnDaily(string sym){
	this->symbol = sym;
	this->close = 0.0;
	this->next = NULL;
}
ConnDaily::ConnDaily(string sym, double close, double pclose){
	this->symbol = sym;
	this->close = close;
	this->pclose = pclose;
	this->next = NULL;
}






class ConnPredicts {
public:
	string symbol;
	double oddsUp;
	double oddsDown;
	int close_match;
	int close_total;
	ConnPredicts * next;
	ConnPredicts();
	void print();
};

void ConnPredicts::print(){
	cout << this->symbol;
	cout << " oddsUp: ";
	cout << this-> oddsUp;
	cout << " oddsDown: ";
	cout << this->oddsDown;
	cout << " close_match: ";
	cout << this->close_match;
	cout << " close_total: ";
	cout << this->close_total;
	cout << "\n";
}
ConnPredicts::ConnPredicts(){
	this->symbol="NULL";
	this->oddsUp=0.0;
	this->oddsDown=0.0;
	this->next = NULL;
	this->close_match = 0;
	this->close_total = 0;
}





class ConnDailyMeta {
public:
	Connections  *fullConnList;
	ConnDaily    *symbolList;
	StockClass   *stockList;
	ConnPredicts *predictionList;
	ConnDailyMeta();
	int build_symbolList();
	int build_predictionList();
	int fillout_symbolList();
	int build_fullConnList();
	int build_stockList(string dateToFind);
	int predictOddsDown();	
	void writePredictsToFile();
private:
	string fullConnsInputFile;	
	string symbolListInputFile;
	string stockListInputFile;
	string predictsOutputFile;
};


void ConnDailyMeta::writePredictsToFile() {
    std::fstream file;
    file.open(this->predictsOutputFile.c_str(),std::fstream::out); 
    ConnPredicts * ptr = this->predictionList;
    while (ptr!=NULL) {
	file << ptr->symbol;
	file << " closeMatch: ";
	file << ptr->close_match;
	file << " closeTotal: ";
	file << ptr->close_total;
	file << " oddsDown: ";
	file << ptr->oddsDown;
	file << " oddsUp: ";
	file << ptr->oddsUp;
	file << "\n";
	ptr=ptr->next;
	}
   file.close();	
}
int ConnDailyMeta::build_predictionList(){
	cout << "init prediction list...\n";

        this->predictionList = new ConnPredicts;
        ConnPredicts * ptr = this->predictionList;
        ifstream file(this->symbolListInputFile.c_str());
        string symbol;
        while (file.good()) {
                getline(file,symbol,'\n');
                ptr->symbol = symbol;
                ptr->next = new ConnPredicts;
                ptr= ptr->next;
        }


        return 1;



}
int ConnDailyMeta::predictOddsDown(){
	cout << "predicting oddsDown..\n";
	Connections * fullConnPtr = this->fullConnList;
	ConnDaily * symbolPtr = this->symbolList;
	ConnPredicts * plist = this->predictionList;
	while (fullConnPtr != NULL) {
		symbolPtr = this->symbolList;
		plist = this->predictionList;
		string Symbol = fullConnPtr->sym;
		string Master = fullConnPtr->parSym;
		while ((symbolPtr != NULL) && (symbolPtr->symbol.compare(Symbol.c_str())!=0)) {
			symbolPtr = symbolPtr->next;
		}
		while ((plist != NULL) && (plist->symbol.compare(Master.c_str())!=0)) {
			plist = plist->next;
		}
		if ((fullConnPtr->strength > 0.0) && (symbolPtr->close < symbolPtr->pclose)) {
			plist->close_match++;
			plist->close_total++;	
			plist->oddsDown = (plist->close_match/plist->close_total);
		} else if ((fullConnPtr->strength > 0.0) && (symbolPtr->close > symbolPtr->pclose)) {
			plist->close_total++;
			plist->oddsDown = (plist->close_match/plist->close_total);
		} else if ((fullConnPtr->strength < 0.0) && (symbolPtr->close > symbolPtr->pclose)) {
			plist->close_match++;
			plist->close_total++;
			plist->oddsDown = (plist->close_match/plist->close_total);
		} else if ((fullConnPtr->strength < 0.0) && (symbolPtr->close < symbolPtr->pclose)) {
			plist->close_total++;
			plist->oddsDown = (plist->close_match/plist->close_total);
		}

		fullConnPtr=fullConnPtr->next;		
	}
return 1;
}

ConnDailyMeta::ConnDailyMeta(){
	this->fullConnList=NULL;
	this->symbolList=NULL;
	this->stockList=new StockClass;
	this->fullConnsInputFile = "LossdayConnsMaster.rpt";
	this->symbolListInputFile = "symbol.list";
	this->stockListInputFile = "/data/y09h02.csv";
	this->predictsOutputFile = "/data/work/2009/ConnPredicts.rpt";
}

int ConnDailyMeta::build_stockList(string dateToFind){
	cout << "building stockList...\n";
	ifstream   file (this->stockListInputFile.c_str());
        string value;
        string sdate;
        string close;
        string change;
        string pdate;
        const char DELIMITER = '-';
        int count=0;
        StockClass *ptr;
        ptr=this->stockList;
        std::string day,month,year;
        while (file.good()) {
            getline(file,value,',');
            getline(file,sdate,',');
            std::istringstream stm(sdate);
            getline(stm,year,DELIMITER);
            getline(stm,month,DELIMITER);
            getline(stm,day);
            getline(file,close,',');
            getline(file,change,',');
            getline(file,pdate,'\n');
	    if (sdate.compare(dateToFind)==0) {
            	ptr->symbol = value;
            	ptr->date = sdate;
            	ptr->year = ::atoi(year.c_str());
            	ptr->month = ::atoi(month.c_str());
            	ptr->day = ::atoi(day.c_str());
            	ptr->close = ::atof(close.c_str());
            	ptr->pclose = ::atof(change.c_str());
            	ptr->pdate = pdate;
            	ptr->next = new StockClass;
            	ptr=ptr->next;
            	count++;
		};
        }

    return count;	


}


int ConnDailyMeta::build_symbolList(){
	cout << "building symbolList ...\n";
	this->symbolList = new ConnDaily;
	ConnDaily * ptr = this->symbolList;
	ifstream file(this->symbolListInputFile.c_str());
	string symbol;
	while (file.good()) {
		getline(file,symbol,'\n');
		ptr->symbol = symbol;
		ptr->next = new ConnDaily;
		ptr= ptr->next;
	}


	return 1;
}
int ConnDailyMeta::fillout_symbolList(){
	cout << "populating symbolist with closes\n";
	ConnDaily * ptr = this->symbolList;
	StockClass * slptr = this->stockList;

	while (ptr!=NULL) {
		slptr = this->stockList;
		while (slptr!=NULL) {
			if (slptr->symbol.compare(ptr->symbol)==0) {
				ptr->pclose = ptr->close;
				ptr->close = slptr->close;
			}
			slptr =slptr->next;
		}
		ptr=ptr->next;
	}

return 1;
}

int ConnDailyMeta::build_fullConnList(){
	cout << "building fullConList...\n";
	this->fullConnList= new Connections;
        Connections *ptr = this->fullConnList;
	ifstream file(this->fullConnsInputFile.c_str());
        string symbol;
        string parsymbol;
        string strength;
        string pctOn;
        string numDaysOn;
        string pctNorm;
        string numDaysTot;

        while (file.good()) {
                getline(file,parsymbol,',');
                getline(file,symbol,',');
                getline(file,strength,',');
                getline(file,pctOn,',');
                getline(file,numDaysOn,',');
                getline(file,pctNorm,',');
                getline(file,numDaysTot,'\n');
                ptr->sym = symbol;
                ptr->parSym = parsymbol;
                ptr->watchDayPct = ::atof(pctOn.c_str());
                ptr->regularPct = ::atof(pctNorm.c_str());
                ptr->strength = ::atof(strength.c_str());
                ptr->watchDayOccs = ::atoi(numDaysOn.c_str());
                ptr->totalOccs = ::atoi(numDaysTot.c_str());
                ptr->next = new Connections;
                ptr = ptr->next;
        }

	return 1;
}
