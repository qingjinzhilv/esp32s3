/*
 * Data declaration.c
 *  数据声明，结构体，堆栈空间，飞行器姿态和PID各类数据在此声明
 *  Created on: 2024年8月22日
 *      Author: 倾尽
 */

#include "Data_declaration.h"
#include <stdio.h>
#include <stdbool.h>
#include "string.h"
#include "math.h"

bool init_ok = false;//初始化成功后置1全部任务才会开始运行
bool unlock = false;

uint32_t VBAT_Value;  //电压

MPU6050_Data My_MPU6050_Data;

uint8_t key = 0;




