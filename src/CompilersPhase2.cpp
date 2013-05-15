#include <iostream>
#include <map>
#include <cstdio>
#include <string>

#include "Parser.h"
using namespace std;

int main() {
	Parser* p = new Parser("CFG.in");
	map<string, pair<int, bool> > m = p->m;
	vector<vector<vector<int> > > g = p->g;
	p->print();
	fflush(NULL);
	return 0;
}
