#include <iostream>
#include <fstream>

#include "methods/BaseMethod.h"
#include "methods/BaseMethodAndPiyavsky.h"
#include "methods/MethodWithTwoR.h"
#include "methods/MethodWithTwoR_lambda.h"
#include "src/Parameters.h"
#include "src/Task.h"
#include "src/Logger.h"

using namespace std;

// ��������� ������:
// BASE_METHOD -- �������� ����������� ������ (���)
// BASE_METHOD_AND_PIYAVSKY -- ����� ��� � ������ ��������� � ���������� lambda
// METHOD_WITH_TWO_R -- ��� c ������� ������� ��������� �������
// METHOD_WITH_TWO_R_LAMBDA -- ��� � ������� ������� ��������� ������� � ���������� ����� �������� ������ lambda
enum method_types { BASE_METHOD, BASE_METHOD_AND_PIYAVSKY, METHOD_WITH_TWO_R, METHOD_WITH_TWO_R_LAMBDA};

string logFiles;

void Run_single_experiment(task_types taskType, int taskNumber, method_types methodType, Parameters methodParameters) {
	// ��������� ������
	Task task(taskType);
	task.SetCurrentFuncNumber(taskNumber);
	methodParameters.AddParameter("a", task.GetLeftBorder());
	methodParameters.AddParameter("b", task.GetRightBorder());

	// ����� ������
	BaseMethod* im = nullptr;
	switch (methodType)
	{
	case BASE_METHOD:
		im = new BaseMethod(methodParameters);
		break;
	case BASE_METHOD_AND_PIYAVSKY:
		im = new BaseMethodAndPiyavsky(methodParameters);
		break;
	case METHOD_WITH_TWO_R:
		im = new MethodWithTwoR(methodParameters);
		break;
	case METHOD_WITH_TWO_R_LAMBDA:
		im = new MethodWithTwoR_lambda(methodParameters);
		break;
	default:
		cout << "Error in method type!\n";
		break;
	}
	im->SetFunc(CalculateFunc);

	Logger logger(im->GetMethodName());
	cout << "\t " << im->GetMethodName() << " " << task.GetName() << " " << taskNumber << endl;
	im->SetRealX(task.GetOptimumPoint());

	// ����� ������
	im->Run();

	// ����� � ���������� ����������
	cout << "\tMethod information:\n";
	im->PrintMethodInformation();
	cout << "\tTask information:\n";
	task.PrintTaskInformation();
	cout << "\tResult information:\n";
	if (fabs(task.GetOptimumPoint() - im->GetBestTrial().x) < im->GetEps()) {
		cout << "Status: OK\n";
	}
	else {
		cout << "Status: ERROR\n";
	}
	cout << "Optimum point: " << task.GetOptimumPoint() << endl;
	cout << "Best trial: " << im->GetBestTrial().x << endl;
	cout << "Trials: " << im->GetTrialsCount() << endl;
	cout << endl;
	logger.SaveSingleStatistics(task, im, methodParameters);
	delete im;
}

void Run_serial_experiment(task_types taskType, method_types methodType, Parameters methodParameters, int* unsolved = nullptr, int* maxTrials = nullptr) {
	// ��������� ������
	Task task(taskType);
	methodParameters.AddParameter("a", task.GetLeftBorder());
	methodParameters.AddParameter("b", task.GetRightBorder());

	// ����� ������
	BaseMethod* im = nullptr;
	switch (methodType)
	{
	case BASE_METHOD:
		im = new BaseMethod(methodParameters);
		break;
	case BASE_METHOD_AND_PIYAVSKY:
		im = new BaseMethodAndPiyavsky(methodParameters);
		break;
	case METHOD_WITH_TWO_R:
		im = new MethodWithTwoR(methodParameters);
		break;
	case METHOD_WITH_TWO_R_LAMBDA:
		im = new MethodWithTwoR_lambda(methodParameters);
		break;
	default:
		cout << "Error in method type!\n";
		break;
	}
	im->SetFunc(CalculateFunc);

	Logger logger(im->GetMethodName());

	cout << "\t " << im->GetMethodName() << endl;
	// ����� ����������� ��� ������ ������ �� �����
	for (int i = 0; i < task.GetMaxFuncNumber(); i++) {
		task.SetCurrentFuncNumber(i);
		im->SetRealX(task.GetOptimumPoint());

		im->Run();

		try {
			IMethodWithTwoR* im_TwoR = dynamic_cast<IMethodWithTwoR*>(im);
			if (im_TwoR == nullptr) {
				Parameters stats = im->GetStatistics();
				logger.AddResult(task, stats);
			}
			else {
				Parameters stats = im->GetStatistics();
				logger.AddResult(task, stats, im_TwoR->GetVectorTypeCharacteristic());
			}
		}
		catch (...) {
			cout << "Error when trying AddResult!\n";
		}
		cout << ".";
		// ���������� ����� ���������� � ������� � �����
		//cout << "#" << task.GetCurrentFuncNumber() << ": Trials = " << im->GetTrialsCount();
		//double t1 = task.GetOptimumPoint();
		//double t2 = im->GetBestTrial().x;
		//if (fabs(task.GetOptimumPoint() - im->GetBestTrial().x) < im->GetEps()) {
		//	cout << " Status: OK\n";
		//}
		//else {
		//	cout << " Status: ERROR\n";
		//}
	}

	// ����� � ���������� ���������� � ������� �����
	cout << endl;
	cout << "\tMethod information:\n";
	im->PrintMethodInformation();
	cout << "\tTask information:\n";
	task.PrintTaskInformation();
	cout << "\tResult information:\n";
	logger.PrintSerialStatistics();
	string statsFile = logger.SaveSerialStatistics(task, im, methodParameters);
	logFiles += statsFile + " ";
	cout << endl;

	if (unsolved != nullptr) {
		*unsolved = logger.GetUnsolvedCount();
	}
	if (maxTrials != nullptr) {
		*maxTrials = logger.GetMaxIters();
	}

	delete im;
}

int main(int argc, char* argv[]) {
	// ����� ��� �������� ���������� ������
	Parameters method_parameters;
	// �������� ������. � ������� ��������� ������������ eps * |b - a|.
	method_parameters.AddParameter("eps", 0.001);
	// ��������� ������� ���������:
	// ACCURACY -- ��������� ��� ����� ��������� ������ eps
	// VICINITY -- ��������� ��� ��������� � eps ����������� ���������� �������
	method_parameters.AddParameter("stopType", ACCURACY);
	// �������� ���������� ��� ���� �������
	method_parameters.AddParameter("r", 3.6);
	// ����������� �� ����� ��������
	method_parameters.AddParameter("maxTrials", 200);

	// ��������� ������: SHEKEL, HILL
	task_types task = HILL;
	// ����� ������ �� ����� ��� � �������
	int taskNumber = 10;

		//	BASE_METHOD
	// ������ �����
	Run_serial_experiment(task, BASE_METHOD, method_parameters);
	// ������ ���������� ������ � ������� taskNumber
	Run_single_experiment(task, taskNumber, BASE_METHOD, method_parameters);

		//	BASE_METHOD_AND_PIYAVSKY
	method_parameters.AddParameter("r", 3.6);
	// �������� M ��� ������ ���������
	method_parameters.AddParameter("M_Piyavsky", 4.1);
	// �������� ����� ���������� 0.0 <= lambda <= 1.0.
	// lambda = 1.0 -- �������� ����������� ������
	// lambda = 0.0 -- ����� ���������
	method_parameters.AddParameter("lambda", 0.5);
	// ������ �����
	Run_serial_experiment(task, BASE_METHOD_AND_PIYAVSKY, method_parameters);
	// ������ ���������� ������ � ������� taskNumber
	Run_single_experiment(task, taskNumber, BASE_METHOD_AND_PIYAVSKY, method_parameters);
	
		//	METHOD_WITH_TWO_R
	// �������� ���������� r ����� ��������
	method_parameters.AddParameter("r", 1.2);
	// �������� ���������� r2 > r
	method_parameters.AddParameter("r2", 3.6);
	// ������ �����
	Run_serial_experiment(task, METHOD_WITH_TWO_R, method_parameters);
	// ������ ���������� ������ � ������� taskNumber
	Run_single_experiment(task, taskNumber, METHOD_WITH_TWO_R, method_parameters);

		// METHOD_WITH_TWO_R_LAMBDA
	// �������� ���������� r ����� ��������
	method_parameters.AddParameter("r", 1.2);
	// �������� ���������� r2 > r
	method_parameters.AddParameter("r2", 3.6);
	// �������� ����� ���������� 0.0 <= lambda <= 1.0.
	// lambda = 1.0 -- ��� ���������� ������ r = r1
	// lambda = 0.0 -- ��� ���������� ������ r2
	method_parameters.AddParameter("lambda", 0.5);
	// ������ �����
	Run_serial_experiment(task, METHOD_WITH_TWO_R_LAMBDA, method_parameters);
	// ������ ���������� ������ � ������� taskNumber
	Run_single_experiment(task, taskNumber, METHOD_WITH_TWO_R_LAMBDA, method_parameters);
	

	// ���������� ������������ �������������� ��� ����� �����
	cout << "Saving image.\n";
	// x_lim -- ����������� ������ �� ��� ������� (����� ��������)
	string x_lim = to_string(int(method_parameters.GetParameter("maxTrials") * 1.1));
	string scriptString = "python scripts/plot_operating_characteristic.py --path ..\\log\\serial --files " + logFiles + " --x_lim " + x_lim + " --save_path ..\\img";
	cout << scriptString << endl;
	system(scriptString.c_str());
	cout << endl;

	return 0;
}
