//
// Created by Robert on 3/27/14.
// Copyright (c) 2014 ___ROBERTBERKES___. All rights reserved.
//
#include <stdlib.h>
#include <string>


using namespace std;


#ifndef __SymbolList_H_
#define __SymbolList_H_


class Neurons {
public:
        string symbol;
        int wasUp;      // days up from previous
        int wasDown;    // days down from previous
        int tenab;      // days above ten, for init purposes
        int tenbe;      // days below ten, for init purposes
        int daygains;
        int daylosses;  // total number of g and l for the year
        double scoreNextDayUp;
        double scoreNextDayDown;
        double confidence;
        double weight;
        bool isThere;
        Neurons *next;
public:
    Neurons();
};

Neurons::Neurons() {
    this->wasUp=0; 
    this->wasDown=0;
    this->tenab=0;
    this->tenbe=0;
    this->daygains=0;
    this->daylosses=0;
    this->scoreNextDayUp=0.0;
    this->scoreNextDayDown=0.0;
    this->confidence=0.0;
    this->weight=0.0;
    this->isThere = false;
    this->symbol.assign("0");
    this->next = NULL;
}

#endif //__SymbolList_H_
