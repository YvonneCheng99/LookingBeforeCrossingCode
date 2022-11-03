#pragma once
#define DEBUG 0
#define VBEST 13.99
#define VISION 50
#define DIS 15
#define SENSORNUM   4
#define DRANGE			50
#define TAU					20
#define HEIGHT_ENERGY_COEF	2.3 //能耗对应高度变化的系数
#define V_DATA_TRANSMIT  2 //数据传输的速度
#define DELTA_H 0.5 //高度离散化尺度
#define EPSLION 1e-8 //判断
#define SHAPE_COEF_LOW	0.5 //气球模型bc关系系数下界
#define SHAPE_COEF_HIGH	2.0 //气球模型bc关系系数上界
#define HEIGHT_BOUNDARY	20 //气球高度上界
#define DIS_HEGH_COEF 1.5
//extern int SENSORNUM = 0;