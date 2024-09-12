#include "VBAT.h"



static void VBAT_task(void *pvParameters){
	uint32_t read_raw;
	TickType_t adp = xTaskGetTickCount();
	const TickType_t adg = 500;//这里的数是指ticks（时间片）的意思，等于1就是每个ticks中断都执行
	while(1)
	{
		vTaskDelayUntil(&adp,adg);
		if(init_ok)
		{
			read_raw = analogRead(VBAT_PIN);// 采集ADC原始值
			VBAT_Value = (float)read_raw * 682.64  / 4095.0f;// 计算电压值
			//printf("ADC Raw->%d   Convert the voltage value->%d mV\r\n",(int)read_raw,(int)VBAT_Value);
		}
	}
}

void VBAT_Init(void)
{
    printf("\r\n******************************VBAT Init Start******************************\r\n");
    pinMode(VBAT_PIN, ANALOG);      // 设置VBAT引脚为输入模式
    printf("\r\n******************************VBAT Task Create Start******************************\r\n");
	xTaskCreate(VBAT_task, "VBAT_task", 1024*2, NULL, 0, NULL);//创建任务不断循环获取电池电压
}
