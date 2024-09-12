#include <Arduino.h>
#include <OLED.h>


void setup()
{
  OLED_Init();
}

void loop()
{

  OLED_ShowString(1,1,(char *)"Hello World!");

}
