#pragma once
#include<iostream>
#include "parameters.h"

using namespace std;

class sensor2D {
public:
	double b;
	double c;
	double h_max;
	double h_flight;
	double position;
	// double time;
	double k;
	double j;
	double v; //速度
	double calx; //累计横坐标
	double dataAmount;

	sensor2D() {
		b = 0;
		c = 0;
		h_max = 0;
		h_flight = 0;
		position = 0;
		dataAmount = 0;
		v = 0;

	}

	sensor2D(double b, double c, double x, double amount) {
		init(b, c, x, amount);
	}
	void init(double b, double c, double x, double amount) {
		this->b = b;
		this->c = c;
		h_max = b;
		position = x;
		dataAmount = amount;
		h_flight = 0;
		v = -1;
		calx = 0;
	}
	void show(void)
	{
		cout << b << '\t' << c << '\t' << position << '\t' << dataAmount << endl;
	}
};
