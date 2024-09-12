#include "LED.h"


void led_Task(void *pvParameter)
{
	bool temp_1 = false;
	int temp_val_1 = 0;
	TickType_t adp = xTaskGetTickCount();
	const TickType_t adg = 50;
	while(1)
	{
		vTaskDelayUntil(&adp,adg);
		if(init_ok)
		{
			
		}
	}
}

void LED_Init(void)
{
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(Buzzer_Pin, OUTPUT);
    digitalWrite(Buzzer_Pin, 1);
    digitalWrite(LED1, 1);
	digitalWrite(LED2, 1);
	vTaskDelay(pdMS_TO_TICKS(500));//延时
	
	digitalWrite(LED1, 0);
	vTaskDelay(pdMS_TO_TICKS(500));//延时
	digitalWrite(LED2, 0);
	vTaskDelay(pdMS_TO_TICKS(500));//延时

	digitalWrite(LED1, 1);
	digitalWrite(LED2, 1);
	vTaskDelay(pdMS_TO_TICKS(1000));//延时
    xTaskCreate(led_Task, "led_Task", 1024 * 5, NULL, 0, NULL); //创建任务，判断状态改变灯的亮灭
}
void LED_Buzzer(uint8_t Pin,bool OnOff)
{
    digitalWrite(Pin, OnOff);
}
