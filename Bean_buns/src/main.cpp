#include <Arduino.h>
#include "ArduinoJson.h"
#include "HTTPClient.h"
#include "WiFi.h"
#include "WiFiClientSecure.h"
#include "AI_Bean_buns.h"


String *AI_answer;
String *AI_sum_answer;

void Bean_buns_task(void *arg)
{
  while(1)
  {
    if(AI_answer->length() > 0)
    {
      Serial.println("AI answer:" + *AI_answer);
      *AI_answer = "";
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
  AI_answer = &str;
  AI_sum_answer = &sum_str;
  xTaskCreate(Bean_buns_task, "Bean_buns_task", 1024*2, NULL, 1, NULL);
}

void loop()
{
  if(Serial.available())
  {
    String inputText = Serial.readStringUntil('\r'); //  \r表示结束符为回车符
    Serial.println("\n Input:" + inputText);

    getGPTAnswer(inputText);
    Serial.println("全部数据: " + *AI_sum_answer);
  }
}