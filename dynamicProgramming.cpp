#include <iostream>
#include <math.h>
#include <vector>
#include "float.h"
#include "dynamicProgramming.h"
#include "myrand.h"
using namespace std;

const int MIN_VAL = 5;
const int MAX_VAL = 100;


void initSensor(sensor2D* s)
{
	s[1] = sensor2D(2.0, 3.0, 2.0, 1.0);
	s[2] = sensor2D(2.5, 4.0, 3.0, 1.4);
	s[3] = sensor2D(1.5, 1.5, 3.8, 0.8);
	s[4] = sensor2D(4.0, 3.0, 6.5, 1.2);
}



int main()
{
	/*
	sensor* sensors2D = new sensor[SENSORNUM];
	//随机初始化GN的s和f的值
	randam(sensors2D, SENSORNUM);
	for (int i = 0; i < SENSORNUM; i++)
	{
		cout << i << "\t";
		(sensors2D + i)->show();
	}
	return 0;
	*/
	
	double height_max = 0.0;

	
	long seed = 1564584525524;
	srand(seed);
	double location[SENSORNUM * 3] = { 0.0 };//
	int sensorNumber[SENSORNUM *3] = { 0 };
	double speed[SENSORNUM * 3] = { 0.0 };//
	for (int i = 0; i < 1; i++)
	{
		cout << "ith:" << i << endl;
		sensor2D* s = new sensor2D[SENSORNUM + 1];
		randam(s, SENSORNUM + 1);
		cout << "sensor show:-------------------------" << endl;
		for (int i = 1; i <= SENSORNUM; i++)
		{
			if (height_max < s[i].h_max)
				height_max = s[i].h_max;
			s[i].show();
		}
		double xtime = 0.0;
		memset(location, 0.0, sizeof(location));
		memset(sensorNumber, 0, sizeof(sensorNumber));
		memset(speed, 0.0, sizeof(speed));
		
		altitudeScheduling(s, xtime, height_max, location, sensorNumber, speed);
		ofstream file("sensor.csv");
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
	

	/*sensor2D* s = new sensor2D[SENSORNUM + 1];
	double* h = new double[1];
	initSensor(s);
	s[1].calculateR();*/
	////randam(s, SENSORNUM + 1);
	////确定高度范围的最大值，即所有传感器中高度最高的传感器的高度
	//for (int i = 1; i <= SENSORNUM; i++)
	//{
	//	if (height_max < s[i].h_max)
	//		height_max = s[i].h_max;
	//	// s[i].show();
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
