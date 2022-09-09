//#include"sensor2D.h"
//#include"sensor.h"
//#include<chrono>
//#include<ctime>
//#include<stdlib.h>
//#include<iostream>
//#include <algorithm>
//using namespace std;
//
//
//void gettimeofday(struct timeval* tv)
//{
//	auto time_now = std::chrono::system_clock::now();
//	std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
//	auto duration_in_s =
//		std::chrono::duration_cast<std::chrono::seconds>(time_now.time_since_epoch()).count();
//	auto duration_in_us =
//		std::chrono::duration_cast<std::chrono::microseconds>(time_now.time_since_epoch()).count();
//
//	tv->tv_sec = duration_in_s;
//	tv->tv_usec = duration_in_us;
//}
//
//double myrandom(double min, double max) {
//	return rand() / (1.0 * RAND_MAX) * (max - min) + min;
//}
//
//bool randam(sensor2D* point, int NUM)
//{
//	int i, n = NUM;
//	long seed;
//	double delta;
//	double* rnda = new double[NUM];
//	struct timeval time;
//	gettimeofday(&time);
//
//	seed = (time.tv_sec * 1000) + (time.tv_usec / 1000);
//	seed = 1564584525524;
//	srand(seed);
//	cout << "Seed=\t" << seed << endl;
//
//	for (i = 0; i < n; i++) {
//		rnda[i] = myrandom(0, DIS - DRANGE);
//	}
//	sort(rnda, rnda + n);
//	// position
//	for (i = 0; i < n; i++) {
//		point[i].position = rnda[i];
//	}
//
//	gettimeofday(&time);
//	seed = (time.tv_sec * 1000) + (time.tv_usec / 1000);
//	srand(seed);
//	//c
//	for (i = 0; i < n; i++) {
//		point[i].c = myrandom(1, HEIGHT_BOUNDARY);
//		point[i].b = point[i].c * myrandom(SHAPE_COEF_LOW, SHAPE_COEF_HIGH);
//	}
//
//	//Êý¾ÝÁ¿
//	for (i = 0; i < n; i++) {
//		point[i].dataAmount = myrandom(1, HEIGHT_BOUNDARY / 2);
//	}
//	
//	
//	i = 1;
//	delta = point[i].position;
//	for (i = 1; i < n; i++) {
//		point[i].position -= delta;
//	}
//
//	return true;
//}

/*
bool randam(sensor* point, int NUM)
{
	int i, n = NUM;
	long seed;
	double delta;
	double* rnda = new double[NUM];
	struct timeval time;
	gettimeofday(&time);

	seed = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	seed = 1564584525524;
	srand(seed);
	cout << "Seed=\t" << seed << endl;

	for (i = 0; i < n; i++) {
		rnda[i] = myrandom(0, DIS - DRANGE);
	}
	sort(rnda, rnda + n);
	for (i = 0; i < n; i++) {
		point[i].start = rnda[i];
	}
	for (i = 0; i < n; i++) {
		rnda[i] = point[i].start + myrandom(1, DRANGE * 2 - 1);
	}
	sort(rnda, rnda + n);
	for (i = 0; i < n; i++) {
		if ((rnda[i] - point[i].start) / 2 > VISION)
			point[i].finish = point[i].start + 2 * VISION - 1e-1;
		else
			point[i].finish = rnda[i];

		point[i].position = (point[i].finish - point[i].start) / 2 + point[i].start;
		point[i].time = max((point[i].finish - point[i].start) / 30, myrandom(0, TAU * 2));
		// point[i].position=rnda[i];
		// point[i].finish=2*point[i].position-point[i].start;
		// point[i].time=myrandom(0, TAU*2);
	}
	i = 0;
	delta = point[i].start;
	point[i].start -= delta;
	point[i].position -= delta;
	point[i].finish -= delta;


	return true;
}
*/