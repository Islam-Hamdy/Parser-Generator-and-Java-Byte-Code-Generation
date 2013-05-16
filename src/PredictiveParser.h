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
#include "Parser.h"
#ifndef PREDICTIVEPARSER_H_
#define PREDICTIVEPARSER_H_

using namespace std;

class PredictiveParser {
public:
	Parser * parser;
	map<int, bool> nullables; // if symbol with id x is nullable or not
	map<int, vector<int> > firstSets; //first set vectors
	vector<int> terminals_list;
	vector<int> non_terminals_list;
	bool* visited;
	int** predictive_table;
	static const int DUMMY = -1;
	int terminal_counter, non_terminal_counter;

	PredictiveParser(Parser* par);
	bool isNullable(int symbolId);
	void initializeNullables();
	int countSymbols(bool isTerminal);
	bool constructTable(); //  returns true if table is LL(1) grammar
	void generateFirstSets();
	vector<int> * go(vector<int> cur);
	vector<int> * first_set(int cur);
	void printNullables();
	void printFirstSets();
	void printPredictiveTable();

};

#endif /* PREDICTIVEPARSER_H_ */
