#include "BaseMethod.h"

BaseMethod::BaseMethod() {
	methodName = "BaseMethod";
	maxTrials = 10000;
}

BaseMethod::BaseMethod(Parameters method_parameters): BaseMethod()
{
	methodName = "BaseMethod";
	a = method_parameters.GetParameter("a");
	b = method_parameters.GetParameter("b");
	eps = method_parameters.GetParameter("eps") * fabs(b - a);
	stopType = static_cast<stop_type>(static_cast<int>(method_parameters.GetParameter("stopType")));
	r = method_parameters.GetParameter("r");
	maxTrials = method_parameters.GetParameter("maxTrials");;
}

//Главная функция индексного метода
void BaseMethod::Run() {
	// Инициализация процесса поиска
	Init();
	bool stop = false;
	CTrial trial;
	while (!stop) {
		CalculateMu();
		pTrial t = FindMaxR();
		//Проведение испытания в интервале t
		pTrial t1 = t;
		t1--;
		double m = 1;
		if (mu > 0)
			m = r * mu;
		double new_x = 0.5 * (t->x + t1->x) - (t->Value - t1->Value) / (2.0 * m);
		trial = MakeTrial(new_x);
		//Вставка результатов очередного испытания
		stop = InsertTrial(trial);
		if (countTrials > maxTrials)
			stop = true;
	}
}

// Инициализация процесса поиска
void BaseMethod::Init() {
	countTrials = 0;
	mu = 1;
	Trials.clear();
	СonsistentTrials.clear();
	//Заносим в массив испытаний граничные точки
	CTrial trial;
	trial.x = a;
	trial.Value = func(a);
	bestTrial = trial;
	InsertTrial(trial);
	trial.x = b;
	trial.Value = func(b);
	InsertTrial(trial);
	//Проводим испытаниe во внутренней точке, например, в серединной 
	trial = MakeTrial((a + b) / 2);
	InsertTrial(trial);
}

void BaseMethod::CalculateMu() {
	pTrial i1 = Trials.begin();
	pTrial i = Trials.begin();
	i++;
	double MaxMu = -HUGE_VAL;
	double tempMu;
	for (i; i != Trials.end(); i++, i1++) {
		tempMu = fabs(i->Value - i1->Value)/(i->x - i1->x);
		if (tempMu > MaxMu) MaxMu = tempMu;
	}
	mu = MaxMu;
}

pTrial BaseMethod::FindMaxR(void) {
	pTrial t;
	pTrial i1 = Trials.begin();
	pTrial i = Trials.begin();
	i++;
	double MaxR = -HUGE_VAL;
	double R;
	for (i; i != Trials.end(); i++, i1++) {
		double deltax = i->x - i1->x;
		double m = 1;
		if (mu > 0)
			m = r * mu;
		R = deltax + (i->Value - i1->Value) * (i->Value - i1->Value) / (deltax * m * m) - 2 * (i->Value + i1->Value) / m;
		if (R > MaxR) {
			MaxR = R;
			t = i;
		}
	}
	return t;
}

CTrial BaseMethod::MakeTrial(double x) {
	CTrial Trial;
	Trial.x = x;
	Trial.Value = func(x);
	return Trial;
}

bool BaseMethod::InsertTrial(CTrial Trial) {
	// Вставка результатов
	std::pair<pTrial, bool> ins;
	ins = Trials.insert(Trial);
	countTrials += 1;
	СonsistentTrials.push_back(Trial);
	// Оценка значений 
	if (Trial.Value < bestTrial.Value) {
		bestTrial = Trial;
	}
	// Остановка по точности ACCURACY
	if (stopType == ACCURACY) {
		//точки итераций совпали - стоп
		if (!ins.second)
			return true;
		if (Trials.size() > 2) {
			pTrial j = ins.first, j1 = ins.first;
			j++;
			j1--;
			// Выполнено условие остановки
			if (fabs(j->x - ins.first->x) < eps)
				return true;
			if (fabs(ins.first->x - j1->x) < eps)
				return true;
		}
	}
	// Остановка по окрестности VICINITY
	else
	{
		if (fabs(bestTrial.x - realX) < eps)
			return true;
	}

	return false;
}

int BaseMethod::GetTrialsCount()
{
	return countTrials;
}

vector<CTrial> BaseMethod::GetConsistentTrials()
{
	return СonsistentTrials;
}

string BaseMethod::GetMethodName()
{
	return methodName;
}

void BaseMethod::PrintMethodInformation()
{
	cout << "Method: " << GetMethodName() << endl;
	cout << "Search area: ["<< a << ", " << b << "]"<< endl;
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
	cout << "MaxTrials: " << maxTrials << endl;
	cout << "eps: " << eps << endl;
	cout << "r: " << r << endl;
}

Parameters BaseMethod::GetStatistics()
{
	Parameters statistics;
	statistics.AddParameter("TrialsCount", GetTrialsCount());
	statistics.AddParameter("BestTrial", bestTrial.x);
	statistics.AddParameter("Eps", eps);
	return statistics;
}

void BaseMethod::SetLeftBorder(double a)
{
	this->a = a;
}

double BaseMethod::GetLeftBorder()
{
	return a;
}

void BaseMethod::SetRightBorder(double b)
{
	this->b = b;
}

double BaseMethod::GetRightBorder()
{
	return b;
}

void BaseMethod::SetEps(double eps)
{
	this->eps = eps;
}

double BaseMethod::GetEps()
{
	return eps;
}

void BaseMethod::SetRealX(double x)
{
	realX = x;
}

double BaseMethod::GetRealX()
{
	return realX;
}

CTrial BaseMethod::GetBestTrial()
{
	return bestTrial;
}

void BaseMethod::SetFunc(pFunc _func)
{
	func = _func;
}
