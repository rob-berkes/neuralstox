//
//  main.cpp
//  NeuralStox_GetListPrevDayPluses
//
//  Created by Robert on 3/27/14.
//  Copyright (c) 2014 ___ROBERTBERKES___. All rights reserved.
//
#include <iostream>
#include "StockDataset.h"

int main(int argc, const char * argv[])
{
    cout << "Begin...";
    time_t beginTime = time(NULL);
    StockDS stocklist;
    int totalRead = 0;
    bool foundStart=false;
    totalRead = stocklist.readFromFile();
    cout << "Total Records: ";
    cout << totalRead;
    cout << "\n";
    Neurons *neurons = stocklist.matchList;
    while (neurons!=NULL){
	if (foundStart==true){
            cout << "Running report for: ";
            cout << neurons->symbol;
            cout << "\n";
            stocklist.score_prev_day(neurons->symbol);
            stocklist.writeMatchesToFile(neurons->symbol);
        }
        neurons->wasUp=0;
        neurons->wasDown=0;
        neurons=neurons->next;
	if (neurons!=NULL)
		if (neurons->symbol.compare("ABCO")==0) //last symbol completed goes here, to find starting point and save time
			foundStart=true;
    }



    time_t endTime = time(NULL);
    cout << "\nProcessing completed in ";
    cout << difftime(endTime, beginTime);
    cout << " seconds.\n";

}
