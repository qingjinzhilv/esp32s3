#include <Arduino.h>

#define LED_PIN0 PB5
#define LED_PIN1 PC13

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN0,OUTPUT);
  pinMode(LED_PIN1,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_PIN0,HIGH);
  digitalWrite(LED_PIN1,HIGH);
  delay(100);
  digitalWrite(LED_PIN0,LOW);
  digitalWrite(LED_PIN1,LOW);
  delay(100);
}
