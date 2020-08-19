#pragma once

#include <vector>
#include <math.h>
#include <fstream>
#include <string>
#include <iostream>

#include "../src/utilities.h"
#include "../src/Parameters.h"

enum stop_type { ACCURACY, VICINITY };
// ACCURACY -- ��������� ��� ����� ��������� ������ eps
// VICINITY -- ��������� ��� ��������� � eps ����������� ���������� �������

// ��������� �����
class BaseMethod {
public:
	// ����� ������� ��������� ������
	void SetLeftBorder(double a);
	double GetLeftBorder();
	// ������ ������� ��������� ������
	void SetRightBorder(double b);
	double GetRightBorder();
	// �������� ������
	void SetEps(double eps);
	double GetEps();
	// ��������� ����� (��� ��������� VICINITY)
	void SetRealX(double x);
	double GetRealX();
	// ������� ������ ���������
	CTrial GetBestTrial();
	// ������� ������� ������
	void SetFunc(pFunc func);
	// ���������� �� ���������
	BaseMethod();
	// ���������� � ����������� ������
	BaseMethod(Parameters method_parameters);
	// ������ ������
	virtual void Run();
	// ������� ����� ���������
	int GetTrialsCount();
	// ������� ����� � ������ �������
	vector<CTrial> GetConsistentTrials();
	// ������� ��� ������
	string GetMethodName();
	// ������� ���������� � �������
	Parameters GetStatistics();
	// ����� ���������� � ������
	virtual void PrintMethodInformation();

protected:
	// ����� ������� ��������� ������
	double a;
	// ������ ������� ��������� ������
	double b;
	// �������� ������
	double eps;
	// �������� ����������
	double r;
	// ������� ������� ������
	pFunc func;
	// ������ ���������
	CTrial bestTrial;
	// ������������ ����� ���������
	int maxTrials;
	// ��� ���������: ACCURACY - �� ��������, VICINITY - �� ��������� � �����������
	stop_type stopType = ACCURACY;
	// ��������� ����� ��� ��������� VICINITY
	double realX;
	// ������ mu ����������� ��������� �������
	double mu;
	// ����� ���������
	set<CTrial> Trials;
	// ������������� �������� ������
	void Init();
	// ���������� �������� mu � ����� �������������
	void CalculateMu();
	// ����� ��������� � ������������ ���������������
	virtual pTrial FindMaxR(void);
	// ���������� ���������
	CTrial MakeTrial(double);
	// ������� ����������� ��������� 
	bool InsertTrial(CTrial);
	// ������� ���������
	int countTrials;
	// ����� ��������� � ������ �������
	vector<CTrial> �onsistentTrials;
	// ��� ������
	string methodName;
};
