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
	map<int, vector<int> > followSets; //first set vectors
	map<int, vector<int> > tgraph; //first set vectors
	map<int, int> indeg; //first set vectors
	vector<int> terminals_list;
	vector<int> non_terminals_list;
	vector<int> * go(vector<int> cur);
	vector<int> * first_set(int cur);
	vector<int> * goFollow(vector<int> cur);
	vector<int> * follow_set(int cur);
	bool isLLOne;
	bool tempGlobalNull;
	int** predictive_table;
	static const int DUMMY = -1;
	int terminal_counter, non_terminal_counter;

	static const char EndOfFile = '$';

	PredictiveParser(Parser* par);
	bool isNullable(int symbolId, int startSymbol);
	void initializeNullables();
	int countSymbols(bool isTerminal);
	bool constructTable(); //  returns true if table is LL(1) grammar
	void generateFirstSets();
	void generateFollowSets();
	void printNullables();
	void printFirstSets();
	void printFollowSets();
	void topo();
	void printPredictiveTable();

};

#endif /* PREDICTIVEPARSER_H_ */
