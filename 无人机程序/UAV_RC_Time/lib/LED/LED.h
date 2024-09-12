#ifndef LED_H
#define LED_H

#include "stdio.h"
#include "Arduino.h"
#include "freertos/FreeRTOS.h"//RTOS头文件
#include "freertos/task.h"    //RTOS头文件

extern bool init_ok;

//引脚定义
#define LED1 GPIO_NUM_37	//飞行状态指示灯（绿色）
#define LED2 GPIO_NUM_36	//电量指示灯（白色）
#define Buzzer_Pin GPIO_NUM_35

enum {LED_Buzzer_on = 0, LED_Buzzer_off = 1};

void LED_Init(void);
void LED_Buzzer(uint8_t Pin,bool OnOff);

#endif
