#pragma once

#include <vector>
#include "BaseMethod.h"

class IMethodWithTwoR: public BaseMethod {
public:
	// ������ �������� ����������, r2 > r
	double r2;

	IMethodWithTwoR() : BaseMethod() {};
	IMethodWithTwoR(Parameters method_parameters) : BaseMethod(method_parameters) {
		r2 = method_parameters.GetParameter("r2");
	};

	virtual void Run() = 0;

	// ���������� ������ � ������ �������� ������ � ������ ���������
	virtual std::vector<char> GetVectorTypeCharacteristic()  {
		return TypeCharacteristic;
	}

protected:
	virtual void Init() = 0;
	virtual pTrial FindMaxR(void) = 0;
	// ��� ��������� ������� � ������ ���������
	std::vector<char> TypeCharacteristic;
};
