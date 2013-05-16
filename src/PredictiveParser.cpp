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
#include <queue>
#include <set>

#define sz(a) (int)(a.size())

using namespace std;

PredictiveParser::PredictiveParser(Parser* par) {
	parser = par;
	isLLOne = true;
	initializeNullables();
	if (isLLOne) {
		generateFirstSets();
		generateFollowSets();
	}
}

void PredictiveParser::initializeNullables() {
	map<int, pair<string, bool> >::iterator miter = parser->rev_m.begin();
	while (miter != parser->rev_m.end() && isLLOne) {
		tempGlobalNull = false;
		nullables[(*miter).first] = isNullable((*miter).first, (*miter).first);
		miter++;
	}
}

bool PredictiveParser::isNullable(int symbolId, int startSymbol) {
	if (!(parser->rev_m[symbolId].first.compare("\\L"))) // epsilon match
		return true;

	if ((parser->rev_m[symbolId].second)) // terminal and not epsilon
		return false;

	if (tempGlobalNull && symbolId == startSymbol) {
		isLLOne = false;
		return false;
	}

	tempGlobalNull = true;
	vector<vector<int> > RHS = parser->g[symbolId];
	bool answer = false;
	for (int i = 0; isLLOne && !answer && i < sz(RHS) ; i++) {
		vector<int> cur = RHS[i];
		bool status = true;
		for (int j = 0; isLLOne && status && j < sz(cur) ; j++)
			status &= isNullable(cur[j], startSymbol);
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
void PredictiveParser::generateFollowSets() {
	map<int, pair<string, bool> >::iterator miter = parser->rev_m.begin();
	vector<int> * t1;
	vector<int> * follow;
	vector<int> * tmp;
	followSets[0].push_back(parser->m["$"].first);
	int nullpointer = parser->m["\\L"].first;
	miter = parser->rev_m.begin();
	while (miter != parser->rev_m.end()) {
		if (!((*miter).second.second)) {
			int cur = (*miter).first;

			for (int i = 0; i < sz(parser->g[cur]) ; i++) {
				t1 = &parser->g[cur][i];
				for (int j = 0; j < sz(parser->g[cur][i]) ; j++) {
					follow = &followSets[(*t1)[j]];
					if (parser->rev_m[(*t1)[j]].second)
						continue;
					for (int k = j + 1; k < sz(parser->g[cur][i]) ; k++) {

						if (parser->rev_m[(*t1)[k]].second) {
							// terminal
							follow->push_back((*t1)[k]);
							break;
						} else {
							tmp = &firstSets[(*t1)[k]];
							for (int j = 0; j < tmp->size(); j++) {
								if ((*tmp)[j] != nullpointer
										&& find(follow->begin(), follow->end(),
												(*tmp)[j]) == follow->end())
									follow->push_back((*tmp)[j]);
							}
							if (!nullables[(*t1)[k]])
								break;
						}
					}

				}
			}
		}
		miter++;
	}
	topo();
}

void PredictiveParser::topo() {
	vector<int> * t1;
	int nullpointer = parser->m["\\L"].first;
	for (int i = 0; i < sz(parser->g) ; i++)
		indeg[i] = 0;
	for (int i = 0; i < sz(parser->g) ; i++) {
		for (int j = 0; j < sz(parser->g[i]) ; j++) {
			t1 = &parser->g[i][j];
			for (int k = parser->g[i][j].size() - 1; k >= 0; k--) {
				tgraph[i].push_back(parser->g[i][j][k]);
				indeg[parser->g[i][j][k]]++;
				if (parser->rev_m[(*t1)[k]].second || !nullables[(*t1)[k]]) {
					break;
				}
			}
		}
	}
	map<int, int>::iterator miter = indeg.begin();
	queue<int> q;
	while (miter != indeg.end()) {
		if ((*miter).second == 0 && !parser->rev_m[(*miter).first].second) {
			q.push((*miter).first);
		}
		miter++;
	}
	while (!q.empty()) {
		int cur = q.front();
		q.pop();

		for (int i = 0; i < sz(tgraph[cur]) ; i++) {
			int nxt = tgraph[cur][i];
			if (parser->rev_m[nxt].second)
				continue;
			indeg[nxt]--;
			if (indeg[nxt] == 0 && !parser->rev_m[nxt].second)
				q.push(nxt);

			for (int j = 0; j < sz(followSets[cur]) ; j++) {
				if (followSets[nxt].end()
						== find(followSets[nxt].begin(), followSets[nxt].end(),
								followSets[cur][j]))
					followSets[nxt].push_back(followSets[cur][j]);
			}
		}
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
	set<int> first_gamma;
	vector<int> temp;

	//for each X -> gamma
	for (int i = 0; i < sz(parser->g) ; i++) {
		// get First (gamma)

		temp.clear();
		first_gamma.clear();
		bool isGammaNullable = false;
		for (int j = 0; j < sz(parser->g[i]) ; j++) {
			int k = 0;
			for (k = 0; k < sz(parser->g[i][j]) ; k++) {
				temp = firstSets[(parser->g[i][j])[k]];
				for (int kk = 0; kk < sz(temp) ; kk++)
					first_gamma.insert(temp[kk]);

				if (!nullables[(parser->g[i])[j][k]])
					break;
			}
			if (k == sz(parser->g[i][j])) { // all tokens are nullables
				isGammaNullable = true;
			}
		}
		//	 for each t in FIRST(gamma)
		// table[X,t] = X->gamma
		set<int>::iterator iter = first_gamma.begin();
		while (iter != first_gamma.end()) {
			int current = (*iter);
			if (predictive_table[i][current] != DUMMY) {
				return false; // not LL(1) grammar
			} else {
				predictive_table[i][current] = i;
			}
			iter++;
		}

		//if gamma is nullable
//		 for each t in FOLLOW(X)
		//	 table[X,t] = X->gamma
		if (isGammaNullable) {
			vector<int> x_follow = followSets[i];
			for (int j = 0; j < sz(x_follow) ; j++) {
				int current = x_follow[j];
				if (predictive_table[i][current] != DUMMY) {
					return false; // not LL(1) grammar
				} else {
					predictive_table[i][current] = i;
				}
			}
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

void PredictiveParser::printFollowSets() {
	puts("=====================================================");
	map<int, vector<int> >::iterator miter = followSets.begin();
	while (miter != followSets.end()) {
		cout << "FollowSet(" << parser->rev_m[(*miter).first].first
				<< " ) : { ";
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
