#include <Arduino.h>
#include "WiFi.h"
#include "MAX9814.h"

void setup()
{
  Serial.begin(115200);
  uint8_t count = 0;
  WiFi.disconnect(true);
  WiFi.begin("倾尽", "00000001"); // 填写自己的wifi账号密码
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    count++;
    if(count >= 75)
    {
      Serial.printf("\r\n-- wifi connect fail! --");
      break;
    }
    vTaskDelay(200);
  }
  Serial.printf("\r\n-- wifi connect success! --\r\n");
  
  MAX9814_init();
}

void loop()
{
  MAX9814_speech_recognition();
}


