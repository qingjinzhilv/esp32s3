#include <Arduino.h>
#include "TFT_eSPI.h"
#include "SPI.h"
#include <XPT2046_Touchscreen.h>

TFT_eSPI tft = TFT_eSPI();
XPT2046_Touchscreen touch(12, 13);

void setup() 
{
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(2); //显示方向
  tft.fillScreen(TFT_BLACK); //用黑色填充屏幕
  tft.setTextColor(TFT_WHITE, TFT_BLACK); //将文本颜色设置为黑底白字
  tft.setCursor(0, 0); //将光标位置设置为左上角
  tft.setTextSize(3); //设置字号
  tft.println("Hello World!"); //显示
}

void loop() 
{
  Serial.println("Hello World!");
  delay(1000);
}
