#include <Arduino.h>
#include "Key.h"
#include "LED.h"
#include "VBAT.h"
#include "My_WIFI.h"
#include "myMPU6050.h"
#include "PWM.h"
#include "PID.h"
#include "Control.h"


void setup() 
{
  Init();
  Task_Create_Init();
}

void loop() 
{
  delay(10000);
}
