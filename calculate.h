#pragma once
#include"sensor.h"
#include<iostream>
#include <algorithm>
#include"assert.h"
#include"parameters.h"
using namespace std;
double vbest = 13.99; //最节能的速度

//一段有范围重叠的节点
double cal(sensor s[], double xtime, int start, int finish, double dd[], int ss[], double vv[], int& ii) {
	if (DEBUG)
		cout << "calculate! " << start << "," << finish << endl;
	double* tim = new double[SENSORNUM * 2];
	//原点坐标
	double alphax = xtime;
	double alphay = s[start].start;

	dd[ii] = s[start].start;
	ss[ii] = start - 1;
	ii++;

	//如果只有一个房间
	if (start == finish) {
		s[start].v = (s[start].finish - alphay) / s[start].time;
		double tt = s[start].time;
		//如果速度比能耗最优的速度大，则增加时间，减小速度，来使得能耗最优
		if (s[start].v > vbest) {
			s[start].v = vbest;
			tt = tt * s[start].v / vbest;
		}
		xtime += tt;
		dd[ii] = s[start].finish;
		ss[ii] = finish;
		vv[ii - 1] = s[start].v;
		ii++;
		delete[] tim;
		return xtime;
	}
	//if (s[start].k <= 0) {
	s[start].k = (s[start].finish - alphay) / (s[start].time);//斜率，北门口的斜率
	s[start].j = (s[start + 1].start - alphay) / (s[start].time);//斜率，南门口的斜率

//}

	s[start].calx = xtime + s[start].time;
	int kmin = start;
	int jmax = start;
	for (int i = start; i < finish + 1; ) {

		for (int p = i + 1; p < finish + 1; p++) {
			if (p != finish) {

				//对于每个传感器，每次向前看一个房间，如果条件允许就继续迭代（更新范围），否则确定速率V，更新原点，跳出循环
				//首先计算斜率
				s[p].calx = s[p - 1].calx + s[p].time;
				s[p].k = (s[p].finish - alphay) / (s[p].calx - alphax);
				s[p].j = (s[p + 1].start - alphay) / (s[p].calx - alphax);
				//比较可行范围做判断
				//若不符合要求，则确定kmin/jmax房间的速度v，以及前面一段的待定房间的速度，更新边界值、calx和原点，跳出循环；
				if (s[p].j > s[kmin].k) {


					s[kmin].v = s[kmin].k;
					vv[ii - 1] = s[kmin].k;
					alphax = s[kmin].calx;
					alphay = s[kmin].finish;
					//更新速率
					s[kmin + 1].k = (s[kmin + 1].finish - alphay) / (s[kmin + 1].time);
					s[kmin + 1].j = (s[kmin + 2].start - alphay) / (s[kmin + 1].time);

					kmin = kmin + 1;
					jmax = kmin;
					i = kmin;
					break;

				}
				else if (s[p].k < s[jmax].j) {

					s[jmax].v = s[jmax].j;
					vv[ii - 1] = s[jmax].j;
					alphax = s[jmax].calx;
					alphay = s[jmax + 1].start;

					//更新速率
					s[jmax + 1].k = (s[jmax + 1].finish - alphay) / (s[jmax + 1].time);
					s[jmax + 1].j = (s[jmax + 2].start - alphay) / (s[jmax + 1].time);
					jmax = jmax + 1;
					kmin = jmax;
					i = jmax;

					break;
				}
				//如果可行，原点不变，更新范围（最小最大值），继续往后看；
				else
				{
					kmin = s[kmin].k < s[p].k ? kmin : p;
					jmax = s[jmax].j > s[p].j ? jmax : p;

				}
			}
			else {
				s[p].calx = s[p - 1].calx + s[p].time;
				double t = (s[p].finish - alphay) / (s[p].calx - alphax);
				if (t >= s[kmin].k) {
					s[kmin].v = s[kmin].k;//如果是倒数第二个房间
					if (kmin == p - 1)
					{
						vv[ii - 1] = s[kmin].k;
						dd[ii] = s[p - 1].finish;
						ss[ii] = kmin;
						ii++;

						s[p].v = (s[p].finish - s[p - 1].finish) / (s[p].time);
						vv[ii - 1] = s[p].v;
						i = finish + 1;
						alphay = s[p].finish;
						break;
					}
					else {
						//更新速率

						s[kmin].v = s[kmin].k;
						vv[ii - 1] = s[kmin].v;
						alphax = s[kmin].calx;
						alphay = s[kmin].finish;

						s[kmin + 1].k = (s[kmin + 1].finish - alphay) / (s[kmin + 1].time);
						s[kmin + 1].j = (s[kmin + 2].start - alphay) / (s[kmin + 1].time);

						kmin = kmin + 1;
						jmax = kmin;
						i = kmin;

						break;

					}


				}
				else
				{
					if (t < s[kmin].k && t >= s[jmax].j) {
						s[p].v = t;
						s[kmin].v = t;
						i = finish + 1;
						vv[ii - 1] = s[p].v;
						alphay = s[p].finish;
						break;
					}
					else {
						//如果是倒数第二个房间
						s[jmax].v = s[jmax].j;
						if (jmax == p - 1) {
							s[jmax].v = s[jmax].j;
							vv[ii - 1] = s[jmax].j;
							dd[ii] = s[p].start;
							ss[ii] = jmax;
							ii++;

							s[p].v = (s[p].finish - s[p].start) / s[p].time;
							vv[ii - 1] = s[p].v;



							i = finish + 1;
							alphay = s[p].finish;

							break;
						}
						else
						{
							//如果没到倒数第二的房间
							s[jmax].v = s[jmax].j;
							vv[ii - 1] = s[jmax].j;
							alphax = s[jmax].calx;
							alphay = s[jmax + 1].start;

							//更新速率
							s[jmax + 1].k = (s[jmax + 1].finish - alphay) / (s[jmax + 1].time);
							s[jmax + 1].j = (s[jmax + 2].start - alphay) / (s[jmax + 1].time);

							jmax = jmax + 1;
							kmin = jmax;
							i = jmax;

							break;
						}

					}

				}

			}
		}
		dd[ii] = alphay;
		ss[ii] = i - 1;
		ii++;

	}
	double temp = 0;
	for (int i = finish; i >= start; i--)
	{
		tim[i - start] = s[i].time;
		if (s[i].v > 0) {

			if (s[i].v > VBEST)
			{
				tim[i - start] *= (s[i].v / VBEST);
				s[i].v = VBEST;

			}
			temp = s[i].v;


		}
		else {
			s[i].v = temp;

		}
	}

	for (int i = start; i < finish + 1; i++) {

		//先计算时间，再输出
		xtime += tim[i - start];
		//		cout << i << "   " << s[i].v << "   " << xtime <<endl;

	}
	delete[] tim;

	return xtime;
}

double divide(sensor s[], int first, int last, double& x, double dd[], int ss[], double vv[])
{

	double xtime = x;
	int start = first;
	int finish = first;
	int ii = 0;
	dd[start] = s[start].start;
	for (int i = first + 1; i < last; i++) {
		//如果重叠则继续往后看

		if (s[i].start < s[i - 1].finish) {//如果范围有重叠（交叉
			finish++;
			if (finish == last - 1) {//如果是最后一个
				xtime = cal(s, xtime, start, finish, dd, ss, vv, ii);
				xtime += (s[i].start - s[i - 1].finish) / vbest;

			}
		}
		else//不重叠就断开，生成新的数组
		{
			xtime = cal(s, xtime, start, finish, dd, ss, vv, ii);
			vv[ii - 1] = vbest;
			start = i;
			finish = start;
			if (finish == last - 1) {
				xtime = cal(s, xtime, start, finish, dd, ss, vv, ii);
			}
			//计算空转时间
			xtime += (s[i].start - s[i - 1].finish) / vbest;
		}
	}

	if (DEBUG)
		cout << "Offline scheduling:" << endl;
	double e = energy_out(dd, ss, vv, ii);
	cout << "Offline energy:\t" << e << endl;

	x = xtime;
	//energy = ;
	return e;
}


void cal_on(sensor s[], int first, int last, double& xtime, double dd_on[], int ss_on[], double vv_on[])
{

	//声明在线数组变量（距离&速度）,和对应的指示物
	double dd[SENSORNUM * 2] = { 0.0 };
	int ss[SENSORNUM * 2] = { 0 };
	double vv[SENSORNUM * 2] = { 0.0 };
	int ii = 0;//离线数组指针
	int jj = 0;//在线数组指针

	double time_on = xtime;
	//声明两个指示变量，指向每个局部的开始和结尾
	int winds = first;
	int windf = winds;
	bool runflag;

	//声明重新计算点的开始距离
	double sofarx = s[winds].start;
	int t = winds;
	//分家族，之后确定重新计算点
	for (int i = 1; i <= last; i++) {
		runflag = false;
		if (i < last && s[i].start < s[i - 1].finish) {
			windf++;
			if (s[windf].position - s[winds].start <= VISION) {
				t = windf;
			}
		}
		else
			runflag = true;

		if (runflag == true)//i现在指向下一段的开始
		{
			//开始在线计算
			double recall[SENSORNUM * 2] = { -1.0 };//
			recall[0] = s[winds].start;

			for (int j = 1; j <= windf - t; j++) {
				//找出所有的重新计算点
				//重新计算点的位置
				recall[j] = s[t + j].position - VISION;

			}
			recall[windf - t + 1] = 2 * DIS;

			//初始化,记录下本段第一个d0的信息，V0先不记录，在第一次调用离线算法时求得;
			dd_on[jj] = s[winds].start;

			jj++;
			int t0 = winds;//每次离线算法的开头
			int k;

			//之后从第一个重新计算点开始，先计算视距之内的离线结果，再不断更新重新计算点之后的结果
			//并且将已经飞过的路程存入最终结果数组中
			for (int j = 0; recall[j] >= 0 && recall[j] < DIS; j++) {
				//记录当前ii的值
				ii = 0;

				//在每个重新计算点都要先算一遍离线算法
				if (DEBUG)
					cout << j << "=j\tNextRecall=" << recall[j + 1] << endl;
				time_on = cal(s, time_on, t0, t, dd, ss, vv, ii);
				if (DEBUG)
					for (int i = 0; i < ii; i++) {
						cout << dd[i] << "\t" << ss[i] << "\t" << vv[i] << endl;
					}


				//先确定是哪个房间
				double time = 0;
				for (k = 1; k < ii; k++) {

					if (dd[k] > recall[j + 1]) {
						dd_on[jj] = recall[j + 1];
						vv_on[jj - 1] = vv[k - 1];
						jj++;

						time = (recall[j + 1] - dd[k - 1]) / vv[k - 1];
						int p = 0;
						for (p = ss[k - 1] + 1; p <= ss[k]; p++) {
							time -= s[p].time;
							if (time < 0) {
								break;
							}
						}

						sofarx = recall[j + 1];
						if (time < 0)
							s[p].time = 0 - time;
						t0 = p;
						t = p;
						for (; t <= windf; t++) {
							if (s[t].position - sofarx > VISION + 1e-6) {

								break;
							}
							if (s[t].start < recall[j + 1]) {
								s[t].start = recall[j + 1];
							}
						}
						t--;
						break;
					}
					else {
						dd_on[jj] = dd[k];
						vv_on[jj - 1] = vv[k - 1];
						jj++;

					}


				}





				//更新后一段的winds,windf以及各个传感器的范围
			}



			//winds 和windf
			winds = i;
			windf = winds;
			t = winds;
			vv_on[jj - 1] = VBEST;

		}


	}
	if (DEBUG)
		cout << "Online scheduling:" << endl;
	double e = energy_out(dd_on, ss_on, vv_on, jj);
	cout << "Online energy:\t" << e << endl;


}

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