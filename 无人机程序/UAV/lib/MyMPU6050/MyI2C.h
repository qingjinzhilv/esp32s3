#ifndef MY_I2C_H_
#define MY_I2C_H_

#include "Arduino.h"

#define SCL_PIN 1
#define SDA_PIN 2

void MyI2C_Init(void);
void MyI2C_Start(void);
void MyI2C_Stop(void);
void MyI2C_SendByte(uint8_t Byte);
uint8_t MyI2C_ReceiveByte(void);
void MyI2C_SendAck(uint8_t AckBit);
uint8_t MyI2C_ReceiveAck(void);

#endif /* MY_I2C_H_ */
