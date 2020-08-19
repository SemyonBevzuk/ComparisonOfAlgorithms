#include "Logger.h"

Logger::Logger(string methodName)
{
	this->methodName = methodName;
	errorCounter = 0;
	allResults.clear();
}

void Logger::AddResult(Task& task, Parameters& methodInfo)
{
	bool isCorrect = false;
	if (fabs(task.GetOptimumPoint() - methodInfo.GetParameter("BestTrial") < methodInfo.GetParameter("Eps"))) {
		isCorrect = true;
	}
	else {
		errorCounter++;
	}
	allResults.push_back(Result(methodInfo.GetParameter("TrialsCount"), isCorrect));
}

void Logger::AddResult(Task& task, Parameters& methodInfo, vector<char> characteristicTypes)
{
	bool isCorrect = false;
	if (fabs(task.GetOptimumPoint() - methodInfo.GetParameter("BestTrial") < methodInfo.GetParameter("Eps"))) {
		isCorrect = true;
	}
	else {
		errorCounter++;
	}
	allResults.push_back(Result(methodInfo.GetParameter("TrialsCount"), isCorrect, characteristicTypes));
}

void Logger::PrintSerialStatistics()
{
	cout << "All errors: " << errorCounter << endl;
	cout << "All tasks: " << allResults.size() << endl;
	cout << "Unsolved tasks: ";
	vector<int> unsolvedTasks = GetUnsolvedTasks();
	if (!unsolvedTasks.empty()) {
		for (int i = 0; i < unsolvedTasks.size(); i++) {
			cout << unsolvedTasks[i] << " ";
		}
	}
	cout << endl;
	cout << "Trials average: " << CalculateCountTrials() / allResults.size() << endl;
	cout << "Max trials: " << FindMaxTrials() << endl;

	if (!allResults[0].characteristicTypes.empty()) {
		int countAll_r1 = 0;
		int countAll_r2 = 0;
		CalculateAllCountsR(&countAll_r1, &countAll_r2);
		cout << "R_1 using average: " << countAll_r1 / allResults.size() << "\n";
		cout << "R_2 using average: " << countAll_r2 / allResults.size() << "\n";
	}
}

string Logger::SaveSerialStatistics(Task& task, BaseMethod* method, Parameters& methodParameters)
{
	string methodName = method->GetMethodName() + "_serial_" + RoundToString(methodParameters.GetParameter("r"));
	if (dynamic_cast<BaseMethodAndPiyavsky*>(method) != nullptr) {
		methodName += "_" + RoundToString(methodParameters.GetParameter("M_Piyavsky"));
		methodName += "_" + RoundToString(methodParameters.GetParameter("lambda"));
	}
	if (dynamic_cast<IMethodWithTwoR*>(method) != nullptr) {
		methodName += "_" + RoundToString(methodParameters.GetParameter("r2"));
	}
	if (dynamic_cast<MethodWithTwoR_lambda*>(method) != nullptr) {
		methodName += "_" + RoundToString(methodParameters.GetParameter("lambda"));
	}

	string fileName = "..\\log\\serial\\" + task.GetName() + "_" + methodName + ".json";
	std::ofstream outFile(fileName);
	outFile << R"({)";
		outFile << R"("general_info": {)";
			outFile << R"("method_name": ")" << method->GetMethodName() << R"(", )";
			outFile << R"("problem_class": ")" << task.GetName() << R"(", )";
			outFile << R"("solved": )" << (allResults.size() - errorCounter) << ", ";
			outFile << R"("average_iters": )" << (CalculateCountTrials() / allResults.size()) << ", ";
			if (dynamic_cast<IMethodWithTwoR*>(method) != nullptr) {
				if (dynamic_cast<MethodWithTwoR_lambda*>(method) != nullptr) {
					outFile << R"("lambda": )" << methodParameters.GetParameter("lambda") << ", ";
				}
				outFile << R"("r1": )" << methodParameters.GetParameter("r") << ", ";
				outFile << R"("r2": )" << methodParameters.GetParameter("r2") << ", ";
				int countAll_r1 = 0;
				int countAll_r2 = 0;
				CalculateAllCountsR(&countAll_r1, &countAll_r2);
				outFile << R"("average_r1": )" << countAll_r1 / allResults.size() << ", ";
				outFile << R"("average_r2": )" << countAll_r2 / allResults.size() << "";
			}
			else {
				outFile << R"("r": )" << methodParameters.GetParameter("r");
				if (dynamic_cast<BaseMethodAndPiyavsky*>(method) != nullptr) {
					outFile << ", "  << R"("M_Piyavsky": )" << methodParameters.GetParameter("M_Piyavsky") << ", ";
					outFile << R"("lambda": )" << methodParameters.GetParameter("lambda") << "";
				}
			}
		outFile << R"(}, )"; 
		outFile << R"("tasks_info": {)";
			for (int i = 0; i < allResults.size(); i++) {
				outFile << R"(")" << i+1 << R"(": {)";
				outFile << R"("trials": )" << allResults[i].trials << ", ";
				outFile << R"("status": )" << allResults[i].isCorrect << "";
				if (dynamic_cast<IMethodWithTwoR*>(method) != nullptr) {
					outFile << R"(, )";
					int count_r1 = 0;
					int count_r2 = 0;
					CalculateCountsR(i, &count_r1, &count_r2);
					outFile << R"("r1_counter": )" << count_r1 << ", ";
					outFile << R"("r2_counter" : )" << count_r2 << ", ";
					vector<vector<int>> rInfo = GetInfoAboutR(i);
					outFile << R"("r_types": [)";
					string array_r = "";
					array_r += "[";
					for (int k = 0; k < 11; k++) {
						array_r += to_string(rInfo[0][k]) + ", ";
					}
					array_r += to_string(rInfo[0][10]) + "], [";
					for (int k = 0; k < 11; k++) {
						array_r += to_string(rInfo[1][k]) + ", ";
					}
					array_r += to_string(rInfo[1][10]) + "]";
					outFile << array_r;
					outFile << R"(])";
				}
				if (i == allResults.size() - 1) {
					outFile << R"(})";
				}
				else {
					outFile << R"(}, )";
				}
			}
		outFile << R"(})";

	outFile << R"(})";
	outFile.close();

	return task.GetName() + "_" + methodName;
}

void Logger::SaveSingleStatistics(Task& task, BaseMethod* method, Parameters& methodParameters)
{
	string methodName = method->GetMethodName() + "_single_" + RoundToString(methodParameters.GetParameter("r"));
	if (dynamic_cast<BaseMethodAndPiyavsky*>(method) != nullptr) {
		methodName += "_" + RoundToString(methodParameters.GetParameter("M_Piyavsky"));
		methodName += "_" + RoundToString(methodParameters.GetParameter("lambda"));
	}
	if (dynamic_cast<IMethodWithTwoR*>(method) != nullptr) {
		methodName += "_" + RoundToString(methodParameters.GetParameter("r2"));
	}
	if (dynamic_cast<MethodWithTwoR_lambda*>(method) != nullptr) {
		methodName += "_" + RoundToString(methodParameters.GetParameter("lambda"));
	}
	string taskName = task.GetName() + "_" +
		to_string(task.GetCurrentFuncNumber());

	string fileName = "..\\log\\tasks\\" + taskName + "_" + methodName + ".json";
	std::ofstream outFile(fileName);
	outFile << R"({)";
	outFile << R"("general_info": {)";
	outFile << R"("method_name": ")" << method->GetMethodName() << R"(", )";
	if (dynamic_cast<IMethodWithTwoR*>(method) != nullptr) {
		if (dynamic_cast<MethodWithTwoR_lambda*>(method) != nullptr) {
			outFile << R"("lambda": )" << methodParameters.GetParameter("lambda") << ", ";
		}
		outFile << R"("r1": )" << methodParameters.GetParameter("r") << ", ";
		outFile << R"("r2": )" << methodParameters.GetParameter("r2") << "";
	}
	else {
		outFile << R"("r": )" << methodParameters.GetParameter("r") << "";
		if (dynamic_cast<BaseMethodAndPiyavsky*>(method) != nullptr) {
			outFile << ", " << R"("M_Piyavsky": )" << methodParameters.GetParameter("M_Piyavsky") << ", ";
			outFile << R"("lambda": )" << methodParameters.GetParameter("lambda") << "";
		}
	}
	outFile << R"(}, )";
	outFile << R"("task_info": {)";
		outFile << R"("problem_class": ")" << task.GetName() << R"(", )";
		outFile << R"("task_number": ")" << task.GetCurrentFuncNumber() << R"(", )";
		outFile << R"("left_border": ")" << task.GetLeftBorder() << R"(", )";
		outFile << R"("right_border": ")" << task.GetRightBorder() << R"(", )";
		outFile << R"("function": )";
			vector<double> funcPoints = task.GetPointsCurrentFunction();
			string points = "[";
			for (int i = 0; i < funcPoints.size()-1; i++) {
				points += to_string(funcPoints[i]) + ", ";
			}
			points += to_string(funcPoints[funcPoints.size() - 1]) + "]";
		outFile	<< points << R"(, )";
		outFile << R"("iters": )";
			vector<CTrial> trials = method->GetConsistentTrials();
			string iters_and_types = "[";
			for (int i = 0; i < trials.size() - 1; i++) {
				iters_and_types += to_string(trials[i].x) + ", " + (trials[i].r_type) + ", ";
			}
			iters_and_types += to_string(trials[trials.size() - 1].x) + ", " + (trials[trials.size() - 1].r_type) + "]";
		outFile << iters_and_types << R"()";
	outFile << R"(})";
	outFile << R"(})";
	outFile.close();

	for (int i = 0; i < allResults.size(); i++) {
		outFile << R"(")" << i + 1 << R"(": {)";
		outFile << R"("trials": )" << allResults[i].trials << ", ";
		outFile << R"("status": )" << allResults[i].isCorrect << "";
		if (dynamic_cast<IMethodWithTwoR*>(method) != nullptr) {
			outFile << R"(, )";
			int count_r1 = 0;
			int count_r2 = 0;
			CalculateCountsR(i, &count_r1, &count_r2);
			outFile << R"("r1_counter": )" << count_r1 << ", ";
			outFile << R"("r2_counter" : )" << count_r2 << ", ";
			vector<vector<int>> rInfo = GetInfoAboutR(i);
			outFile << R"("r_types": [)";
			string array_r = "";
			array_r += "[";
			for (int k = 0; k < 11; k++) {
				array_r += to_string(rInfo[0][k]) + ", ";
			}
			array_r += to_string(rInfo[0][10]) + "], [";
			for (int k = 0; k < 11; k++) {
				array_r += to_string(rInfo[1][k]) + ", ";
			}
			array_r += to_string(rInfo[1][10]) + "]";
			outFile << array_r;
			outFile << R"(])";
		}
		if (i == allResults.size() - 1) {
			outFile << R"(})";
		}
		else {
			outFile << R"(}, )";
		}
	}
	outFile.close();
	cout << "Saving image." << endl;
	string scriptString = "python scripts/plot_func_and_points.py --path " + fileName + " --save_path ..\\img";
	cout << scriptString << endl;
	system(scriptString.c_str());
	cout << endl;
}

void Logger::Clear()
{
	errorCounter = 0;
	allResults.clear();
}

int Logger::GetUnsolvedCount()
{
	int unsolved = 0;
	for (int i = 0; i < allResults.size(); i++) {
		if (!allResults[i].isCorrect) {
			unsolved++;
		}
	}
	return unsolved;
}

int Logger::GetMaxIters()
{
	int maxTrials = allResults[0].trials;
	for (int i = 0; i < allResults.size(); i++) {
		if (allResults[i].trials > maxTrials) {
			maxTrials = allResults[i].trials;
		}
	}
	return maxTrials;
}

string Logger::RoundToString(double x)
{
	string str = to_string(x);
	return  str.substr(0, str.find('.') + 2);
}

void Logger::CalculateCountsR(int taskNumber, int* count_r1, int* count_r2)
{
	*count_r1 = 0;
	*count_r2 = 0;
	for (int i = 0; i < allResults[taskNumber].characteristicTypes.size(); i++) {
		if (allResults[taskNumber].characteristicTypes[i] == '1') {
			(*count_r1)++;
		}
		else {
			(*count_r2)++;
		}
	}
}

void Logger::CalculateAllCountsR(int* countAll_r1, int* countAll_r2)
{
	*countAll_r1 = 0;
	*countAll_r2 = 0;
	for (int i = 0; i < allResults.size(); i++) {
		int count_r1 = 0;
		int count_r2 = 0;
		for (int j = 0; j < allResults[i].characteristicTypes.size(); j++) {
			if (allResults[i].characteristicTypes[j] == '1') {
				count_r1++;
			}
			else {
				count_r2++;
			}
		}
		*countAll_r1 += count_r1;
		*countAll_r2 += count_r2;
	}
}

vector<vector<int>> Logger::GetInfoAboutR(int numberResult)
{
	vector<int> r1(11);
	vector<int> r2(11);
	int counter_r1 = 0, counter_r2 = 0;
	int idx = 0;
	int size = allResults[numberResult].characteristicTypes.size();
	for (int i = 0; i < size; i++) {
		if (i > idx * (size / 10.0)) {
			idx += 1;
		}

		if (allResults[numberResult].characteristicTypes[i] == '1') {
			r1[idx] += 1;
		}
		else{
			r2[idx] += 1;
		}
	}
	vector<vector<int>> result;
	result.push_back(r1);
	result.push_back(r2);
	return result;
}

int Logger::CalculateCountTrials()
{
	int countTrials = 0;
	for (int i = 0; i < allResults.size(); i++) {
		countTrials += allResults[i].trials;
	}
	return countTrials;
}

int Logger::FindMaxTrials()
{
	int maxTrials = 0;
	for (int i = 0; i < allResults.size(); i++) {
		if (allResults[i].trials > maxTrials) {
			maxTrials = allResults[i].trials;
		}
	}
	return maxTrials;
}

vector<int> Logger::GetUnsolvedTasks()
{
	vector<int> unsolvedTasks;
	for (int i = 0; i < allResults.size(); i++) {
		if (!allResults[i].isCorrect) {
			unsolvedTasks.push_back(i);
		}
	}
	return unsolvedTasks;
}
