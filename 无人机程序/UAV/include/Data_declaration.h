#ifndef DATA_DECLARATION_H_
#define DATA_DECLARATION_H_

#include <stdio.h>
#include <stdbool.h>
#include "string.h"
#include "math.h"
#include "freertos/FreeRTOS.h"//RTOS头文件
#include "freertos/task.h"    //RTOS头文件
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "PID.h"
#include "My_WIFI.h"
#include "My_espnow.h"
#include "myMPU6050.h"

//PWM输出限幅值
#define PWM_MAX	899
#define PWM_MIN -899

//平衡角度中值
#define Yaw_Target		180
#define Roll_Target		0
#define Pitch_Target	0

//初始化标志位
extern bool init_ok;

//电压检测数据
extern uint32_t VBAT_Value;
//陀螺仪数据
extern MPU6050_Data My_MPU6050_Data;
//PID参数
extern PID_Demo PID_Yaw,PID_Pitch,PID_Roll;
extern int16_t Yaw_loop_out,Pitch_loop_out,Roll_loop_out;
//航偏角输出
extern float Yaw_kp;		
extern float Yaw_ki;	
extern float Yaw_kd;		
extern float Yaw_imax;//误差积分项阈值
//俯仰角输出
extern float Pitch_kp;		
extern float Pitch_ki;	
extern float Pitch_kd;		
extern float Pitch_imax;//误差积分项阈值
//横滚角输出
extern float Roll_kp;		
extern float Roll_ki;	
extern float Roll_kd;		
extern float Roll_imax;//误差积分项阈值
//PWM输出数据
extern int16_t M1_PWM,M2_PWM,M3_PWM,M4_PWM;

extern WIFI MyWiFi;

//互斥信号句柄
extern QueueHandle_t metux_semhore_handle;

#endif
