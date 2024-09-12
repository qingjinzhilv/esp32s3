#ifndef __KEY_H_
#define __KEY_H_

#include "stdio.h"
#include "Arduino.h"


#define SW2_Pin   0

enum {Key_press = true,Key_repress = false};


void Key_Init(void);
bool Key_ReadPin_IO(uint8_t gpio_num);


#endif
