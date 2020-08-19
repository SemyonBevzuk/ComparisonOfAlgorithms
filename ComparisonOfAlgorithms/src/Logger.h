#pragma once
#include <fstream>
#include <vector>
#include <string>

#include "Task.h"
#include "Parameters.h"
#include "../methods/BaseMethod.h"
#include "../methods/BaseMethodAndPiyavsky.h"
#include "../methods/MethodWithTwoR.h"
#include "../methods/MethodWithTwoR_lambda.h"

class Logger
{
public:
	Logger(string methodName);
	void AddResult(Task& task, Parameters& methodInfo);
	void AddResult(Task& task, Parameters& methodInfo, vector<char> characteristicTypes);
	void PrintSerialStatistics();
	string SaveSerialStatistics(Task& task, BaseMethod* method, Parameters& methodParameters);
	void SaveSingleStatistics(Task& task, BaseMethod* method, Parameters& methodParameters);
	void Clear();

	int GetUnsolvedCount();
	int GetMaxIters();
private:
	struct Result {
		int trials = 0;
		bool isCorrect = false;
		vector<char> characteristicTypes;
		Result(int trials, bool isCorrect) {
			this->trials = trials;
			this->isCorrect = isCorrect;
		}
		Result(int trials, bool isCorrect, vector<char> characteristicTypes) {
			this->trials = trials;
			this->isCorrect = isCorrect;
			this->characteristicTypes = characteristicTypes;
		}
	};

	string methodName;
	int errorCounter;
	vector<Result> allResults;

	string RoundToString(double x);
	void CalculateCountsR(int taskNumber, int* count_r1, int* count_r2);
	void CalculateAllCountsR(int* countAll_r1, int* countAll_r2);
	vector<vector<int>> GetInfoAboutR(int numberResult);
	int CalculateCountTrials();
	int FindMaxTrials();
	vector<int> GetUnsolvedTasks();
};

