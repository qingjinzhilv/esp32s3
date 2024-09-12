#include <Arduino.h>
#include <WiFi.h>
#include "Wire.h"

void setup() 
{
  Serial.begin(115200);
  pinMode(,OUTPUT);

  WiFi.begin("倾尽","00000001");
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  Serial.print("WiFI Success!");
}

void loop() {
  millis();
  // put your main code here, to run repeatedly:
}
