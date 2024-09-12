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
#include "lvgl_task.h"


void setup() 
{
  Init(); //初始化函数
  lvgl_Task(); //lvgl任务初始化
}

void loop() 
{
  
}
