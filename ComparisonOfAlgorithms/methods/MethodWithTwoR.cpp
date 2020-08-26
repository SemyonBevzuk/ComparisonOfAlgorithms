#include "MethodWithTwoR.h"

MethodWithTwoR::MethodWithTwoR(): IMethodWithTwoR() {
	methodName = "MethodWithTwoR";
}

MethodWithTwoR::MethodWithTwoR(Parameters method_parameters): IMethodWithTwoR(method_parameters)
{
	methodName = "MethodWithTwoR";
}

void MethodWithTwoR::Run() {
	Init();
	bool stop = false;
	CTrial trial;
	while (!stop) {
		CalculateMu();
		pTrial t = FindMaxR();
		//Проведение испытания в интервале t
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
	}
}

void MethodWithTwoR::Init() {
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
	ro = pow((1.0 - 1.0 / r2) / (1.0 - 1.0 / r), 2);
	ro = pow((pow(r2, 0.5) - 1.0 / pow(r2, 0.5)) / (pow(r, 0.5) - 1.0 / pow(r, 0.5)), 2);
}

pTrial MethodWithTwoR::FindMaxR(void) {
	pTrial t = Trials.begin();
	pTrial i1 = Trials.begin();
	pTrial i = Trials.begin();
	i++;
	double MaxR = -HUGE_VAL;
	double R1, R2;
	char R_type;
	//Вычисление характеристик
	for (i; i != Trials.end(); i++, i1++) {
		double deltax = i->x - i1->x;
		double m1 = 1, m2 = 1;
		if (mu > 0) {
			m1 = r * mu;
			m2 = r2 * mu;
		}
		R1 = deltax + (i->Value - i1->Value) * (i->Value - i1->Value) / (deltax * m1 * m1) - 2 * (i->Value + i1->Value) / m1;
		R2 = deltax + (i->Value - i1->Value) * (i->Value - i1->Value) / (deltax * m2 * m2) - 2 * (i->Value + i1->Value) / m2;

		R1 = ro * R1;
		if (R1 > MaxR) {
			MaxR = R1;
			t = i;
			R_type = '1'; // первая характеристика c r1
		}
		if (R2 > MaxR) {
			MaxR = R2;
			t = i;
			R_type = '2'; // вторая характеристика c r2
		}
	}
	TypeCharacteristic.push_back(R_type);
	return t;
}

void MethodWithTwoR::PrintMethodInformation()
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
		cout << "Error!";
		break;
	}
	cout << endl;
	cout << "eps: " << eps << endl;
	cout << "r_1: " << r << endl;
	cout << "r_2: " << r2 << endl;
	cout << "ro: " << ro << endl;
}
