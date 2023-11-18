#include "kalmanFilter.h"

void KalmanFilterInit(KalmanFilter *kalman,float lastP,float q,float r)
{
   kalman->lastP=lastP;
	
   kalman->nowP=0;
 
   kalman->out=0;
	  
   kalman->kg=0;

   kalman->q=q;

   kalman->r=r;
}

float KalmanFilterCalculate(KalmanFilter *kalman,float input)
{
	
	kalman->nowP=kalman->lastP+kalman->q;
	
	kalman->kg=kalman->nowP/(kalman->nowP+kalman->r);
	
	kalman->out=kalman->out+kalman->kg*(input-kalman->out);

    kalman->lastP=(1-kalman->kg)*kalman->nowP;

    return kalman->out;
			
}

