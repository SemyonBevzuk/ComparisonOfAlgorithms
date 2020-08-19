#pragma once
#include "BaseMethod.h"
class BaseMethodAndPiyavsky : public BaseMethod
{
public:
	// Контруктор по умолчанию
	BaseMethodAndPiyavsky();
	// Контруктор с параметрами метода
	BaseMethodAndPiyavsky(Parameters method_parameters);
	// Вывод информации о методе
	void PrintMethodInformation();

private:
	// Параметр метода Пиявского (фиксированная оценка константы Липшица)
	double M_Piyavsky = 2.0;
	// Параметр смеси двух методов 0.0 <= lambda <= 1.0.
	// lambda = 1.0 -- алгоритм глобального поиска
	// lambda = 0.0 -- метод Пиявского
	double lambda = 0.5; 
	// Поиск интервала с максимальной характеристикой
	virtual pTrial FindMaxR(void);
};

