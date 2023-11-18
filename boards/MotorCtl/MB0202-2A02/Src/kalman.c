#include "kalman.h"
#include <stdio.h>
#include "stdlib.h"


unsigned long KalmanFilter(unsigned long inData, volatile float *kalman)
{
	//�ϴο�����ֵ(���Ƴ�������ֵ)
     
	//�������
    static float Eest = 100;
	//process_noise:��������
    float process_noise = 0.001; 
	//r:�������
    float Emea = 0.01; 
    float kg = 0; //kg:����������
 
    Eest += process_noise;
    kg = Eest / ( Eest + Emea ); //���㿨��������
    *kalman = *kalman + (kg * (inData - *kalman)); //���㱾���˲�����ֵ
    Eest = (1 - kg) * Eest; //���²�������
    
    return (unsigned long)(*kalman); //���ع���ֵ
}

