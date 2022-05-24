#include <iostream>
#include <math.h>
#include <vector>
#include "float.h"
#include "dynamicProgramming.h"
using namespace std;


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
	
	sensor2D* s = new sensor2D[SENSORNUM];
	// double* h = new double[1];
	initSensor(s);
	//确定高度范围的最大值，即所有传感器中高度最高的传感器的高度
	for (int i = 1; i <= SENSORNUM; i++)
	{
		if (height_max < s[i].h_max)
			height_max = s[i].h_max;
	}

	double xtime = 0.0;
	altitudeScheduling(s, xtime, height_max);
}
