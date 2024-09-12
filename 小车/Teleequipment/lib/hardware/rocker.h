#ifndef __ROCKER_H_
#define __ROCKER_H_

#include "Arduino.h"

extern uint8_t key_flag;

#define Right_VRX 0
#define Right_VRY 1
#define Right_SW 12

#define Left_VRX 2
#define Left_VRY 3
#define Left_SW 10

void Rocker_Init(void);
void Rocker_GetData(uint32_t *ADC_Data,uint8_t *SW);



#endif