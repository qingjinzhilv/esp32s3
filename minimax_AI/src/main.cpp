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

// 替换为您的 OpenAI API 密钥
const char* apiKey = "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJHcm91cE5hbWUiOiLlgL7lsL0iLCJVc2VyTmFtZSI6IuWAvuWwvSIsIkFjY291bnQiOiIiLCJTdWJqZWN0SUQiOiIxODI5MTMwNzc1NDg4OTA1NjM3IiwiUGhvbmUiOiIxNTg0Mjk2MjMyNyIsIkdyb3VwSUQiOiIxODI5MTMwNzc1NDgwNTE2NjIwIiwiUGFnZU5hbWUiOiIiLCJNYWlsIjoiIiwiQ3JlYXRlVGltZSI6IjIwMjQtMDktMDkgMTM6Mjk6MzgiLCJpc3MiOiJtaW5pbWF4In0.A9F7tLeJaXFAlJOlb2kvovv5inyTo4fkdJpVw4LVze95Ol459IjbG3USeuh8hw_8JuDBRWugLsTnMSh-YCHfz-HY9VVlgF6o7pHUDObnPgiYaow4RLqgC8o0BaCfoDL6WSvN0ixoh5CgtU-wYkAPTIXt4l30BFvG5bLy4T-j9eyiUGxlkSD4yqNQLcU5Beh40WIqjB76hzdIHr-z7q5NL4ShL-89GsXwdhhAzM8q7WfAsM2GHqGVJcxeDC66ctujL4vE0RlvWNZa8eQTqrM5RjXcE1hNwYCj5yhxJ4Qkt4sF-BBsa9KkgjQsEcVZKOV0f8dG6O6-5AdmbxaDgOrTSw";

// 向 OpenAI API 发送请求
String inputText = "你好，minimax！";
String apiUrl = "https://api.minimax.chat/v1/text/chatcompletion_v2";

TimerHandle_t Time_Task_handle;

TFT_eSPI tft = TFT_eSPI();
WIFI My_WiFi;
GetTime_t My_GetTime;

uint16_t x, y; 

String answer;
String getGPTAnswer(String inputText) 
{
  HTTPClient http;
  http.setTimeout(10000);
  http.begin(apiUrl);
  http.addHeader("Content-Type", "application/json");
  String token_key = String("Bearer ") + apiKey;
  http.addHeader("Authorization", token_key);
  String payload = "{\"model\":\"abab5.5s-chat\",\"messages\":[{\"role\": \"system\",\"content\": \"你是倾尽的生活助手机器人，要求下面的回答严格控制在256字符以内。\"},{\"role\": \"user\",\"content\": \"" + inputText + "\"}]}";
  int httpResponseCode = http.POST(payload);
  if (httpResponseCode == 200) 
  {
    String response = http.getString();
    http.end();
    // Serial.println(response);

    // 解析 JSON 响应
    DynamicJsonDocument jsonDoc(1024);
    deserializeJson(jsonDoc, response);
    String outputText = jsonDoc["choices"][0]["message"]["content"];
    return outputText;
    // Serial.println(outputText);
  } 
  else 
  {
    http.end();
    Serial.printf("Error %i \n", httpResponseCode);
    return "<error>";
  }
}

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

void setup() 
{
  Serial.begin(115200);
  XPT2046_Init(); //初始化触摸屏
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

  if (Serial.available()) {
    inputText = Serial.readStringUntil('\n');
    // inputText.trim();
    Serial.println("\n me:"+inputText);

    answer = getGPTAnswer(inputText);
    Serial.println("AI: " + answer);
    Serial.println("Enter a prompt:");
  }
}
