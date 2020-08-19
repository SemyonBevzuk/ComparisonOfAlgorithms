#pragma once

#include "IMethodWithTwoR.h"

class MethodWithTwoR: public IMethodWithTwoR {
public:
	MethodWithTwoR();
	MethodWithTwoR(Parameters method_parameters);
    void Run();
	void PrintMethodInformation();

private:
    // Константа для сравнимости R1 и R2
    double ro;
    void Init();
    pTrial FindMaxR(void);
};
