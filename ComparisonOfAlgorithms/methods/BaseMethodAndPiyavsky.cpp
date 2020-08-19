#include "BaseMethodAndPiyavsky.h"

BaseMethodAndPiyavsky::BaseMethodAndPiyavsky() : BaseMethod() {
	methodName = "BaseMethodAndPiyavsky";
	lambda = 0.5;
	M_Piyavsky = 1.0;
};

BaseMethodAndPiyavsky::BaseMethodAndPiyavsky(Parameters method_parameters) : BaseMethod(method_parameters) {
	methodName = "BaseMethodAndPiyavsky";
	lambda = method_parameters.GetParameter("lambda");
	M_Piyavsky = method_parameters.GetParameter("M_Piyavsky");
};

pTrial BaseMethodAndPiyavsky::FindMaxR(void) {
	double R1_Max = -HUGE_VAL;
	double R2_Max = -HUGE_VAL;
	double R1_Min = HUGE_VAL;
	double R2_Min = HUGE_VAL;
	pTrial t = Trials.begin();
	pTrial i1 = Trials.begin();
	pTrial i = Trials.begin();
	i++;
	//Вычисление характеристик
	for (i; i != Trials.end(); i++, i1++) {
		//Находим R1 и R2
		double deltax = i->x - i1->x;
		double m = 1;
		if (mu > 0)
			m = r * mu;
		double R1 = deltax + (i->Value - i1->Value) * (i->Value - i1->Value) / (deltax * m * m) - 2 * (i->Value + i1->Value) / m;
		double R2 = 0.5 * M_Piyavsky * deltax - (i->Value + i1->Value) / 2.0;
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
	i = Trials.begin();
	i++;
	for (i; i != Trials.end(); i++) {
		double R_r1, R_r2;
		//Считаем итоговую характеристику и определяем решающее правило
		if (fabs(R1_Max - R1_Min) != 0 && fabs(R2_Max - R2_Max) != 0) {
			R_r1 = lambda * ((i->R1 - R1_Min) / (R1_Max - R1_Min));
			R_r2 = (1 - lambda) * ((i->R2 - R2_Min) / (R2_Max - R2_Max));
		}
		else{
			R_r1 = lambda * (i->R1 / R1_Max);
			R_r2 = (1 - lambda) * (i->R2 / R2_Max);
		}

		double R = R_r1 + R_r2;
		if (R > R_Max)
		{
			R_Max = R;
			t = i;
		}
	}
	return t;
}

void BaseMethodAndPiyavsky::PrintMethodInformation()
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
	cout << "r: " << r << endl;
	cout << "M_Piyavsky: " << M_Piyavsky << endl;
	cout << "lambda: " << lambda << endl;
}
