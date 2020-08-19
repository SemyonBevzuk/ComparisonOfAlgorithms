#include "Parameters.h"

void Parameters::AddParameter(string name, double value)
{
	parameters[name] = value;
}

double Parameters::GetParameter(string name)
{
	auto iter = parameters.find(name);
	if (iter == parameters.end()) {
		cout << "Error! " << name << " - parameter does not exist!\n";
		throw "Parameter does not exist!";
	}
	return parameters[name];
}
