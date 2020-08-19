#pragma once
#include <set>
#include "Task.h"
using namespace std;

const double PI = 3.141592653;
typedef double(*pFunc) (double);

inline double sign(double val) {
	return (val > 0) ? (1) : ((val < 0) ? (-1) : (0));
}

struct CTrial {
	double x;
	double Value;
	char r_type = '0';
	mutable double R1;
	mutable double R2;
};
inline bool operator<(const CTrial& t1, const CTrial& t2) { return (t1.x < t2.x); }
typedef std::set<CTrial>::iterator pTrial;
