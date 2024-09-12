#include "head.h"


void setup(void) 
{
    Serial.begin(115200);
    Buzzer_Init();      //蜂鸣器引脚初始化
    lvgl_Task();        //UI任务创建
    Time_Init();        //软件定时器创建
}

void loop() 
{
}




