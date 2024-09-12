#ifndef __PWM_H_
#define __PWM_H_

#include <stdio.h>
#include "Arduino.h"

void PWM_init(void);
void Moto_Pwm(float MOTO1_PWM,float MOTO2_PWM,float MOTO3_PWM,float MOTO4_PWM);

#endif
