#include <iostream>
#include <math.h>
#include <vector>
#include "float.h"
#include "dynamicProgramming.h"
#include "myrand.h"
using namespace std;

const int MIN_VAL = 5;
const int MAX_VAL = 100;
const int NUM_OF_EXP = 1;

void initSensor(sensor2D* s)
{
	s[1] = sensor2D(2.0, 3.0, 2.0, 1.0);
	s[2] = sensor2D(2.5, 4.0, 3.0, 1.4);
	s[3] = sensor2D(1.5, 1.5, 3.8, 0.8);
	s[4] = sensor2D(4.0, 3.0, 6.5, 1.2);
}


int main()
{
	double height_max = 0.0;
	double min_max_height = DBL_MAX;
	
	long seed = 1564584525524;
	srand(seed);
	double location[SENSORNUM * 3] = { 0.0 };//
	int sensorNumber[SENSORNUM *3] = { 0 };
	double speed[SENSORNUM * 3] = { 0.0 };//
	long seeds[NUM_OF_EXP + 1] = { 0.0 };
	double energy[NUM_OF_EXP + 1] = { 0.0 };//存储改变高度算法的能耗
	double energyNotChange[NUM_OF_EXP + 1] = { 0.0 };//存储不改变高度的算法的最低的能耗
	//不改变高度算法的一些存储数据的数组
	double* dd = new double[SENSORNUM * 3];
	int* ss = new int[SENSORNUM * 3];
	double* vv = new double[SENSORNUM * 3];
	sensor* sNotChange = new sensor[SENSORNUM + 1];
	
	for (int i = 0; i < NUM_OF_EXP; i++)
	{
		cout << "ith:" << i << endl;
		sensor2D* s = new sensor2D[SENSORNUM + 1];
		seed = randam(s, SENSORNUM + 1);
		seeds[i] = seed;
		cout << "sensor show:-------------------------" << endl;
		for (int k = 1; k <= SENSORNUM; k++)
		{
			if (height_max < s[k].h_max)
				height_max = s[k].h_max;
			if (min_max_height > s[k].h_max)
				min_max_height = s[k].h_max;
			s[k].show();
			//先检查一下每个传感器每个高度上的范围半径是否都计算过了
			for (int j = 0; j < s[k].h_max/DELTA_H; j++)
			{
				cout << s[k].RofDifferentHeight[j] << endl;
			}
		}
		double xtime = 0.0;
		memset(location, 0.0, sizeof(location));
		memset(sensorNumber, 0, sizeof(sensorNumber));
		memset(speed, 0.0, sizeof(speed));
		// 有高度的算法
		energy[i] = altitudeScheduling(s, xtime, height_max, location, sensorNumber, speed);
		// 不改变高度的方法，对于每个可能的高度都计算一遍，取能耗最低的一个
		energyNotChange[i] = DBL_MAX;
		for (int j = 1; j < min_max_height / DELTA_H; j++)
		{
			int numOfP = 0;
			bool judge = true;
			for (int p = 1; p <= SENSORNUM; p++)
			{
				judge = sensor2DToSensor(&s[p], &sNotChange[p - j - 1], j);
				if (!judge) break;
			}
			for (int p = 0; p < SENSORNUM * 3; p++)
			{
				dd[p] = 0.0;
				ss[p] = 0;
				vv[p] = 0.0;
			}
			double e = divide(sNotChange, 0, i - j, xtime, dd, ss, vv, numOfP);
			energyNotChange[i] = e < energyNotChange[i] ? e : energyNotChange[i];
		}
		string fileName = "sensor" + to_string(seeds[i]) + ".csv";
		ofstream file(fileName);
		if (file)
		{
			for (int i = 1; i <= SENSORNUM; i++)
			{
				file << s[i].b << " ," << s[i].c << " ," << s[i].position << "\n";
			}
		}
		file.close();
		
		delete[]s;
	}
	ofstream file("result.csv");
	if (file)
	{
		for (int i = 0; i < NUM_OF_EXP; i++)
		{
			file << seeds[i] << " ," << energy[i] <<"," << energyNotChange[i] << "\n";
		}
	}
	file.close();
	
	/*sensor2D* s = new sensor2D[SENSORNUM + 1];
	double* h = new double[1];
	initSensor(s);
	s[1].calculateR();*/
	////randam(s, SENSORNUM + 1);
	////确定高度范围的最大值，即所有传感器中高度最高的传感器的高度
	//for (int k = 1; k <= SENSORNUM; k++)
	//{
	//	if (height_max < s[k].h_max)
	//		height_max = s[k].h_max;
	//	// s[k].show();
	//}

	//double xtime = 0.0;
	//double location[max_val * 3] = { 0.0 };//
	//int sensorNumber[SENSORNUM * 3] = { 0 };
	//double speed[SENSORNUM * 3] = { 0.0 };//
	//altitudeScheduling(s, xtime, height_max, location, sensorNumber, speed);
	
	/*
	//test divide
	sensor* s = new sensor[SENSORNUM + 1];
	s[0] = sensor(0, 1.0, 0.5, 1.0);
	s[1] = sensor(1.2, 2.8, 2.0, 2.0);
	s[2] = sensor(1.8, 3.0, 2.4, 1.5);
	s[3] = sensor(2.5, 3.5, 3.0, 1.0);
	double x_time = 0;
	double dd[SENSORNUM * 3] = { 0.0 };//
	int ss[SENSORNUM * 3] = { 0 };
	double vv[SENSORNUM * 3] = { 0.0 };//
	int numOfPeriod = 0;
	double energy = divide(s, 0, SENSORNUM, x_time, dd, ss, vv, numOfPeriod);
	cout << numOfPeriod << endl;
	*/
	// double R = getR(2.0, 2.0, 3.0);
	// cout << "R:" << R << endl;
	//delete[]s;
}
