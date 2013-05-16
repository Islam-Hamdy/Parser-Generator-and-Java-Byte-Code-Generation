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

vector<int> * PredictiveParser::first_set(int cur) {
	vector<int> res;
	vector<int> tmp;
	for (int i = 0; i < (parser->g[cur]).size(); i++) {
		tmp = *go(parser->g[cur][i]);
		for (int j = 0; j < tmp.size(); j++) {
			res.push_back(tmp[i]);
		}
	}
	return &res;
}

vector<int> * PredictiveParser::go(vector<int> cur) {
	vector<int> res;
	vector<int> tmp;
	for (int i = 0; i < cur.size(); i++) {
		if (parser->rev_m[cur[i]].second) {
			// terminal
			res.push_back(cur[i]);
			break;
		} else {
			tmp = *first_set(cur[i]);
			for (int j = 0; j < tmp.size(); j++) {
				// MAKE SURE THAT YOU DON"T ADD THE DON"T CARE HERE!
				res.push_back(tmp[i]);
			}
			if (!nullables[cur[i]])
				break;
		}
		if(i==cur.size()-1){
			// we're at the end and all of them are nullable

			// XXX add null char to the answer !
		}
	}
	return &res;
}
