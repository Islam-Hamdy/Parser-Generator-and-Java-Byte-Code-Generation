/*
 * Parser.h
 *
 *  Created on: ???/???/????
 *      Author: ahmed
 */
#include <vector>
#include <map>
#include <string>
#include <iostream>
#ifndef PARSER_H_
#define PARSER_H_

using namespace std;

class Parser {
public:
	Parser(char* input_file);
	vector<vector<int> > get_parsing_graph();
	void parse();
	void print();
	vector<vector<vector<int> > > g;
	map<string, pair<int, bool> > m;
	map<int, pair<string, bool> > rev_m;
};

#endif /* PARSER_H_ */
