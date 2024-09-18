#include <Arduino.h>
#include "WiFi.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"
#include "string.h"
#include "xf.h"

String *AI_answer;

void xf_task(void *arg)
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
  while (WiFi.status()!= WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  AI_answer = &str;

  xTaskCreate(xf_task, "xf_task", 1024*2, NULL, 1, NULL);
}

void loop() 
{
  if(Serial.available() > 0)
  {
    String inputText = Serial.readStringUntil('\n');
    Serial.printf("Input: %s\n", inputText.c_str());
    String outputText = sendRequest(inputText);
    Serial.println(outputText);
  }

  
}