#include <Arduino.h>
#include "ArduinoJson.h"
#include "HTTPClient.h"
#include "WiFi.h"
#include "WiFiClientSecure.h"
#include "AI_Bean_buns.h"
#include "MAX98357_MiniMax.h"
#include "MAX9814.h"

String *AI_answer;
String *AI_sum_answer;
String ask = "";

void Bean_buns_task(void *arg)
{
  while (1)
  {
    if (AI_answer->length() > 0)
    {
      Serial.println("AI answer:" + *AI_answer);
      *AI_answer = "";
    }
    delay(15);
  }
}

void MAX98357_task(void *arg)
{
  while (1)
  {
    // MAX98357_loop();
    ask = MAX9814_speech_recognition();
    if (ask != "")
    {
      Serial.println("me ask:" + ask);
      getGPTAnswer(ask);
      Serial.println("全部数据: " + *AI_sum_answer);
      // MAX98357_SetText(*AI_sum_answer);
      ask = "";
    }
    delay(100);
  }
}

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin("倾尽", "00000001");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");
  MAX9814_init();

  // MAX98357_Init();
  // MAX98357_Settimbre(WUMEI_YUJIE);
  // MAX98357_SetSound_volume(10);

  AI_answer = &str;
  AI_sum_answer = &sum_str;
  xTaskCreate(Bean_buns_task, "Bean_buns_task", 1024 * 8, NULL, 1, NULL);
  xTaskCreate(MAX98357_task, "MAX98357_task", 1024 * 8, NULL, 2, NULL);
  // MAX98357_SetText("你好，我是测试程序小K!");
}

void loop()
{
}