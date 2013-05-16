/*
 * PredictiveParser.cpp
 *
 *  Created on: May 16, 2013
 *      Author: islamhamdy
 */

#include "PredictiveParser.h"
#include <cstdio>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <cstring>

using namespace std;

PredictiveParser::PredictiveParser(Parser* par) {
	parser = par;
	visited = new bool[parser->rev_m.size()];
	initializeNullables();
}

void PredictiveParser::initializeNullables() {
	map<int, pair<string, bool> >::iterator miter = parser->rev_m.begin();
	while (miter != parser->rev_m.end()) {
		nullables[(*miter).first] = isNullable((*miter).first);
		miter++;
	}
}

bool PredictiveParser::isNullable(int symbolId) {
	if (!(parser->rev_m[symbolId].first.compare("\L"))) // epsilon match
		return true;

	vector<vector<int> > RHS = parser->g[symbolId];
	bool answer = false;
	for (int i = 0; !answer && i < RHS.size(); i++) {
		vector<int> cur = RHS[i];
		bool status = true;
		for (int j = 0; status && j < cur.size(); j++)
			if (!visited[cur[j]]) {
				visited[cur[j]] = true;
				status &= isNullable(cur[j]);
			}
		answer |= status;
	}
	return answer;
}
