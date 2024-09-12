#include "TimeInitGet.h"

//函数声明
void SetClock(void *pv);
void Timer(TimerHandle_t xTimer);


TimerHandle_t Timer_handle;
TaskHandle_t GetTime_Handle;

uint16_t h_m_s[7] = {0};

const String WDAY_NAMES[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};                //星期

//获取时间任务创建
void GetTime_Create(void)
{
  Serial.println("GetTime_Create");
  xTaskCreate((TaskFunction_t)SetClock,(char *)"SetClock",(uint32_t)8 * 1024,NULL,(UBaseType_t)11,&GetTime_Handle);
}

//创建软件定时器
void Time_Init(void)
{
    Serial.println("TimerCreate");
    Timer_handle = xTimerCreate("Timer",1000,pdTRUE,(void *)1,Timer); 
    if(Timer_handle == NULL)
    {
      return;
    }   
    xTimerStart(Timer_handle,0);
}

//软件定时器回调函数
void Timer(TimerHandle_t xTimer)
{
  h_m_s[0] ++;
  if(h_m_s[0] >= 60)
  {
    h_m_s[0] = 0;
    h_m_s[1] ++;
    if(h_m_s[1] >= 60)
    {
      h_m_s[1] = 0;
      h_m_s[2] ++;
      if(h_m_s[2] >= 24)
      {
        h_m_s[2] = 0;
      }
      lv_label_set_text_fmt(ui_hour, "%.2d",h_m_s[2]);
    }
    lv_label_set_text_fmt(ui_minute, "%.2d",h_m_s[1]);
  }
  lv_label_set_text_fmt(ui_second, "%.2d",h_m_s[0]);
}

struct tm timeinfo;
void SetClock(void *pv)
{
  delay(1000);
  //如果获取失败，就开启联网模式，获取时间
  if(!getLocalTime(&timeinfo))
  {
    Serial.println("Fail to obtain time");
    //创建连接WiFi的任务
    WiFi_Creat();
  }
  else 
  {
    Serial.println("timeinfo Start");
    //秒
    h_m_s[0] = timeinfo.tm_sec;
    //分
    h_m_s[1] = timeinfo.tm_min;
    //时
    h_m_s[2] = timeinfo.tm_hour;
    //日
    h_m_s[3] = timeinfo.tm_mday;
    //月
    h_m_s[4] = timeinfo.tm_mon + 1;
    //年 
    h_m_s[5] = timeinfo.tm_year + 1900;
    //周
    h_m_s[6] = timeinfo.tm_wday;
    //时
    lv_label_set_text_fmt(ui_hour, "%.2d",h_m_s[2]);
    //分
    lv_label_set_text_fmt(ui_minute, "%.2d",h_m_s[1]);
    //秒
    lv_label_set_text_fmt(ui_second, "%.2d",h_m_s[0]);
    //年 月 日 ui_Label7
    lv_label_set_text_fmt(ui_YearMonDa, "%.4d/%.2d/%.2d",h_m_s[5],h_m_s[4],h_m_s[3]);
    //星期
    lv_label_set_text_fmt(ui_week, "%s",WDAY_NAMES[h_m_s[6]]);

    Serial.printf("%.4d-%.2d-%.2d %.2d:%.2d:%.2d %.1d\r\n",
                h_m_s[5],h_m_s[4],h_m_s[3],
                h_m_s[2],h_m_s[1],h_m_s[0],
                h_m_s[6]);

    Serial.println("timeinfo end");
  }
  vTaskDelete(NULL);
}

