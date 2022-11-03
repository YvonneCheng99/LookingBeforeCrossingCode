#pragma once
#include<iostream>
#include <gsl/gsl_math.h>
#include <gsl/gsl_poly.h>
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
	double* RofDifferentHeight; //存储不同高度下的范围半径的数组
	int heightOfMaxR; //范围半径最大的高度对应的高度下标

	sensor2D() {
		b = 0;
		c = 0;
		h_max = 0;
		h_flight = 0;
		position = 0;
		dataAmount = 0;
		v = 0;
		RofDifferentHeight = new double[HEIGHT_BOUNDARY / DELTA_H + 1];
		heightOfMaxR = 1;
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
		RofDifferentHeight = new double[HEIGHT_BOUNDARY / DELTA_H + 1];
		heightOfMaxR = 1;
	}
	void show(void)
	{
		cout << b << '\t' << c << '\t' << position << '\t' << dataAmount << endl;
	}
	void setPosition(double x)
	{
		position = x;
	}

	void calculateR()
	{
		int i = 1;
		double  hCal = DELTA_H;
		RofDifferentHeight[i] = 0;
		while (hCal < b)
		{
			// double a[5] = { pow(hCal,4) - b * pow(hCal,3), 0, 2 * hCal * hCal - b * hCal + c, 0, 1 };
			/*
			double a[5] = { 1, 0, 2 * hCal * hCal - b * hCal + c, 0, pow(hCal,4) - b * pow(hCal,3) };
			double z[8];
			gsl_poly_complex_workspace* w = gsl_poly_complex_workspace_alloc(5);
			gsl_poly_complex_solve(a, 5, w, z);
			gsl_poly_complex_workspace_free(w);
			RofDifferentHeight[i] = abs(z[1] - z[3]) / 2;
			*/
			// 解两次一元二次方程
			double aOfEquation = 1;
			double bOfEquation = 2 * hCal * hCal - b * hCal + c;
			double cOfEquation = pow(hCal, 4) - b * pow(hCal, 3);
			// cout << "bOfEquation: " << bOfEquation << " ; cOfEquati0on" << cOfEquation << endl;
			double delta = bOfEquation * bOfEquation - 4 * aOfEquation * cOfEquation;
			// 判别式小于0，无实根
			if (delta - 0 < EPSLION)
			{
				cout << "there is something wrong when calculating the R." << endl;
				return;
			}
			//判别式不小于0
			else
			{
				//判别式等于0
				if (delta - 0 == EPSLION)
				{
					if (-bOfEquation / (2 * aOfEquation) - 0 < EPSLION)
					{
						cout << "there is something wrong when calculating the R." << endl;
						return;
					}
					else 
						RofDifferentHeight[i] = sqrt(-bOfEquation / (2 * aOfEquation));
				}
				// 判别式大于零 两个实数解
				else
				{
					double x1 = (-bOfEquation + sqrt(delta)) / (2 * aOfEquation);
					double x2 = (-bOfEquation - sqrt(delta)) / (2 * aOfEquation);
					if (x1 < 0 || x2 >0)
					{
						cout << "there is something wrong when calculating the R." << endl;
						return;
					}
					else
					{
						RofDifferentHeight[i] = sqrt(x1);
					}
				}
				//验证一下计算结果是否正确
				double y = RofDifferentHeight[i];
				/*
				double res = pow(hCal * hCal + y * y, 2) - b * hCal * (hCal * hCal + y * y) + c * y * y;
				cout << "result = " << res << endl;
				*/
			}
			if (RofDifferentHeight[i] >= RofDifferentHeight[heightOfMaxR]) heightOfMaxR = i;
			else break;
			i++;
			hCal = hCal + DELTA_H;
		}
	}
};
