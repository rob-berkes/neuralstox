#include <stdlib.h>
#include <cstdio>
#include <string>
//
// Created by Robert on 3/18/14.
// Copyright (c) 2014 ___ROBERTBERKES___. All rights reserved.
//
#ifndef __StockClass_H_
#define __StockClass_H_


using namespace std;

class StockClass {
public:
    string symbol;
    string date;
    string pdate;
    double close;
    double pclose;
    int day;
    int month;
    int year;
    StockClass *next= NULL;
    public:
        StockClass();
        StockClass(string symbol, float close);

};

    StockClass::StockClass(){

    }
    StockClass::StockClass(string symbol, float close) {
        this->symbol = symbol;
        this->close = close;
        this->pclose = 0;
        this->pdate.assign("0000-00-00");
    }

#endif //__StockClass_H_

