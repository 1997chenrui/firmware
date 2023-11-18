#ifndef _KALMANFILTER_H_
#define _KALMANFILTER_H_



typedef struct kalmanFilter
{
	float nowP;//当前协方差
	float lastP;//上次协方差
	
	float out;//滤波器输出
	float kg;//卡尔曼增益
	
	float q;//过程噪音
	float r;//观测噪音
	
}KalmanFilter;


void KalmanFilterInit(KalmanFilter *kalman,float lastP,float q,float r);
float KalmanFilterCalculate(KalmanFilter *kalman,float input);
#endif

