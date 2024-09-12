#include "Arduino.h"

void Buzzer_Init(void)
{
    pinMode(8,OUTPUT);
    digitalWrite(8,LOW);
}

void Buzzer_ON(void)
{
    digitalWrite(8,HIGH);
}

void Buzzer_OFF(void)
{
    digitalWrite(8,LOW);
}

void Buzzer_Turn(void)
{
    uint8_t pin = digitalRead(8);
    digitalWrite(8,!pin);
}

