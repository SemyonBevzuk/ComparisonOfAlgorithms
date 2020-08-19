#pragma once

#include "IMethodWithTwoR.h"

class MethodWithTwoR_lambda : public IMethodWithTwoR {
public:
	MethodWithTwoR_lambda();
	MethodWithTwoR_lambda(Parameters method_parameters);
    void Run();
	void PrintMethodInformation();
    // ��������� ����� ��� R1 � R2, 0.0 <= lambda <= 1.0.
	// lambda = 1.0 -- �������� ����������� ������ ���������� ������ r = r1
	// lambda = 0.0 -- �������� ����������� ������ ���������� ������ r2
    double lambda = 1.0;

private:
    void Init();
    pTrial FindMaxR(void);
};
