#include"sensor.h"
#include"calculate.h"
#include<iostream>
#include <algorithm>

using namespace std;

double energy_out(double dd[], int ss[], double vv[], int jj) {
	for (int i = 0; i < jj; i++)
		vv[i] = vv[i] > VBEST ? VBEST : vv[i];
	if (DEBUG)
		for (int i = 0; i < jj; i++)
			cout << dd[i] << "   " << ss[i] << "   " << vv[i] << endl;

	double d, v, p, e = 0;
	for (int i = 0; i < jj - 1; i++) {
		d = dd[i + 1] - dd[i];
		v = vv[i];
		p = 0.07 * v * v * v + 0.0391 * v * v - 13.196 * v + 390.95;
		e += p * d / v;
	}
	return e;
}


int main() {
	sensor* ptr = new sensor[SENSORNUM + 1];//传感器

	double xtime = 0;
	double xtime_on = 0;
	//随机初始化GN的s和f的值
	randam(ptr, SENSORNUM);
	if (DEBUG)
		for (int i = 0; i < SENSORNUM; i++)
		{
			cout << i << "\t";
			(ptr + i)->show();
		}


	//TODO
	xtime = ptr[0].start / VBEST;//离线的时间
	xtime_on = xtime;//在线的时间
	//离线算法
	double dd[SENSORNUM * 3] = { 0.0 };//
	int ss[SENSORNUM * 3] = { 0 };
	double vv[SENSORNUM * 3] = { 0.0 };//
	divide(ptr, 0, SENSORNUM, xtime, dd, ss, vv);

	//在线算法


	//声明在线数组变量（距离&速度）,和对应的指示物
	double dd_on[SENSORNUM * 3] = { 0.0 };
	int ss_on[SENSORNUM * 3] = { 0 };
	double vv_on[SENSORNUM * 3] = { 0.0 };


	cal_on(ptr, 0, SENSORNUM, xtime_on, dd_on, ss_on, vv_on);

	delete[] ptr;
	return 0;
}

