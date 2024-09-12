#ifndef __VBAT_H_
#define __VBAT_H_

#include "stdio.h"
#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern uint32_t VBAT_Value;
extern bool init_ok;

#define VBAT_PIN 4

void VBAT_Init(void);


#endif