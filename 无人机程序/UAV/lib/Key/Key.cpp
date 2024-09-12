#include <stdbool.h>
#include <stdio.h>
#include "Key.h"
#include "freertos/FreeRTOS.h"//RTOS头文件
#include "freertos/task.h"    //RTOS头文件

void Key_Init(void)
{
	pinMode(SW2_Pin,INPUT_PULLUP);
}

bool Key_ReadPin_IO(uint8_t gpio_num)
{
	bool flag = false;
	static bool flag_value = false;
	if(digitalRead(gpio_num) == 0)
	{
		vTaskDelay(pdMS_TO_TICKS(20));//延时
		if(digitalRead(gpio_num) == 0)
		{
			flag = true;
		}
	}
	else 
	{
		flag = false;
	}
	if(flag != flag_value)
	{
		flag_value = flag;
	}
	else 
	{
		flag = false;
	}
	return flag;
}
