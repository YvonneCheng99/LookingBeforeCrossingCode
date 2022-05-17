#pragma once
#include"sensor.h"
#include<iostream>
#include <algorithm>
#include"assert.h"
#include"parameters.h"
using namespace std;
double vbest = 13.99; //����ܵ��ٶ�

//һ���з�Χ�ص��Ľڵ�
double cal(sensor s[], double xtime, int start, int finish, double dd[], int ss[], double vv[], int& ii) {
	if (DEBUG)
		cout << "calculate! " << start << "," << finish << endl;
	double* tim = new double[SENSORNUM * 2];
	//ԭ������
	double alphax = xtime;
	double alphay = s[start].start;

	dd[ii] = s[start].start;
	ss[ii] = start - 1;
	ii++;

	//���ֻ��һ������
	if (start == finish) {
		s[start].v = (s[start].finish - alphay) / s[start].time;
		double tt = s[start].time;
		//����ٶȱ��ܺ����ŵ��ٶȴ�������ʱ�䣬��С�ٶȣ���ʹ���ܺ�����
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
	s[start].k = (s[start].finish - alphay) / (s[start].time);//б�ʣ����ſڵ�б��
	s[start].j = (s[start + 1].start - alphay) / (s[start].time);//б�ʣ����ſڵ�б��

//}

	s[start].calx = xtime + s[start].time;
	int kmin = start;
	int jmax = start;
	for (int i = start; i < finish + 1; ) {

		for (int p = i + 1; p < finish + 1; p++) {
			if (p != finish) {

				//����ÿ����������ÿ����ǰ��һ�����䣬�����������ͼ������������·�Χ��������ȷ������V������ԭ�㣬����ѭ��
				//���ȼ���б��
				s[p].calx = s[p - 1].calx + s[p].time;
				s[p].k = (s[p].finish - alphay) / (s[p].calx - alphax);
				s[p].j = (s[p + 1].start - alphay) / (s[p].calx - alphax);
				//�ȽϿ��з�Χ���ж�
				//��������Ҫ����ȷ��kmin/jmax������ٶ�v���Լ�ǰ��һ�εĴ���������ٶȣ����±߽�ֵ��calx��ԭ�㣬����ѭ����
				if (s[p].j > s[kmin].k) {


					s[kmin].v = s[kmin].k;
					vv[ii - 1] = s[kmin].k;
					alphax = s[kmin].calx;
					alphay = s[kmin].finish;
					//��������
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

					//��������
					s[jmax + 1].k = (s[jmax + 1].finish - alphay) / (s[jmax + 1].time);
					s[jmax + 1].j = (s[jmax + 2].start - alphay) / (s[jmax + 1].time);
					jmax = jmax + 1;
					kmin = jmax;
					i = jmax;

					break;
				}
				//������У�ԭ�㲻�䣬���·�Χ����С���ֵ�����������󿴣�
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
					s[kmin].v = s[kmin].k;//����ǵ����ڶ�������
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
						//��������

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
						//����ǵ����ڶ�������
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
							//���û�������ڶ��ķ���
							s[jmax].v = s[jmax].j;
							vv[ii - 1] = s[jmax].j;
							alphax = s[jmax].calx;
							alphay = s[jmax + 1].start;

							//��������
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

		//�ȼ���ʱ�䣬�����
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
		//����ص����������

		if (s[i].start < s[i - 1].finish) {//�����Χ���ص�������
			finish++;
			if (finish == last - 1) {//��������һ��
				xtime = cal(s, xtime, start, finish, dd, ss, vv, ii);
				xtime += (s[i].start - s[i - 1].finish) / vbest;

			}
		}
		else//���ص��ͶϿ��������µ�����
		{
			xtime = cal(s, xtime, start, finish, dd, ss, vv, ii);
			vv[ii - 1] = vbest;
			start = i;
			finish = start;
			if (finish == last - 1) {
				xtime = cal(s, xtime, start, finish, dd, ss, vv, ii);
			}
			//�����תʱ��
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

	//���������������������&�ٶȣ�,�Ͷ�Ӧ��ָʾ��
	double dd[SENSORNUM * 2] = { 0.0 };
	int ss[SENSORNUM * 2] = { 0 };
	double vv[SENSORNUM * 2] = { 0.0 };
	int ii = 0;//��������ָ��
	int jj = 0;//��������ָ��

	double time_on = xtime;
	//��������ָʾ������ָ��ÿ���ֲ��Ŀ�ʼ�ͽ�β
	int winds = first;
	int windf = winds;
	bool runflag;

	//�������¼����Ŀ�ʼ����
	double sofarx = s[winds].start;
	int t = winds;
	//�ּ��壬֮��ȷ�����¼����
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

		if (runflag == true)//i����ָ����һ�εĿ�ʼ
		{
			//��ʼ���߼���
			double recall[SENSORNUM * 2] = { -1.0 };//
			recall[0] = s[winds].start;

			for (int j = 1; j <= windf - t; j++) {
				//�ҳ����е����¼����
				//���¼�����λ��
				recall[j] = s[t + j].position - VISION;

			}
			recall[windf - t + 1] = 2 * DIS;

			//��ʼ��,��¼�±��ε�һ��d0����Ϣ��V0�Ȳ���¼���ڵ�һ�ε��������㷨ʱ���;
			dd_on[jj] = s[winds].start;

			jj++;
			int t0 = winds;//ÿ�������㷨�Ŀ�ͷ
			int k;

			//֮��ӵ�һ�����¼���㿪ʼ���ȼ����Ӿ�֮�ڵ����߽�����ٲ��ϸ������¼����֮��Ľ��
			//���ҽ��Ѿ��ɹ���·�̴������ս��������
			for (int j = 0; recall[j] >= 0 && recall[j] < DIS; j++) {
				//��¼��ǰii��ֵ
				ii = 0;

				//��ÿ�����¼���㶼Ҫ����һ�������㷨
				if (DEBUG)
					cout << j << "=j\tNextRecall=" << recall[j + 1] << endl;
				time_on = cal(s, time_on, t0, t, dd, ss, vv, ii);
				if (DEBUG)
					for (int i = 0; i < ii; i++) {
						cout << dd[i] << "\t" << ss[i] << "\t" << vv[i] << endl;
					}


				//��ȷ�����ĸ�����
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





				//���º�һ�ε�winds,windf�Լ������������ķ�Χ
			}



			//winds ��windf
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