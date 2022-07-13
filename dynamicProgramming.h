#pragma once
#include <iostream>
#include <vector>
#include <gsl/gsl_math.h>
#include <gsl/gsl_poly.h>
#include <cmath>
#include <float.h>
#include <fstream>
#include <istream>
#include "sensor.h"
#include "sensor2D.h"
#include "calculate.h"
using namespace std;

//���ݸ߶Ȼ�ȡ���˻��ɽ������ݷ�Χ�İ뾶
double getR(double h, double b, double c)
{
	double R = 0.0;
	double a[5] = { pow(h,4) - b * pow(h,3), 0,
		2 * h * h - b * h + c, 0, 1 };
	double z[8];
	gsl_poly_complex_workspace* w = gsl_poly_complex_workspace_alloc(5);
	gsl_poly_complex_solve(a, 5, w, z);
	gsl_poly_complex_workspace_free(w);
	R = abs(z[0] - z[2]);
	return R;
}

//���и߶ȵĽڵ����Ϣ��ֵ��ĳһ�߶��Ͻڵ����Ϣ�����ڵ���һά�㷨����
//���height���ڻ���ڸô����������߶ȣ���ô����false�����򣬷���true
bool sensor2DToSensor(sensor2D* s2D, sensor* s, double height)
{
	double R = getR(height, s2D->b, s2D->c);
	double posi = s2D->position;
	s->position = posi;
	s->start = posi - R;
	s ->finish = posi + R;
	s->time = s2D->dataAmount / V_DATA_TRANSMIT;
	if (height >= s2D->h_max) return false;
	return true;
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

double altitudeScheduling(sensor2D* sensors2D, double xtime, double maxHeight, double* location, int* sensorNumber, double* speed)
{
	//��ɢ��H
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
	
	//��̬�滮
	//�洢�ܺ�ֵ������
	vector<vector<double>> e_all(SENSORNUM + 1, vector<double>(K + 1));
	//�洢����n��������Ϊֹ�����ķ��и߶�Ϊhk������µĶ���
	vector<vector<int>> numOfPeriodVector(SENSORNUM + 1, vector<int>(K + 1));
	//�洢ÿ��GN��Ӧ�ĸ߶ȣ���������ô�棬�ᱻ���ǣ��洢���߶�Ϊhk���ڼ��εĸ߶�
	vector<vector<double>> hFly(K + 1, vector<double>(SENSORNUM * 2));
	// �洢���һ�εĸ߶�Ϊhk������µڼ��εĿ�ʼˮƽ����
	vector<vector<double>> startCoordinate(K + 1, vector<double>(SENSORNUM * 2));
	//�洢���һ�θ߶�Ϊhk������µڼ��εĽ���ˮƽ����
	vector<vector<double>> endCoordinate(K + 1, vector<double>(SENSORNUM * 2));
	//�洢���һ�θ߶�Ϊhk������µڼ��εķ����ٶ�
	vector<vector<double>> speedVector(K + 1, vector<double>(SENSORNUM * 2));

	//��ʼֵ����һ��GN�����
	double p, e = 0;
	double t_min = sensors2D[1].dataAmount / V_DATA_TRANSMIT; //��GN�������С����ʱ��
	for (int i = 1; i <= K; i++)
	{
		if (h[i] < sensors2D[1].h_max)
		{
			double R = getR(h[i], sensors2D[1].b, sensors2D[1].c);
			double v = 2 * R / t_min; //��֤��GN��ɴ���������ٶ�
			//����ٶȱ��ܺ����ŵ��ٶȴ�������ʱ�䣬��С�ٶȣ���ʹ���ܺ�����
			if (v > VBEST)
			{
				v = VBEST;
			}
			p = 0.07 * v * v * v + 0.0391 * v * v - 13.196 * v + 390.95;
			e = p * 2 * R / v;
			e_all[1][i] = e;
			numOfPeriodVector[1][i] = 1;
			hFly[i][1] = h[i];
			startCoordinate[i][1] = sensors2D[1].position - R;
			endCoordinate[i][1] = sensors2D[1].position + R;
			speedVector[i][1] = v;
		}
		else 
			e_all[1][i] = DBL_MAX;
		
	}
	//�ӵڶ���GN��ʼdp
	for (int i = 2; i <= SENSORNUM; i++)
	{
		if (DEBUG)
		{
			cout << "i:" << i << "----------------------------------" << endl;
		}
		for (int k = 1; k <= K; k++)
		{
			if (DEBUG)
			{
				cout << "k:" << k << endl;
			}
			if (h[k] >= sensors2D[i].h_max)
			{
				e_all[i][k] = DBL_MAX;
				continue;
			}
			//����ɼ�����i�������������߶�Ϊh���ܺ�
			double minEnergy = DBL_MAX;
			
			//���ڵ�i��������֮ǰ�Ĵ������������������ҳ��߶ȱ仯���ŵ�λ�úͱ仯��Χ
			//��j+1���������ĸ߶ȿ�ʼΪh[k]����j���������ĸ߶�Ϊh[kk]
			for (int j = 1; j < i; j++)
			{
				if (DEBUG)
				{
					cout << "j:" << j << endl;
				}
				int numOfPeriod = 0;
				sensor* s = new sensor[i - j];
				//��ʼ������s�ʹ洢�м�����������
				double* dd = new double[(i - j) * 3];
				int* ss = new int[(i - j) * 3];
				double* vv = new double[(i - j) * 3];
				bool judge = true;
				for (int p = j + 1; p <= i; p++)
				{
					judge = sensor2DToSensor(&sensors2D[p], &s[p - j - 1], h[k]);
					if (!judge) break;
				}
				if (!judge && DEBUG) cout << "continue!" << endl;
				if (!judge) continue;
				if (DEBUG)
				{
					for (int a = 0; a < i - j; a++)
					{
						cout << "s" << a << ": " << "start-" << s[a].start << " finish-" << s[a].finish << " time-" << s[a].time << endl;
					}
				}
				//����ˮƽ���е��ܺ�
				double energy_hori = 0;
				energy_hori = divide(s, 0, i - j, xtime, dd, ss, vv, numOfPeriod);
				for (int kk = 1; kk <= K; kk++)
				{
					if (h[kk] < sensors2D[j].h_max && k != kk)
					{
						double delta_h = abs(h[k] - h[kk]);
						//��������ѭ���ĸ߶���ȣ��򿴳�һ��ˮƽ·���������㣬ֱ������
						if (delta_h <= EPSLION) continue;
						double energy_h = delta_h * HEIGHT_ENERGY_COEF;
						
						double temp = e_all[j][kk] + energy_h + energy_hori;
						// ����仯�߶�ǰ�������GN���ص�����Ҫ����������GN֮����е��ܺ�
						// �������Ӧ�ķ��и߶��ϵ�j�����������ұ߽����j+1������������߽��ĸ������ж��Ƿ����ص�
						double rightOfJ = sensors2D[j].position + getR(h[kk], sensors2D[j].b, sensors2D[j].c);
						double leftOfJ1 = sensors2D[j + 1].position - getR(h[k], sensors2D[j + 1].b, sensors2D[j + 1].c);
						if (rightOfJ < leftOfJ1)
						{
							double v = VBEST;
							p = 0.07 * v * v * v + 0.0391 * v * v - 13.196 * v + 390.95;
							double energy_between = p * (leftOfJ1 - rightOfJ) / v;
							temp += energy_between;
						}
						//�ܺĸ�С����Ҫ������Сֵ�����Ҹ��±����µ��м����ݵ�ֵ
						if (false)
						{
							cout << "kk: " << kk << " ;energy: " << temp << endl;
						}
						if (temp < minEnergy)
						{
							minEnergy = temp;
							for (int m = j; m <= i; m++)
							{
								sensors2D[m].h_flight = h[kk];
								location[m] = dd[m - j];
								sensorNumber[m] = ss[m - j];
								speed[m] = vv[m - j];
							}
							//�洢�м���
							//�ȼ�¼������֮����ݶ�����¼������ֵ
							numOfPeriodVector[i][k] = (rightOfJ < leftOfJ1) ? 
								numOfPeriod + numOfPeriodVector[j][kk] : numOfPeriod + numOfPeriodVector[j][kk] + 1;
							//����仯�߶�ʱ�շɣ��߶���ô��
							//�ǲ����з�Χ���ص����������Ӧ��ͬ�ĸ�ֵ���
							//�����Χû���ص������ȸ��շɵ���һ�θ�ֵ��Ȼ������±�+1,��������������
							int m = numOfPeriodVector[j][kk];
							if (rightOfJ < leftOfJ1)
							{
								//�ȸ��շɵ���һ�εĸ��ֵ
								hFly[k][m] = h[kk];
								startCoordinate[k][m] = rightOfJ;
								endCoordinate[k][m] = leftOfJ1;
								speedVector[k][m] = VBEST;
								m = numOfPeriodVector[j][kk] + 1;
							}
							//��ÿһ�εĸ���ֵ��ֵ
							for (; m < numOfPeriodVector[i][k]; m++)
							{
								int a = (rightOfJ < leftOfJ1) ? m - numOfPeriodVector[j][kk] - 1 : m - numOfPeriodVector[j][kk];
								hFly[k][m] = h[kk];
								startCoordinate[k][m] = dd[a];
								// endCoordinate[k][m] = dd[a + 1]
								speedVector[k][m] = vv[a];
							}
							if (false)
							{
								cout << "There is smaller energy! Change the minEnergy." << endl;
							}
							
						}
					}
				}
				delete[]s;
				delete[]dd;
				delete[]ss;
				delete[]vv;
			}
			e_all[i][k] = minEnergy;
		}
	}
	int bestK = -1;
	double result = DBL_MAX;
	for (int i = 1; i <= K; i++)
	{
		if (result > e_all[SENSORNUM][i])
		{
			result = e_all[SENSORNUM][i];
			sensors2D[SENSORNUM].h_flight = h[i];
			bestK = i;
		}
	}
	startCoordinate[bestK][numOfPeriodVector[SENSORNUM][bestK]] = 
		sensors2D[SENSORNUM].position + getR(h[bestK], sensors2D[SENSORNUM].b, sensors2D[SENSORNUM].c);

	if (DEBUG)
	{
		ofstream file("test.csv");
		if (file)
		{
			cout << "hFly," << "startCoordinate," << "endCoordinate," << "speedVector:" << endl;
			for (int i = 0; i < numOfPeriodVector[SENSORNUM][bestK] - 1; i++)
			{
				file << hFly[bestK][i] << " ," << startCoordinate[bestK][i] << " ," << startCoordinate[bestK][i + 1] << "," <<speedVector[bestK][i] << "\n";
				cout << hFly[bestK][i] << " ," << startCoordinate[bestK][i] << " ," << startCoordinate[bestK][i + 1] << "," << speedVector[bestK][i] << endl;
			}
		}
		file.close();
	}
	return result;
}
