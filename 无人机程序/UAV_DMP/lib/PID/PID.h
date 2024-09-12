#ifndef __PID_H_
#define __PID_H_

#include "stdio.h"

typedef struct
{
	float                kp;         					//P
	float                ki;         					//I
	float                kd;         					//D
	float                imax;       					//积分限幅
	
	float                out_p;  		 				//KP输出
	float                out_i;  		 				//KI输出
	float                out_d;  		 				//KD输出
	float                out;    		 				//pid输出
	float				 last_out;	 					//pid上一次输出
	float				 err;	 						//pid当前输出与上一次输出的差值
	
	float                integrator; 					// 积分值
	float                last_err; 						// 上次误差
	float                last_derivative;				// 上次误差与上上次误差之差
	
	float                target;    				 	//  设置的期望值
}PID_Demo;

//PID初始化
void Pid_Init(PID_Demo *pid);
 
float PidIncCtrl(PID_Demo * pid, float actual_val);//pid增量式控制器输出
void PidSet(PID_Demo * pid, float p, float i, float d, float imax);//pid参数设置
void pidSetTarget(PID_Demo * pid, float target);//pid设置期望值
void pidClear(PID_Demo * pid);//pid清零
float constrain_float(float dat, float low, float high);//限幅函数


#endif
