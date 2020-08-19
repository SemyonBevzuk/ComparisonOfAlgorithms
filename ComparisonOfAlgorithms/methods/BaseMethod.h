#pragma once

#include <vector>
#include <math.h>
#include <fstream>
#include <string>
#include <iostream>

#include "../src/utilities.h"
#include "../src/Parameters.h"

enum stop_type { ACCURACY, VICINITY };
// ACCURACY -- остановка при длине интервала меньше eps
// VICINITY -- остановка при попадании в eps окрестность известного решения

// Индексный метод
class BaseMethod {
public:
	// Левая граница интервала поиска
	void SetLeftBorder(double a);
	double GetLeftBorder();
	// Правая граница интервала поиска
	void SetRightBorder(double b);
	double GetRightBorder();
	// Точность поиска
	void SetEps(double eps);
	double GetEps();
	// Известный овтет (для остановки VICINITY)
	void SetRealX(double x);
	double GetRealX();
	// Вернуть лучшее испытание
	CTrial GetBestTrial();
	// Целевая функция задачи
	void SetFunc(pFunc func);
	// Контруктор по умолчанию
	BaseMethod();
	// Контруктор с параметрами метода
	BaseMethod(Parameters method_parameters);
	// Запуск метода
	virtual void Run();
	// Вернуть число испытаний
	int GetTrialsCount();
	// Вернуть точки в прямом порядке
	vector<CTrial> GetConsistentTrials();
	// Вернуть имя метода
	string GetMethodName();
	// Вернуть информацию о решении
	Parameters GetStatistics();
	// Вывод информации о методе
	virtual void PrintMethodInformation();

protected:
	// Левая граница интервала поиска
	double a;
	// Правая граница интервала поиска
	double b;
	// Точность поиска
	double eps;
	// Параметр надежности
	double r;
	// Цедевая функция задачи
	pFunc func;
	// Лучшее испытание
	CTrial bestTrial;
	// Максимальное число испытаний
	int maxTrials;
	// Тип остановки: ACCURACY - по точности, VICINITY - по попаданию в окрестность
	stop_type stopType = ACCURACY;
	// Известный ответ для остановки VICINITY
	double realX;
	// Оценка mu неизвестной константы Липшица
	double mu;
	// Точки испытаний
	set<CTrial> Trials;
	// Инициализация процесса поиска
	void Init();
	// Вычисление значений mu и поиск максимального
	void CalculateMu();
	// Поиск интервала с максимальной характеристикой
	virtual pTrial FindMaxR(void);
	// Проведение испытания
	CTrial MakeTrial(double);
	// Вставка результатов испытания 
	bool InsertTrial(CTrial);
	// Счётчик испытаний
	int countTrials;
	// Точки испытаний в прямом порядке
	vector<CTrial> СonsistentTrials;
	// Имя метода
	string methodName;
};
