#ifndef MY_XPT2046_H_
#define MY_XPT2046_H_

#include "Arduino.h"
#include <XPT2046_Bitbang.h>

#define MOSI_PIN 13
#define MISO_PIN 14
#define CLK_PIN  11
#define CS_PIN   12


void XPT2046_Init(void);
void XPT2046_GetCoordinate(uint16_t *x, uint16_t *y);

#endif /* MY_XPT2046_H_ */
