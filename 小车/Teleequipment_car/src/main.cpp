#include <Arduino.h>
#include <OLED.h>
#include "My_espnow.h"


void setup()
{
  Serial.begin(115200);
  OLED_Init();
  espnow_Init();
}

void loop()
{
  OLED_ShowString(1,1,(char *)"Hello World!");
}
