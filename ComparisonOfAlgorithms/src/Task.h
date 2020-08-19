#pragma once
#include <string>
#include <iostream>
#include <fstream>

#include "../generator/HillProblem.hpp"
#include "../generator/HillProblemFamily.hpp"
#include "../generator/ShekelProblem.hpp"
#include "../generator/ShekelProblemFamily.hpp"
#include "../generator/ShekelProblem.hpp"
#include "utilities.h"

using namespace std;

enum task_types { SHEKEL, HILL};

extern IOptProblem* currentProblem;

double CalculateFunc(double x);

class Task
{
public:
	Task(task_types taskType);
	double GetLeftBorder();
	double GetRightBorder();
	int GetMaxFuncNumber();
	string GetName();
	int GetCurrentFuncNumber();
	void SetCurrentFuncNumber(int funcNumber);
	double GetOptimumPoint();
	void PrintTaskInformation();
	vector<double> GetPointsCurrentFunction();
	~Task();

private:
	task_types taskType;
	string name;
	IOptProblem* problem;
	double leftBorder;
	double rightBorder;
	int maxFuncNumber;
	int currentFuncNumber;
};

