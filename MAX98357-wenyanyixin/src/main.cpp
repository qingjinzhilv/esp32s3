#include <Audio.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "Arduino.h"
#include "MAX98357_MiniMAX.h"
#include "AI.h"
//1. 替换为您的网络凭证
const char* ssid = "倾尽";
const char* password = "00000001";



void setup() 
{
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());

  // 打印您连接到的网络的 SSID:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  MAX98357_Init();
  MAX98357_Settimbre(LENGDAN_XIONGZHANG);
  MAX98357_SetSound_volume(20);
  MAX98357_SetText("你好，我是测试程序小K!");
}

void loop() 
{
    MAX98357_loop();
    if(Serial.available() > 0)
    {
        String c = Serial.readStringUntil('\n');
        Serial.printf("me->%s\r\n", c.c_str());
        String AI_answer = getGPTAnswer(c);
        MAX98357_SetText(AI_answer);
    }
}


