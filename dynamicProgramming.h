#pragma once
#include <iostream>
#include <vector>
#include <gsl/gsl_math.h>
#include <gsl/gsl_poly.h>
#include <cmath>
#include "sensor.h"
#include "sensor2D.h"
#include "calculate.h"
using namespace std;

//根据高度获取无人机可接收数据范围的半径
double getR(double h, double b, double c)
{
	double R = 0.0;
	double a[5] = { pow(h,4) - b * pow(h,3), 0,
		2 * h * h - b * h + c, 0, 1 };
	double z[8];
	gsl_poly_complex_workspace* w = gsl_poly_complex_workspace_alloc(5);
	gsl_poly_complex_solve(a, 5, w, z);
	gsl_poly_complex_workspace_free(w);
	for (int i = 0; i < 8; i++)
	{
		cout << z[i] << endl;
	}
	R = abs(z[0] - z[2]);
	return R;
}

//将有高度的节点的信息赋值给某一高度上节点的信息，便于调用一维算法计算
void sensor2DToSensor(sensor2D* s2D, sensor* s, double height)
{
	double R = getR(height, s2D->b, s2D->c);
	double posi = s2D->position;
	s->position = posi;
	s->start = posi - R;
	s ->finish = posi + R;
	s->time = s2D->dataAmount / V_DATA_TRANSMIT;
}


//根据气球模型获取对应高度的可传输数据范围
double getTransmittableRange(int sennum, int h)
{
	return 0.0;
}

//获取对应速度的能耗值
double getEnergyCon(double v)
{
	if (v <= 0)
	{
		cout << "Invalid velocity." << endl;
		return -1.0;
	}
	else
	{
		return 0.07 * v * v * v + 0.0391 * v * v - 13.196 * v + 390.95;
	}
}

double altitudeScheduling(sensor2D* sensors2D, double xtime, double maxHeight)
{
	//离散化H
	vector<double> h;
	int K = ceil(maxHeight / DELTA_H);
	/*if (K > 0) h = new double[K + 2]();
	else h = new double[1]();*/
	
	for (int i = 0; i <= K; i++)
	{
		h.push_back(i * DELTA_H);
	}
	
	if (DEBUG)
	{
		cout << "h:";
		for (int i = 0; i <= K; i++)
			cout << h[i] << " ";
		cout << endl;
	}
	
	//动态规划
	//存储能耗值的数组
	vector<vector<double>> e_all(SENSORNUM, vector<double>(K));
	//存储每个GN对应的高度
	vector<double> h_fly(SENSORNUM);
	//初始值
	for (int i = 1; i <= K; i++)
	{
		e_all[1][K] = 0;
	}
	for (int i = 2; i < SENSORNUM; i++)
	{
		for (int k = 1; k < K; k++)
		{
			double minEnergy = DBL_MAX;
			for (int j = 1; j < i; j++)
			{
				for (int kk = 1; kk <= K; kk++)
				{
					double delta_h = abs(h[k] - h[kk]);
					double energy_h = delta_h * HEIGHT_ENERGY_COEF;
					//TODO 调用之前的算法，计算能耗的值
					sensor* s = new sensor[i - j + 1];
					//初始化数组s和存储中间结果的数组们
					double* dd = new double[(i - j) * 3];
					int* ss = new int[(i - j) * 3];
					double* vv = new double[(i - j) * 3];
					for (int p = j; p <= i; p++)
					{
						//s[p - j] = sensors2D[p];
						////首先对于现在的高度，计算范围,给每一个sensor对象的start和finish赋值
						//double R = 1.0;
						//double location = sensors2D[p].position;
						//s[p - j].start = location - R / 2;
						//s[p - j].finish = location + R / 2;
						sensor2DToSensor(&sensors2D[p], &s[p - j], h[kk]);
					}

					//计算水平飞行的能耗
					double energy_hori = 0.0;
					energy_hori = divide(s, 0, i - j, xtime, dd, ss, vv);
					double temp = e_all[j - 1][kk] + energy_h + energy_hori;
					if (temp < minEnergy)
					{
						minEnergy = temp;
						for (int m = j; m <= i; m++)
							sensors2D[m].h_flight = h[kk];
					}
					delete[]s;
					delete[]dd;
					delete[]ss;
					delete[]vv;
				}
			}
			e_all[i][k] = minEnergy;
		}
	}

	double result = DBL_MAX;
	for (int i = 1; i <= K; i++)
	{
		if (result > e_all[SENSORNUM][i])
		{
			result = e_all[SENSORNUM][i];
			sensors2D[SENSORNUM].h_flight = h[i];
		}
	}
	
	return result;
}
