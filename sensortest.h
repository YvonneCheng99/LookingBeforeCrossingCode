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
	double b;
	double c;
	double h_max;
	double h_flight;
	double position;
	double time;
	double k;
	double j;
	double v; //速度
	double calx; //累计横坐标
	sensor() {
		b = 0;
		c = 0;
		h_max = 0;
		h_flight = 0;
		position = 0;
		time = 0;
		v = 0;

	}

	sensor(double b, double c, double x, double t) {
		init(b, c, x, t);
	}
	void init(double b, double c, double x, double t) {
		this->b = b;
		this->c = c;
		h_max = b;
		position = x;
		time = t;
		h_flight = 0;
		v = -1;
		calx = 0;
	}
	void show(void)
	{
		cout << b << '\t' << c << '\t' << position << '\t' << time << endl;
	}
};

//bool randam(sensor*, int);
//double energy_out(double dd_on[], int ss_on[], double vv_on[], int jj);