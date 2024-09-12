#ifndef __MYSPI_H
#define __MYSPI_H

#include "Arduino.h"

#define SPI_CS_PIN 12
#define SPI_CLK_PIN 11
#define SPI_MOSI_PIN 13
#define SPI_MISO_PIN 14

#define SPI_IRQ_PIN 21

void MySPI_Init(void);
void MySPI_Start(void);
void MySPI_Stop(void);
uint8_t MySPI_SwapByta(uint8_t ByteSend);

#endif
