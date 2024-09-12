#include "head.h"


void setup(void) 
{
    Serial.begin(115200);
    lvgl_Task();        //UI任务创建
    GetTime_Create();   //获取时间任务创建
    Time_Init();        //软件定时器创建
}

void loop() 
{
}




