#include <Arduino.h>

#include "TFT_eSPI.h"
#include "SPI.h"
#include "My_XPT2046.h"
#include "My_WiFi.h"
// #include "My_GetTime.h"

TFT_eSPI tft = TFT_eSPI();
WIFI My_WiFi;

uint16_t x, y;  

void setup() 
{
  Serial.begin(115200);
  XPT2046_Init(); //初始化触摸屏
  My_WiFi.WIFI_Connect("倾尽","00000001"); //连接WIFI
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
  tft.printf("Touch Test"); //显示
}
