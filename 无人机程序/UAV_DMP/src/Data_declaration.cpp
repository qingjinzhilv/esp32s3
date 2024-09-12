/*
 * Data declaration.c
 *  数据声明，结构体，堆栈空间，飞行器姿态和PID各类数据在此声明
 *  Created on: 2024年8月22日
 *      Author: 倾尽
 */

#include "Data_declaration.h"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "string.h"
#include "math.h"
#include "My_WIFI.h"

//初始化成功后置1全部任务才会开始运行
bool init_ok = false;
//遥控解锁
bool unlock = false;

uint32_t VBAT_Value;  //电压

MPU6050_Data My_MPU6050_Data;
//WiFi变量
WIFI MyWiFi;

//espnow通信协议相关数据
uint8_t __espnow_send_data[100] = {0};
uint8_t __espnow_Receive_data[100] = {0};
//PID参数
PID_Demo PID_Yaw,PID_Pitch,PID_Roll;
int16_t Yaw_loop_out = 0,Pitch_loop_out = 0,Roll_loop_out = 0;
//航偏角输出
float Yaw_kp = 0.0;
float Yaw_ki = 0;
float Yaw_kd = 0;
float Yaw_imax = 100;//误差积分项阈值
//俯仰角输出
float Pitch_kp = 0.08;
float Pitch_ki = 0.0;
float Pitch_kd = -0.01;
float Pitch_imax = 100;//误差积分项阈值
//横滚角输出
float Roll_kp = 0.08;
float Roll_ki = 0.0;
float Roll_kd = -0.01;
float Roll_imax = 100;//误差积分项阈值
//PWM控制输出
int16_t M1_PWM = 0,M2_PWM = 0,M3_PWM = 0,M4_PWM = 0;

//互斥信号句柄
QueueHandle_t metux_semhore_handle;


