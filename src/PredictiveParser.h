/*
 * Parser.h
 *
 *  Created on: ???/???/????
 *      Author: Islam
 */
#include <vector>
#include <map>
#include <string>
#include <iostream>
#ifndef PREDICTIVEPARSER_H_
#define PREDICTIVEPARSER_H_

using namespace std;

class PredictiveParser {
public:
	Parser * parser;
	map<int, bool> nullables; // if symbol with id x is nullable or not
	map<int, vector<int> > firstSets; //first set vectors
	bool* visited;
	vector<vector<int> > predictive_table;

	PredictiveParser(Parser* par);
	bool isNullable(int symbolId);
	void initializeNullables();
	void constructTable();

};

#endif /* PREDICTIVEPARSER_H_ */
