#ifndef LED_H
#define LED_H

#include "stdio.h"
#include "Arduino.h"
#include "freertos/FreeRTOS.h"//RTOS头文件
#include "freertos/task.h"    //RTOS头文件

extern bool init_ok;
extern uint32_t VBAT_Value;

//引脚定义
#define LED1 GPIO_NUM_16	//飞行状态指示灯（绿色）
#define LED2 GPIO_NUM_17	//电量指示灯（白色）
#define LED3 GPIO_NUM_18	//连接指示灯（蓝色）



void LED_Init(void);

#endif
