#ifndef __CONTROL_H_
#define __CONTROL_H_

#include "stdio.h"
#include "freertos/FreeRTOS.h"//RTOS头文件
#include "freertos/task.h"    //RTOS头文件

void Init(void);
void Task_Create_Init(void);


void Data_Task(void *pvParameter);

#endif
