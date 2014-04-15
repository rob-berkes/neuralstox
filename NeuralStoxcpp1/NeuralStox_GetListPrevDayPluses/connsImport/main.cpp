#include "../Connections.h"
int main(int argc, const char * argv[]) {
	ConnDailyMeta connDataSet;
	connDataSet.build_fullConnList();
	connDataSet.build_symbolList();
	connDataSet.build_predictionList();
	string dateToFind="2009-07-01";
	connDataSet.build_stockList(dateToFind.c_str());
	connDataSet.fillout_symbolList();
	string newdate="2009-07-02";
	connDataSet.build_stockList(newdate.c_str());
	connDataSet.fillout_symbolList();
	connDataSet.predictOddsDown();
	connDataSet.writePredictsToFile();
	string futureCheckDate="2009-07-06";
	connDataSet.build_stockList(futureCheckDate.c_str());
	connDataSet.checkOddsDownOutliers();		
}
