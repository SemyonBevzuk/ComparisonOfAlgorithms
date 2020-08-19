#pragma once
#include <unordered_map>
#include <iostream>
#include <string>

using namespace std;

class Parameters {
public:
	void AddParameter(string name, double value);
	double GetParameter(string name);
private:
	unordered_map <string, double> parameters;
};

