#ifndef __MYSPI_H
#define __MYSPI_H

#include "Arduino.h"

#define SPI_CS_PIN 12
#define SPI_CLK_PIN 11
#define SPI_MOSI_PIN 13     //SDI
#define SPI_MISO_PIN 14     //SDO

#define SPI_IRQ_PIN 21

void MySPI_Init(void);
void MySPI_Start(void);
void MySPI_Stop(void);
uint8_t MySPI_SwapByta(uint8_t ByteSend);

void MySPI_W_SS(uint8_t BitValue);
void MySPI_W_SCK(uint8_t BitValue);
void MySPI_W_MOSI(uint8_t BitValue);
uint8_t MySPI_R_MISO(void);

#endif
