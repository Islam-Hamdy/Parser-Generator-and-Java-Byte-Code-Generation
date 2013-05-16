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

#define sz(a) (int)(a.size())

using namespace std;

PredictiveParser::PredictiveParser(Parser* par) {
	parser = par;
	visited = new bool[sz(parser->g) ];
	initializeNullables();
	generateFirstSets();
}

void PredictiveParser::initializeNullables() {
	map<int, pair<string, bool> >::iterator miter = parser->rev_m.begin();
	while (miter != parser->rev_m.end()) {
		nullables[(*miter).first] = isNullable((*miter).first);
		miter++;
	}
}

bool PredictiveParser::isNullable(int symbolId) {
	if (!(parser->rev_m[symbolId].first.compare("\\L"))) // epsilon match
		return true;

	if ((parser->rev_m[symbolId].second)) // terminal and not epsilon
		return false;

	vector<vector<int> > RHS = parser->g[symbolId];
	bool answer = false;
	for (int i = 0; !answer && i < sz(RHS) ; i++) {
		vector<int> cur = RHS[i];
		bool status = true;
		for (int j = 0; status && j < sz(cur) ; j++)
			status &= isNullable(cur[j]);
		answer |= status;
	}
	return answer;
}

void PredictiveParser::generateFirstSets() {
	map<int, pair<string, bool> >::iterator miter = parser->rev_m.begin();
	while (miter != parser->rev_m.end()) {
		firstSets[(*miter).first] = *first_set((*miter).first);
		miter++;
	}
}

vector<int> * PredictiveParser::first_set(int cur) {
	vector<int> *res = new vector<int>;
	vector<int> tmp;
	if (parser->rev_m[cur].second) {
		res->push_back(cur);
	} else {
		for (int i = 0; i < sz(parser->g[cur]) ; i++) {
			tmp = *go(parser->g[cur][i]);
			for (int j = 0; j < sz(tmp) ; j++) {
				if (find(res->begin(), res->end(), tmp[j]) == res->end())
					res->push_back(tmp[j]);
			}
		}
	}
	return res;
}

vector<int> * PredictiveParser::go(vector<int> cur) {
	vector<int> *res = new vector<int>;
	vector<int> tmp;
	int nullpointer = parser->m["\\L"].first;
	for (int i = 0; i < sz(cur) ; i++) {
		if (parser->rev_m[cur[i]].second) {
			// terminal
//			puts(parser->rev_m[cur[i]].first.c_str());
			res->push_back(cur[i]);
			break;
		} else {
			tmp = *first_set(cur[i]);
			for (int j = 0; j < sz(tmp) ; j++) {
				if (tmp[j] != nullpointer)
					res->push_back(tmp[j]);
			}
			if (!nullables[cur[i]])
				break;
		}
		if (i == sz(cur) - 1) {
			// we're at the end and all of them are nullable
			res->push_back(nullpointer);
			// XXX add null char to the answer !
		}
	}
	return res;
}

bool PredictiveParser::constructTable() {
//	predictive_table
	terminal_counter = countSymbols(true);
	non_terminal_counter = countSymbols(false);
	predictive_table = new int*[non_terminal_counter];
	for (int i = 0; i < non_terminal_counter; ++i) {
		predictive_table[i] = new int[terminal_counter];
		memset(predictive_table[i], DUMMY, sizeof(int) * terminal_counter);
	}

	//for each X -> gamma
	for (int i = 0; i < sz(parser->g) ; i++) {
		// get First (gamma)

		vector<int> first_gamma;
		int first_not_nullable = 0;
		bool not_nullable_exists = false;
		for (int j = 0; j < sz(parser->g[i]) ; j++) {
			for (int k = 0; k < sz(parser->g[i][j]) ; k++) {
				if (!nullables[(parser->g[i])[j][k]]) {
					first_not_nullable = j;
					not_nullable_exists = true;
					break;
				}
			}
			vector<int> temp = firstSets[(parser->g[i][j])[first_not_nullable]];
			for (int kk = 0; kk < sz(temp) ; kk++)
				first_gamma.push_back(temp[kk]);
		}
		//	 for each t in FIRST(gamma)
		// table[X,t] = X->gamma
		for (int j = 0; j < sz(first_gamma) ; j++) {
			if (predictive_table[i][first_gamma[j]] != DUMMY) {
				return false; // not LL(1) grammar
			} else {
				predictive_table[i][first_gamma[j]] = i;
			}
		}

		//if gamma is nullable
//		 for each t in FOLLOW(X)
		//	 table[X,t] = X->gamma
		if (!not_nullable_exists) {
			// TODO merging followsets
		}
	}
	return true;
}

int PredictiveParser::countSymbols(bool isTerminal) {
	int counter = 0;
	map<int, pair<string, bool> >::iterator miter = parser->rev_m.begin();
	while (miter != parser->rev_m.end()) {
		if ((*miter).second.second == isTerminal) {
			if (isTerminal)
				terminals_list.push_back((*miter).first);
			else
				non_terminals_list.push_back((*miter).first);
			counter++;
		}
		miter++;
	}
	return counter;
}

void PredictiveParser::printNullables() {
	map<int, bool>::iterator miter = nullables.begin();
	while (miter != nullables.end()) {
		cout << "Symbol { " << parser->rev_m[(*miter).first].first
				<< " } Is :  ";
		if ((*miter).second)
			cout << "Nullable";
		else
			cout << "Not nullable";
		cout << endl;
		miter++;
	}
}

void PredictiveParser::printFirstSets() {
	map<int, vector<int> >::iterator miter = firstSets.begin();
	while (miter != firstSets.end()) {
		cout << "FirstSet(" << parser->rev_m[(*miter).first].first << " ) : { ";
		for (int i = 0; i < sz((*miter).second) ; i++) {
			cout << parser->rev_m[((*miter).second)[i]].first << ", ";
		}
		cout << " }" << endl;
		miter++;
	}
}

void PredictiveParser::printPredictiveTable() {
	map<int, pair<string, bool> >::iterator miter = parser->rev_m.begin();
	while (miter != parser->rev_m.end()) {
		int cur = (*miter).first;
		cout << parser->rev_m[cur].first << " --> { ";
		for (int j = 0; j < terminal_counter; j++) {
			cout << "(" << parser->rev_m[terminals_list[j]].first
					<< ", Production # "
					<< predictive_table[cur][terminals_list[j]] << "),  ";
		}
		cout << " }" << endl;
		miter++;
	}
}
