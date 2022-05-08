#pragma once
#ifndef _utility_h
#define _utility_h

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define PATH_LEN				10000
#define PATH_WIDTH 			360
#define MOBILE_SPEED 		10
#define BATTERY 				1000
#define PEAK_POWER 			330
#define STORAGE 				10
#define SQ_UNIT 				0.001
#define SENSING_MAX 		0
#define TRIP						1
#define DELAY_TOLERATE	22
#define SLOT_LEN 				1
#define CEILING(x,y) (((x) + (y) - 1) / (y))
#define PATH_LOSS_EXPONENTIAL 2
#define HEIGHT					5
#define TURNRANGE				7
#define USPEED					TURNRANGE
#define TURN_COST				20
#define INFTY						(INT_MAX/2)
#define RANGE_LIMIT 		15
#define M								USPEED*AREA_LEN
#define AREA_LEN				200
#define AREA_WIDTH 			160

#define SEED						0
#define N 							<<sensor_number>>
#define E_UAV						<<uav_energy_budget>>
#define HARVESTING_MAX	<<max_harvest_energy>>

/*
 * distance/length:											1unit = 1m
 * energy/battery/harvesting/power:			1unit = 1mW
 * data/storage/sensing/rate:						1unit = 1Mbps
 * (Mbps) = 0.5 * log2(1+(mW)/(m)^2)
 */

#define max(a,b)																		\
   ({ __typeof__ (a) _a = (a);											\
       __typeof__ (b) _b = (b);											\
     _a > _b ? _a : _b; })

#define min(a,b)																		\
   ({ __typeof__ (a) _a = (a);											\
       __typeof__ (b) _b = (b);											\
     _a < _b ? _a : _b; })

#define alloc2d(type, var, x, y)										\
  do {                                              \
    (var) = calloc(x, sizeof(type *));              \
    if ((var) == NULL)															\
      perror("Error allocating memory.\n");					\
    for(int i = 0; i < x; i++) {										\
      (var)[i] = calloc(y, sizeof(type));						\
      if((var)[i] == NULL)													\
        perror("Error allocating memory.\n");				\
		}																								\
  } while(0)

#define free2d(var, x, y)														\
  do {                                              \
    for(int i = 0; i < x; i++)											\
      free((var)[i]);																\
    free(var);																			\
  } while(0)

struct sensor_t {
	int no;
	double x;
	double y;
	double battery;		//battery capacity
	double storage;		//data store capacity
	double peak_power;	//peak power constrain
	double energy;		//remain harvested energy
	struct storage_queue_t* qa;//the queue of remain sensed data
};

struct trans_range_t {
	int j_start;
	int j_end;
};

struct rmscf_t {
	int no;
	double value;
	char type;		// 'c'eiling or 'f'loor
};

int rmscf_value_compare(const void* a, const void* b);

int trans_range_determine(struct sensor_t* sensor, struct trans_range_t* tr);

int int_compare(const void* a, const void* b);

int sensor_x_compare(const void* a, const void* b);

#endif /* _utility_h */
