#include "My_XPT2046.h"

XPT2046_Bitbang touchscreen(MOSI_PIN, MISO_PIN, CLK_PIN, CS_PIN);

void XPT2046_Init(void)
{
    // 初始化触摸屏
  touchscreen.begin();
  pinMode(21,INPUT); //使能中断引脚
}

void XPT2046_GetCoordinate(uint16_t *x, uint16_t *y)
{
	TouchPoint touch = touchscreen.getTouch();
   	// 判断是否右触摸
	// if(digitalRead(21) == 0) 
	// {
		*x = 320 - touch.x;
		*y = touch.y;
	// }
}

