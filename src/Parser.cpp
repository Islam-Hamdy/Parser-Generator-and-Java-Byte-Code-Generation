#include "Parser.h"
#include <cstdio>
#include <algorithm>
#include <cstring>

using namespace std;

FILE *in_file;
Parser::Parser(char* input_file) {
	in_file = fopen(input_file, "r");
	if (!in_file) {
		printf("Cannot Open File %s\n", input_file);
		fflush(NULL);
		return;
	}
	parse();
}

void Parser::print() {
	cout << "map\n-------------------------------------------------" << endl;
	map<string, pair<int, bool> >::iterator miter = m.begin();
	while (miter != m.end()) {
		cout << (*miter).first << " --> map number: " << (*miter).second.first
				<< " --> is terminal: " << (*miter).second.second << endl;
		miter++;
	}
	cout << "\n\n------------------------------------------------------\n\n"
			<< endl;
	cout << "graph\n------------------------------------------------------"
			<< endl;
	for (size_t i = 0; i < g.size(); ++i)
		if (!rev_m[i].second) {
			cout << rev_m[i].first << " --> ";
			for (size_t j = 0; j < g[i].size(); ++j) {
				if (j > 0)
					cout << " | ";
				for (size_t k = 0; k < g[i][j].size(); ++k) {
					if (k > 0)
						cout << " ";
					cout << rev_m[g[i][j][k]].first;
				}
			}
			cout << endl;
		}
}

void Parser::parse() {
	char tmp[1000];
	string s;
	if (EOF == fscanf(in_file, "%s", tmp)) {
		printf("Empty File\n");
		fflush(NULL);
		return;
	}

	if (strlen(tmp) != 1 || tmp[0] != '#'
			|| EOF == fscanf(in_file, "%s", tmp)) {
		printf("Corrupted File\n");
		fflush(NULL);
		return;
	}

	while (true) {

		//read LHS
		s.clear();
		s.append(tmp);
		int from = -1;
		if (m.find(s) == m.end()) {
			m[s] = pair<int, bool>(g.size(), false);
			rev_m[g.size()] = pair<string, bool>(s, false);
			from = g.size();
			vector<vector<int> > vv;
			g.push_back(vv);
		} else {
			from = m[s].first;
		}

		// read '=' sign
		if (EOF == fscanf(in_file, "%s", tmp) || strlen(tmp) != 1
				|| tmp[0] != '=') {
			printf("Corrupted File\n");
			fflush(NULL);
			return;
		}
		bool first = true;

		// read "to" words
		while ((EOF != fscanf(in_file, "%s", tmp))) {

			bool firstword = true;

			while (firstword
					|| (EOF != fscanf(in_file, "%s", tmp)
							&& (strlen(tmp) != 1
									|| (tmp[0] != '|' && tmp[0] != '#')))) {
				// read word to go
				if (strlen(tmp) == 1
						&& (tmp[0] == '=' || tmp[0] == '|' || tmp[0] == '#')) {
					printf("Corrupted File\n");
					fflush(NULL);
					return;
				}

				//check if this word is terminal word
				bool terminal = (strlen(tmp) > 1 && tmp[0] == '\''
						&& tmp[strlen(tmp) - 1] == '\'');

				char* tmp2 = tmp;
				if (terminal) {
					tmp2++;
					tmp2[strlen(tmp) - 2] = 0;
				}

				s.clear();
				s.append(tmp2);

				int to;
				if (m.find(s) == m.end()) {
					m[s] = pair<int, bool>(g.size(), terminal);
					rev_m[g.size()] = pair<string, bool>(s, terminal);
					to = g.size();
					vector<vector<int> > vv;
					g.push_back(vv);

				} else {
					to = m[s].first;
				};

				if (firstword) {
					firstword = false;
					vector<int> tovector;
					g[from].push_back(tovector);
				}

				g[from][g[from].size() - 1].push_back(to);
			}
			if (strlen(tmp) == 1)
				if (tmp[0] == '#') {
					first = false;
					break;
				}

		}

		if (EOF == fscanf(in_file, "%s", tmp)) {
			printf("Parsing finished Successfully\n\n");
			fflush(NULL);
			return;
		}
		if (first) {
			printf("Corrupted File\n");
			fflush(NULL);
			return;
		}
	}
}

