#include "../Connections.h"
int main(int argc, const char * argv[]) {
	ConnDailyMeta connDataSet;
	connDataSet.build_symbolList();
	connDataSet.buildNeuronList(connDataSet.lossdayNeuronList,connDataSet.LossdayInputFile);
	connDataSet.buildNeuronList(connDataSet.gaindayNeuronList,connDataSet.GaindayInputFile);
	connDataSet.build_predictionList();
	string dateToFind="2009-07-01";
	connDataSet.build_stockList(dateToFind.c_str());
	connDataSet.fillout_symbolList();
	string newdate="2009-07-02";
	connDataSet.build_stockList(newdate.c_str());
	connDataSet.fillout_symbolList();
	connDataSet.predictOddsDown();
	connDataSet.predictOddsUp();
	connDataSet.writePredictsToFile();
	string futureCheckDate="2009-07-06";
	connDataSet.build_stockList(futureCheckDate.c_str());
	connDataSet.checkOddsDownOutliers();		
	connDataSet.checkOddsUpOutliers();		
}
