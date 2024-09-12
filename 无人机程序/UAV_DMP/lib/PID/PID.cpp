#include <stdio.h>
#include "PID.h"

/*************************************************************************
 *  函数名称：void Pid_Init(PID_Demo * pid)
 *  功能说明：PID初始化函数
 *  参数说明：
  * @param    pid   ： 参数
 *  函数返回：无
 *  备    注：
 *************************************************************************/
void Pid_Init(PID_Demo * pid)
{
    pid->kp        = 0;
    pid->ki        = 0;
    pid->kd        = 0;
    pid->imax      = 0;
    pid->out_p     = 0;
    pid->out_i     = 0;
    pid->out_d     = 0;
    pid->out       = 0;
	pid->last_out  = 0;
	pid->err  = 0;
    pid->integrator= 0;
    pid->last_err= 0;
    pid->last_derivative   = 0;
    pid->target    = 0;
}


/*************************************************************************
 *  函数名称：float PidIncCtrl(PID_Demo * pid, float actual_val)
 *  功能说明：pid增量式控制器输出
 *  参数说明：
  * @param    pid     	    pid参数
  * @param    actual_val    当前值
 *  函数返回：PID输出结果   注意输出结果已经包涵了上次结果
 *  备    注：
 *************************************************************************/
float PidIncCtrl(PID_Demo * pid, float actual_val)
{
    float error = pid->target - actual_val;//计算当前无误差
	
	// //消除抖动误差
	// if(error < 3 && error > -3)
	// 	error = 0;
	
    // pid->out_p = pid->kp * (error - pid->last_err);
    // pid->out_i = pid->ki * error;
    // pid->out_d = pid->kd * ((error - 2*pid->last_err) + pid->last_derivative);
 
    // pid->last_derivative = error - pid->last_err;
    // pid->last_err = error;
 
    // pid->out = pid->out_p + pid->out_i + pid->out_d;

    pid->integrator += error;
    pid->out_p = pid->kp * error;
    pid->last_derivative = error - pid->last_err;
    pid->out_i = pid->ki * pid->integrator;//pid->integrator;
    pid->out_d = pid->kd * pid->last_derivative;
    
    pid->last_err = error;

    if(pid->out_i>pid->imax)
		pid->out_i=pid->imax;
	else if(pid->out_i<-pid->imax)
		pid->out_i=-pid->imax;
 
    pid->out = pid->out_p + pid->out_i + pid->out_d;
	if(pid->out>1000)
	{
		pid->out=1000;
	}
	else if(pid->out<-1000)
	{
		pid->out=-1000;
	}
    return pid->out;
}

 
/*************************************************************************
 *  函数名称：void PidSet(PID_Demo * pid, float p, float i, float d, float imax)
 *  功能说明：pid参数设置
 *  参数说明：
  * @param    pid      pid参数
  * @param    p			   比例项
  * @param    i			   积分项
  * @param    d			   微分项
  * @param    imax		 积分上限
 *  函数返回：无   
 *  备    注：
 *************************************************************************/
void PidSet(PID_Demo * pid, float p, float i, float d, float imax)
{
    pid->kp = p;
    pid->ki = i;
    pid->kd = d;
    pid->imax = imax;
}
 
 
/*************************************************************************
 *  函数名称：void pidSetTarget(PID_Demo * pid, float t)
 *  功能说明：pid设置期望值
 *  参数说明：
  * @param    pid     	   pid参数
  * @param    t     	   pid期望值
 *  函数返回：无  
 *  备    注：
 *************************************************************************/
void pidSetTarget(PID_Demo * pid, float target)
{
    pid->target = target;
}
 
 
/*************************************************************************
 *  函数名称：void pidClear(PID_Demo * pid)
 *  功能说明：pid清零
 *  参数说明：
  * @param    pid     	   pid参数
 *  函数返回：无  
 *  备    注：
 *************************************************************************/
void pidClear(PID_Demo * pid)
{
    pid->integrator= 0;
    pid->last_err= 0;
    pid->last_derivative   = 0;
    pid->out = 0;
}
 
 
/*************************************************************************
 *  函数名称：float constrain_float(float amt, float low, float high)
 *  功能说明：限幅函数
 *  参数说明：
  * @param    amt   ： 参数
  * @param    low   ： 最低值
  * @param    high  ： 最高值
 *  函数返回：无
 *  备    注：
 *************************************************************************/
float constrain_float(float dat, float low, float high)
{
    if(dat<=low)
		return low;
	else if(dat>=high)
		return high;
	else
		return dat;
}

