#pragma once
#include "BaseMethod.h"
class BaseMethodAndPiyavsky : public BaseMethod
{
public:
	// ���������� �� ���������
	BaseMethodAndPiyavsky();
	// ���������� � ����������� ������
	BaseMethodAndPiyavsky(Parameters method_parameters);
	// ����� ���������� � ������
	void PrintMethodInformation();

private:
	// �������� ������ ��������� (������������� ������ ��������� �������)
	double M_Piyavsky = 2.0;
	// �������� ����� ���� ������� 0.0 <= lambda <= 1.0.
	// lambda = 1.0 -- �������� ����������� ������
	// lambda = 0.0 -- ����� ���������
	double lambda = 0.5; 
	// ����� ��������� � ������������ ���������������
	virtual pTrial FindMaxR(void);
};

