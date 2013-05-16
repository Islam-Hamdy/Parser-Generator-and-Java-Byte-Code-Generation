/*
 * PredictiveParser.cpp
 *
 *  Created on: May 16, 2013
 *      Author: islamhamdy
 */

#include "PredictiveParser.h"
#include <cstdio>
#include <algorithm>
#include <cstring>

using namespace std;

PredictiveParser::PredictiveParser(Parser* par) {
	parser = par;
}

