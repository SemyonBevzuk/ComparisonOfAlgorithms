#include "Task.h"

IOptProblem* currentProblem;

double CalculateFunc(double x)
{
	return currentProblem->ComputeFunction({ x });
}

Task::Task(task_types taskType)
{
	this->taskType = taskType;
	currentFuncNumber = 0;
	switch (taskType)
	{
	case SHEKEL:
		name = "Shekel";
		leftBorder = 0.0;
		rightBorder = 10.0;
		maxFuncNumber = 1000;
		problem = new TShekelProblem(currentFuncNumber);
		break;
	case HILL:
		name = "Hill";
		leftBorder = 0.0;
		rightBorder = 1.0;
		maxFuncNumber = 1000;
		problem = new THillProblem(currentFuncNumber);
		break;
	default:
		cout << "Error in TaskType!";
		break;
	}
	currentProblem = problem;
}

double Task::GetLeftBorder()
{
	return leftBorder;
}

double Task::GetRightBorder()
{
	return rightBorder;
}

int Task::GetMaxFuncNumber()
{
	return maxFuncNumber;
}

string Task::GetName()
{
	return name;
}

int Task::GetCurrentFuncNumber()
{
	return currentFuncNumber;
}

void Task::SetCurrentFuncNumber(int funcNumber)
{
	if (funcNumber >= 0 && funcNumber < maxFuncNumber) {
		currentFuncNumber = funcNumber;
		delete problem;
		switch (taskType)
		{
		case SHEKEL:
			problem = new TShekelProblem(currentFuncNumber);
			break;
		case HILL:
			problem = new THillProblem(currentFuncNumber);
			break;
		}
		currentProblem = problem;
	}
	else {
		string error = "Error, funcNumber must be >=0 and < " + to_string(maxFuncNumber);
		throw error;
	}
}

double Task::GetOptimumPoint()
{
	return problem->GetOptimumPoint()[0];
}

void Task::PrintTaskInformation()
{
	cout << "Problem: " << name << endl;
	cout << "Range of definition: [" << leftBorder << ", " << rightBorder << "]" << endl;
}

vector<double> Task::GetPointsCurrentFunction()
{
	string fileName = name + "_function_" + to_string(currentFuncNumber) + ".txt";
	vector<double> funcPoints;
	double step = 0.001 * fabs(rightBorder - leftBorder);
	double x = leftBorder;
    while (x <= rightBorder) {
		funcPoints.push_back(problem->ComputeFunction({ x }));
        x += step;
    }
	return funcPoints;
}

Task::~Task()
{
	delete problem;
}
