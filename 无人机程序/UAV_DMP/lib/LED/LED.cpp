#include "LED.h"

void LED_Init(void)
{
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(LED3, OUTPUT);
	
	
	digitalWrite(LED1, 1);
	digitalWrite(LED2, 1);
	digitalWrite(LED3, 1);
	vTaskDelay(pdMS_TO_TICKS(500));//延时
	
	digitalWrite(LED3, 0);
	vTaskDelay(pdMS_TO_TICKS(500));//延时
	digitalWrite(LED1, 0);
	vTaskDelay(pdMS_TO_TICKS(500));//延时
	digitalWrite(LED2, 0);
	vTaskDelay(pdMS_TO_TICKS(500));//延时

	digitalWrite(LED1, 1);
	digitalWrite(LED2, 1);
	digitalWrite(LED3, 1);
	vTaskDelay(pdMS_TO_TICKS(1000));//延时
}
