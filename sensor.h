#pragma once
#include<iostream>
#include "parameters.h"

struct timeval
{
	__int64 tv_sec;
	__int64 tv_usec;
};

using namespace std;
class sensor {
public:
	double start;
	double finish;
	double position;
	double time;
	double k;
	double j;
	double v; //速度
	double calx; //累计横坐标
	sensor() {
		start = 0;
		finish = 0;
		position = 0;
		time = 0;
		v = 0;

	}

	sensor(double s, double f, double x, double t) {
		init(s, f, x, t);
	}
	void init(double s, double f, double x, double t) {
		start = s;
		finish = f;
		position = x;
		time = t;
		v = -1;
		calx = 0;
	}
	void show(void)
	{
		cout << start << '\t' << finish << '\t' << position << '\t' << time << endl;
	}
};

// bool randam(sensor*, int);
double energy_out(double dd_on[], int ss_on[], double vv_on[], int jj);
