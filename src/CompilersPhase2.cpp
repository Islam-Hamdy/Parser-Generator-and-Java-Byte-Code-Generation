#include <iostream>
#include <map>
#include <cstdio>
#include <string>

#include "Parser.h"
#include "PredictiveParser.h"
using namespace std;

int main() {
	Parser* p = new Parser("CFG.in");
//	map<string, pair<int, bool> > m = p->m;
//	vector<vector<vector<int> > > g = p->g;
	int total = p->g.size();
	p->m["$"] = make_pair(total,true);
	p->rev_m[total] = make_pair("$",true);
	p->print();
	PredictiveParser*pred_parser = new PredictiveParser(p);
	pred_parser->printNullables();
	pred_parser->printFirstSets();
	pred_parser->printFollowSets();
	pred_parser->constructTable();
	pred_parser->printPredictiveTable();
//	fflush(NULL);
	return 0;
}
