#include "MethodWithTwoR_lambda.h"

MethodWithTwoR_lambda::MethodWithTwoR_lambda() : IMethodWithTwoR() {
	methodName = "MethodWithTwoRAlpha";
	lambda = 1.0;
}

MethodWithTwoR_lambda::MethodWithTwoR_lambda(Parameters method_parameters) : IMethodWithTwoR(method_parameters)
{
	methodName = "MethodWithTwoRLambda";
	lambda = method_parameters.GetParameter("lambda");
}


void MethodWithTwoR_lambda::Run() {
	Init();
	bool stop = false;
	CTrial trial;
	while (!stop) {
		CalculateMu();
		pTrial t = FindMaxR();
		// Проведение испытания в интервале  t
		pTrial t1 = t;
		t1--;
		double new_x;
		double m = 1;
		if (TypeCharacteristic[TypeCharacteristic.size() - 1] == '1') {
			if (mu > 0)
				m = r * mu;
		}
		else
		{
			if (mu > 0)
				m = r2 * mu;
		}
		new_x = 0.5 * (t->x + t1->x) - (t->Value - t1->Value) / (2.0 * m);
		trial = MakeTrial(new_x);
		trial.r_type = TypeCharacteristic[TypeCharacteristic.size() - 1];
		stop = InsertTrial(trial);
		if (countTrials > maxTrials)
			stop = true;
	}
}

void MethodWithTwoR_lambda::Init() {
	countTrials = 0;
	mu = 1;
	Trials.clear();
	СonsistentTrials.clear();
	CTrial trial;
	trial.x = a;
	trial.Value = func(a);
	bestTrial = trial;
	InsertTrial(trial);
	trial.x = b;
	trial.Value = func(b);
	InsertTrial(trial);
	trial = MakeTrial((a + b) / 2);
	InsertTrial(trial);

	TypeCharacteristic.clear();
}

pTrial MethodWithTwoR_lambda::FindMaxR(void) {
	pTrial t;
	pTrial i1 = Trials.begin();
	pTrial i = Trials.begin();
	i++;
	double R1_Max = -HUGE_VAL;
	double R2_Max = -HUGE_VAL;
	double R1_Min = HUGE_VAL;
	double R2_Min = HUGE_VAL;
	for (i; i != Trials.end(); i++, i1++) {
		double R1, R2;
		double deltax = i->x - i1->x;
		double m1 = 1, m2 = 1;
		if (mu > 0) {
			m1 = r * mu;
			m2 = r2 * mu;
		}
		R1 = deltax + (i->Value - i1->Value) * (i->Value - i1->Value) / (deltax * m1 * m1) - 2 * (i->Value + i1->Value) / m1;
		R2 = deltax + (i->Value - i1->Value) * (i->Value - i1->Value) / (deltax * m2 * m2) - 2 * (i->Value + i1->Value) / m2;
		//Находим максимум и минимум для нормировки
		if (R1 > R1_Max) R1_Max = R1;
		if (R2 > R2_Max) R2_Max = R2;
		if (R1 < R1_Min) R1_Min = R1;
		if (R2 < R2_Min) R2_Min = R2;
		//Сохраним R1 и R2
		i->R1 = R1;
		i->R2 = R2;
	}

	double R_Max = -HUGE_VAL;
	double R, R_r1, R_r2;
	char R_type;
	i = Trials.begin();
	i++;
	for (i; i != Trials.end(); i++) {
		//Считаем характеристику и определяем решающее правило
		if (fabs(R1_Max - R1_Min) != 0 && fabs(R2_Max - R2_Max) != 0) {
			R_r1 = lambda * ((i->R1 - R1_Min) / (R1_Max - R1_Min));
			R_r2 = (1 - lambda) * ((i->R2 - R2_Min) / (R2_Max - R2_Max));
		}
		else {
			R_r1 = lambda * (i->R1 / R1_Max);
			R_r2 = (1 - lambda) * (i->R2 / R2_Max);
		}

		if (R_r1 > R_r2) {
			R_type = '1'; // первая характеристика c r1
		}
		else {
			R_type = '2'; // первая характеристика с r2
		}

		R = R_r1 + R_r2;
		if (R > R_Max) {
			R_Max = R;
			t = i;
		}
	}
	TypeCharacteristic.push_back(R_type);
	return t;
}

void MethodWithTwoR_lambda::PrintMethodInformation()
{
	cout << "Method: " << GetMethodName() << endl;
	cout << "Search area: [" << a << ", " << b << "]" << endl;
	cout << "Stop type: ";
	switch (stopType)
	{
	case ACCURACY:
		cout << "ACCURACY";
		break;
	case VICINITY:
		cout << "VICINITY";
		break;
	default:
		cout << "Error in StopType!";
		break;
	}
	cout << endl;
	cout << "eps: " << eps << endl;
	cout << "r_1: " << r << endl;
	cout << "r_2: " << r2 << endl;
	cout << "lambda: " << lambda << endl;
}
