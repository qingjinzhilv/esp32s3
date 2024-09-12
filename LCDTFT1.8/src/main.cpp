#include <TFT_eSPI.h>
#include "Chinese.h"
#include "dog.h"

TFT_eSPI tft = TFT_eSPI();  // 引脚请自行配置tft_espi库中的 User_Setup.h文件


void setup()
{
  //tft液晶屏初始化设置
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(0x0000);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);//红色汉字，蓝色背景
  tft.loadFont(Chinese_16);
  tft.drawString("倾尽制作",0,10); //在坐标0,0位置输出汉字,就可以在tft显示出来了
  tft.loadFont(Chinese_20);
  tft.drawString("尽力而为",0,50);
  tft.setSwapBytes(true);
  tft.pushImage(0,80,96,96,dog_0);
  // tft.unloadFont();
}
void loop(){
  while(1){
    delay(2000);
  }
} 