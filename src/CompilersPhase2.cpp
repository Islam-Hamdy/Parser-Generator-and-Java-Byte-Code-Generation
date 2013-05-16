#include <iostream>
#include <map>
#include <cstdio>
#include <string>

#include "Parser.h"
#include "PredictiveParser.h"
using namespace std;

int main() {
	Parser* p = new Parser("CFG.in");
	map<string, pair<int, bool> > m = p->m;
	vector<vector<vector<int> > > g = p->g;
	p->print();
	PredictiveParser*pred_parser = new PredictiveParser(p);
	pred_parser->printNullables();
	pred_parser->printFirstSets();
//	fflush(NULL);
	return 0;
}
