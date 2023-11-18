#include "kalman.h"
#include <stdio.h>
#include "stdlib.h"


unsigned long KalmanFilter(unsigned long inData, volatile float *kalman)
{
	//上次卡尔曼值(估计出的最优值)
     
	//估计误差
    static float Eest = 100;
	//process_noise:过程噪声
    float process_noise = 0.001; 
	//r:测量误差
    float Emea = 0.01; 
    float kg = 0; //kg:卡尔曼增益
 
    Eest += process_noise;
    kg = Eest / ( Eest + Emea ); //计算卡尔曼增益
    *kalman = *kalman + (kg * (inData - *kalman)); //计算本次滤波估计值
    Eest = (1 - kg) * Eest; //更新测量方差
    
    return (unsigned long)(*kalman); //返回估计值
}

