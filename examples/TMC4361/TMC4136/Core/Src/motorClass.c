#include "motorClass.h"

typedef enum
{
    STEPMOTOR
}MotorKind;

typedef enum
{
    NONE,
    TMC4361
}DrivingMethod;

typedef enum
{
  //  NONE,
    ERR_1
}MotorRtCode;



typedef enum
{
  POSITION_MODE,//定位模式
  SPEED_MODE//速度模式
}MoveMode;

typedef struct 
{
    /* data */
    MoveMode moveMode;
}MotorCfg;

typedef struct 
{
    uint32_t outTime;

}MotorTask;

#define MOTOR_MAX_TASKS 10
typedef struct motorClass
{
   MotorKind motorKind;//电机种类
   DrivingMethod driving;//驱动的方式
   void * driveHdObject;//驱动配置实例
   MotorCfg cfg;//电机配置
   MotorTask task[MOTOR_MAX_TASKS];//电机任务列表

}MotorClass;









MotorRtCode MotorActionUntilEventTrigger()
{

}


void MotorTaskRun(MotorTask *task)
{

}
void MotorThread(void *motor)
{
   MotorClass *motorObject=(MotorClass*) motor;

   while (1)
   {

        //pend信号量 
        if( motorObject->task[0].outTime!=0)
        {
            MotorTaskRun(motorObject->task);
        }

   }
   
    
   
}

