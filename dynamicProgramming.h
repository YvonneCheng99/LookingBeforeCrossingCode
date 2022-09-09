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
	R = abs(z[0] - z[2])/2;
	return R;
}

//将有高度的节点的信息赋值给某一高度上节点的信息，便于调用一维算法计算
//如果height大于或等于该传感器的最大高度，那么返回false；否则，返回true
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

//打印数组
void printVector(vector<vector<double>>& v)
{
	for (auto& row : v)
	{
		for (auto& column : row)
		{
			cout << column << " ";
		}
		cout << endl;
	}
}
void printVector(vector<vector<int>>& v)
{
	for (auto& row : v)
	{
		for (auto& column : row)
		{
			cout << column << " ";
		}
		cout << endl;
	}
}

//sensor2D randomSensors()
//{
//	sensor2D* sensors = new sensor2D[SENSORNUM];
//
//}

double altitudeScheduling(sensor2D* sensors2D, double xtime, double maxHeight, double* location, int* sensorNumber, double* speed)
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
	vector<vector<double>> e_all(SENSORNUM + 1, vector<double>(K + 1));
	//存储到第n个传感器为止，最后的飞行高度为hk的情况下的段数
	vector<vector<int>> numOfPeriodVector(SENSORNUM + 1, vector<int>(K + 1));
	//存储每个GN对应的高度，不可以这么存，会被覆盖，存储最后高度为hk，第几段的高度
	vector<vector<double>> hFly(K + 1, vector<double>(SENSORNUM * 2));
	// 存储最后一段的高度为hk的情况下第几段的开始水平坐标
	vector<vector<double>> startCoordinate(K + 1, vector<double>(SENSORNUM * 3));
	//存储最后一段高度为hk的情况下第几段的结束水平坐标
	vector<vector<double>> endCoordinate(K + 1, vector<double>(SENSORNUM * 2));
	//存储最后一段高度为hk的情况下第几段的飞行速度
	vector<vector<double>> speedVector(K + 1, vector<double>(SENSORNUM * 2));

	//初始值，第一个GN的情况
	double p, e = 0;
	double t_min = sensors2D[1].dataAmount / V_DATA_TRANSMIT; //该GN所需的最小传输时间
	for (int i = 1; i <= K; i++)
	{
		if (h[i] < sensors2D[1].h_max)
		{
			double R = getR(h[i], sensors2D[1].b, sensors2D[1].c);
			double v = 2 * R / t_min; //保证该GN完成传输的最快的速度
			//如果速度比能耗最优的速度大，则增加时间，减小速度，来使得能耗最优
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
			
			if (DEBUG)
			{
				cout << "Sensor: 1, " << "Height: " << h[i] << " , Energy: " << e << endl;
				cout << "energy:----------" << endl;
				printVector(e_all);
				cout << "numOfPeriodVector:----------" << endl;
				printVector(numOfPeriodVector);
				cout << "hFly:----------" << endl;
				printVector(hFly);
				cout << "startCoordinate:----------" << endl;
				printVector(startCoordinate);
				cout << "endCoordinate:----------" << endl;
				printVector(endCoordinate);
				cout << "speedVector:----------" << endl;
				printVector(speedVector);
			}
		}
		else 
			e_all[1][i] = DBL_MAX;
		
	}
	//从第二个GN开始dp
	for (int i = 2; i <= SENSORNUM; i++)
	{
		if (DEBUG)
		{
			cout << "计算到第" << i << "个传感器为止----------------------------------" << endl;
		}
		for (int k = 1; k <= K; k++)
		{
			if (DEBUG)
			{
				cout << "第" << i << "个传感器的高度为: " << h[k] << endl;
			}
			if (h[k] >= sensors2D[i].h_max)
			{
				e_all[i][k] = DBL_MAX;
				break;
			}
			//计算采集到第i个传感器且最后高度为h的能耗
			double minEnergy = DBL_MAX;
			
			//对于第i个传感器之前的传感器，挨个遍历，找出高度变化最优的位置和变化范围
			//第j+1个传感器的高度开始为h[k]，第j个传感器的高度为h[kk]
			for (int j = 1; j < i; j++)
			{
				if (DEBUG)
				{
					cout << "j:" << j << endl;
				}
				int numOfPeriod = 0;
				sensor* s = new sensor[i - j];
				//初始化数组s和存储中间结果的数组们
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
				//计算水平飞行的能耗
				double energy_hori = 0;
				cout << "before----------------------" << endl;
				energy_hori = divide(s, 0, i - j, xtime, dd, ss, vv, numOfPeriod);
				cout << "after----------------------" << endl;
				for (int kk = 1; kk <= K; kk++)
				{
					if (h[kk] >= sensors2D[j].h_max) break;
					if (h[kk] < sensors2D[j].h_max && k != kk)
					{
						double delta_h = abs(h[k] - h[kk]);
						//如果内外层循环的高度相等，则看成一段水平路径，不计算，直接跳过
						if (delta_h <= EPSLION) continue;
						double energy_h = delta_h * HEIGHT_ENERGY_COEF;
						
						double temp = e_all[j][kk] + energy_h + energy_hori;
						// 如果变化高度前后的两个GN不重叠，需要加上在两个GN之间飞行的能耗
						// 看在其对应的飞行高度上第j个传感器的右边界与第j+1个传感器的左边界哪个更大判断是否有重叠
						double rightOfJ = sensors2D[j].position + getR(h[kk], sensors2D[j].b, sensors2D[j].c);
						double leftOfJ1 = sensors2D[j + 1].position - getR(h[k], sensors2D[j + 1].b, sensors2D[j + 1].c);
						if (rightOfJ < leftOfJ1)
						{
							double v = VBEST;
							p = 0.07 * v * v * v + 0.0391 * v * v - 13.196 * v + 390.95;
							double energy_between = p * (leftOfJ1 - rightOfJ) / v;
							temp += energy_between;
						}
						//能耗更小，需要更新最小值，并且更新保存下的中间数据的值
						if (DEBUG)
						{
							cout << "kk: " << kk << " ;energy: " << temp << endl;
						}
						if (temp < minEnergy)
						{
							minEnergy = temp;
							/*
							for (int m = j; m <= i; m++)
							{
								sensors2D[m].h_flight = h[kk];
								location[m] = dd[m - j];
								sensorNumber[m] = ss[m - j];
								speed[m] = vv[m - j];
							}
							*/
							//存储中间结果
							//先记录段数，之后根据段数记录其他的值
							numOfPeriodVector[i][k] = (rightOfJ < leftOfJ1) ? 
								numOfPeriod + numOfPeriodVector[j][kk] + 1 : numOfPeriod + numOfPeriodVector[j][kk];
							//什么意思 为什么从1到numOfPeriodVector[j][kk],
							for (int a = 1; a <= numOfPeriodVector[j][kk]; a++)
							{
								hFly[k][a] = hFly[kk][a];
								startCoordinate[k][a] = startCoordinate[kk][a];
								endCoordinate[k][a] = endCoordinate[kk][a];
								speedVector[k][a] = speedVector[kk][a];
							}
							//如果变化高度时空飞，高度怎么算
							//是不是有范围的重叠两种情况对应不同的赋值情况
							//如果范围没有重叠，首先给空飞的这一段赋值，然后段数下标+1,否则不做其他操作
							int m = numOfPeriodVector[j][kk] + 1;
							if (rightOfJ < leftOfJ1)
							{
								//先给空飞的这一段的各项赋值
								hFly[k][m] = h[kk];
								startCoordinate[k][m] = rightOfJ;
								endCoordinate[k][m] = leftOfJ1;
								speedVector[k][m] = VBEST;
								m = m + 1;
							}
							//给每一段的各个值赋值
							//要看bookingbefore算法的每一段怎么算的
							int a = 0;
							for (; m <= numOfPeriodVector[i][k]; m++)
							{
								//int a = (rightOfJ < leftOfJ1) ? m - numOfPeriodVector[j][kk] - 2 : m - numOfPeriodVector[j][kk] - 1;
								hFly[k][m] = h[k];
								startCoordinate[k][m] = dd[a];
								// endCoordinate[k][m] = dd[a + 1]
								speedVector[k][m] = vv[a];
								a++;
							}
							startCoordinate[k][numOfPeriodVector[i][k] + 1] = sensors2D[SENSORNUM].position + getR(hFly[k][SENSORNUM], sensors2D[SENSORNUM].b, sensors2D[SENSORNUM].c);
							if (DEBUG)
							{
								cout << "There is smaller energy! Change the minEnergy." << endl;
								cout << "energy:----------" << endl;
								printVector(e_all);
								cout << "numOfPeriodVector:----------" << endl;
								printVector(numOfPeriodVector);
								cout << "hFly:----------" << endl;
								printVector(hFly);
								cout << "startCoordinate:----------" << endl;
								printVector(startCoordinate);
								cout << "endCoordinate:----------" << endl;
								printVector(endCoordinate);
								cout << "speedVector:----------" << endl;
								printVector(speedVector);
							}
							
						}
					}
				}
				delete[]s;
				delete[]dd;
				delete[]ss;
				delete[]vv;
			}
			//如果整个过程中高度不变化
			int numOfPeriod = 0;
			sensor* s = new sensor[i];
			//初始化数组s和存储中间结果的数组们
			double* dd = new double[i * 3];
			int* ss = new int[i * 3];
			double* vv = new double[i * 3];
			bool judge = true;
			for (int p = 1; p <= i; p++)
			{
				judge = sensor2DToSensor(&sensors2D[p], &s[p - 1], h[k]);
				if (!judge) break;
			}
			//如果当前高度所有传感器都可以传输数据
			if (judge)
			{
				double energyAllHori = divide(s, 0, i, xtime, dd, ss, vv, numOfPeriod);
				if (energyAllHori < minEnergy)
				{
					minEnergy = energyAllHori;
					numOfPeriodVector[i][k] = numOfPeriod;
					for (int m = 1; m <= numOfPeriod; m++)
					{
						hFly[k][m] = h[k];
						startCoordinate[k][m] = dd[m];
						speedVector[k][m] = vv[m];
					}
					//最后一个传感器在最后一段的高度下的右边界
					startCoordinate[k][numOfPeriod + 1] = sensors2D[SENSORNUM].position + getR(hFly[k][SENSORNUM], sensors2D[SENSORNUM].b, sensors2D[SENSORNUM].c);
				}
				if (DEBUG)
				{
					cout << "energy:----------" << endl;
					printVector(e_all);
					cout << "numOfPeriodVector:----------" << endl;
					printVector(numOfPeriodVector);
					cout << "hFly:----------" << endl;
					printVector(hFly);
					cout << "startCoordinate:----------" << endl;
					printVector(startCoordinate);
					cout << "endCoordinate:----------" << endl;
					printVector(endCoordinate);
					cout << "speedVector:----------" << endl;
					printVector(speedVector);
				}
			}
			delete[]s;
			delete[]dd;
			delete[]ss;
			delete[]vv;
			e_all[i][k] = minEnergy;
		}
	}
	int bestK = -1;
	double result = DBL_MAX;
	for (int i = 1; h[i] < sensors2D[SENSORNUM].h_max; i++)
	{
		if (result > e_all[SENSORNUM][i])
		{
			result = e_all[SENSORNUM][i];
			sensors2D[SENSORNUM].h_flight = h[i];
			bestK = i;
		}
	}
	startCoordinate[bestK][numOfPeriodVector[SENSORNUM][bestK] + 1] = 
		sensors2D[SENSORNUM].position + getR(h[bestK], sensors2D[SENSORNUM].b, sensors2D[SENSORNUM].c);

	if (DEBUG)
	{
		cout << "hFly," << "startCoordinate," << "endCoordinate," << "speedVector:" << endl;
		for (int i = 1; i <= numOfPeriodVector[SENSORNUM][bestK]; i++)
		{
			cout << hFly[bestK][i] << " ," << startCoordinate[bestK][i] << " ," << startCoordinate[bestK][i + 1] << "," << speedVector[bestK][i] << endl;
		}
	}

	if (DEBUG)
	{
		ofstream file("test.csv");
		if (file)
		{
			cout << "hFly," << "startCoordinate," << "endCoordinate," << "speedVector:" << endl;
			for (int i = 1; i <= numOfPeriodVector[SENSORNUM][bestK]; i++)
			{
				file << hFly[bestK][i] << " ," << startCoordinate[bestK][i] << " ," << startCoordinate[bestK][i + 1] << "," <<speedVector[bestK][i] << "\n";
				cout << hFly[bestK][i] << " ," << startCoordinate[bestK][i] << " ," << startCoordinate[bestK][i + 1] << "," << speedVector[bestK][i] << endl;
			}
		}
		file.close();
	}
	return result;
}
