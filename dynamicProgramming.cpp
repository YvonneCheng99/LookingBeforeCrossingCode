#include <iostream>
#include <math.h>
#include <vector>
#include "parameters.h"
#include "sensor.h"
using namespace std;

const double HEIGHT = 10;
const double DELAT_H = 0.1;
double* h;

double altitudeScheduling()
{
	//��ɢ��H
	int K = ceil(HEIGHT / DELAT_H);
	if (K > 0) h = new double[K];
	else h = new double[1];
	for (int i = 0; i <= K; i++)
	{
		h[i] = i * DELAT_H;
	}

	//��̬�滮
	//�洢�ܺ�ֵ������
	vector<vector<double>> e_all(SENSORNUM, vector<double>(K));
	//�洢ÿ��GN��Ӧ�ĸ߶�
	vector<double> h_fly(SENSORNUM);
	//��ʼֵ
	for (int i = 1; i < K; i++)
	{
		e_all[1][K] = 0;
	}
	for (int i = 2; i < SENSORNUM; i++)
	{
		for (int k = 1; k < K; k++)
		{

		}
	}
	return 0.0;
}

//��������ģ�ͻ�ȡ��Ӧ�߶ȵĿɴ������ݷ�Χ
double getTransmittableRange(int sennum, int h)
{
	return 0.0;
}

//��ȡ��Ӧ�ٶȵ��ܺ�ֵ
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

/*
int mian()
{
	sensor* sensors = new sensor[SENSORNUM];
	//�����ʼ��GN��s��f��ֵ
	randam(sensors, SENSORNUM);
	for (int i = 0; i < SENSORNUM; i++)
	{
		cout << i << "\t";
		(sensors + i)->show();
	}
	return 0;
}
*/