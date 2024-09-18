#include <Arduino.h>
#include "My_GetTime.h"
#include "My_WiFi.h"
#include "TFT_eSPI.h"
#include "SPI.h"
#include "My_XPT2046.h"
#include "lvgl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include "AI.h"


TimerHandle_t Time_Task_handle;

TFT_eSPI tft = TFT_eSPI();
WIFI My_WiFi;
GetTime_t My_GetTime;

uint16_t x, y; 


String inputText = "你好，文心一言";
String answer;

void Time_Task(TimerHandle_t xTimer)
{
  My_GetTime.My_Time_Data.second ++;
  if(My_GetTime.My_Time_Data.second >= 60)
  {
    My_GetTime.My_Time_Data.second = 0;
    My_GetTime.My_Time_Data.minute++;
    if(My_GetTime.My_Time_Data.minute >= 60)
    {
      My_GetTime.My_Time_Data.minute = 0;
      My_GetTime.My_Time_Data.hour++;
      if(My_GetTime.My_Time_Data.hour >= 24)
      {
        My_GetTime.My_Time_Data.hour = 0;
        My_GetTime.My_Time_Data.day++;
        if(My_GetTime.My_Time_Data.day >= 32)
        {
          My_GetTime.My_Time_Data.day = 1;
          My_GetTime.My_Time_Data.month++;
          if(My_GetTime.My_Time_Data.month >= 13)
          {
            My_GetTime.My_Time_Data.month = 1;
            My_GetTime.My_Time_Data.year++;
          }
        }
      }
    }
  }
}

void AI_Task(void *pvParameters)
{
  while(1)
  {
    if(AI_Data_Str_child.length() > 0)
    {
      Serial.println("AI: " + AI_Data_Str);
      Serial.println("AI: " + AI_Data_Str_child);
      AI_Data_Str_child = "";
    }
    vTaskDelay(100/portTICK_PERIOD_MS);
  }
}
// 

void setup() 
{
  Serial.begin(115200);
  XPT2046_Init(); //初始化触摸屏
  xTaskCreate(AI_Task, "AI_Task", 1024*2, NULL, 1, NULL); //创建AI任务
  My_WiFi.WIFI_Connect("倾尽","00000001"); //连接WIFI
  answer = getGPTAnswer(inputText);
  Serial.println("Answer: " + answer);
  My_GetTime.GetTime_Set(2024,9,8,9,45,0);
  My_GetTime.GetTime_Data_Analyze();
  Time_Task_handle = xTimerCreate("Time_Task",1000,pdTRUE,(void*)0,Time_Task); //创建定时器
  xTimerStart(Time_Task_handle,0); //启动定时器
  tft.begin();
  tft.setRotation(3); //显示方向
  tft.fillScreen(TFT_BLACK); //用黑色填充屏幕
  tft.setTextColor(TFT_WHITE, TFT_BLACK); //将文本颜色设置为黑底白字
  tft.setCursor(0, 0); //将光标位置设置为左上角
  tft.setTextSize(2); //设置字号
  tft.println("Hello World!"); //显示
  
}

void loop() 
{
  XPT2046_GetCoordinate(&x, &y); //获取触摸坐标
  tft.setCursor(0, 20); //将光标位置设置为左上角
  tft.setTextSize(2); //设置字号
  tft.printf("x->%d y->%d    ",x,y); //显示
  tft.setCursor(0, 40); //将光标位置设置为左上角
  tft.setTextSize(2); //设置字号
  tft.printf("Touch Test"); //显示
  tft.setCursor(0, 60); //将光标位置设置为左上角
  tft.setTextSize(2); //设置字号
  tft.printf("%.4d-%.2d-%.2d %.2d:%.2d:%.2d",My_GetTime.My_Time_Data.year,My_GetTime.My_Time_Data.month,My_GetTime.My_Time_Data.day,My_GetTime.My_Time_Data.hour,My_GetTime.My_Time_Data.minute,My_GetTime.My_Time_Data.second); //显示

  if (Serial.available()) 
  {
    inputText = Serial.readStringUntil('\n');
    // inputText.trim();
    Serial.println("\n me:"+inputText);
    if(inputText.length() > 0)
    {
      answer = getGPTAnswer(inputText);
      Serial.println("AI: " + answer);
      Serial.println("Enter a prompt:");
    }
  }
}
