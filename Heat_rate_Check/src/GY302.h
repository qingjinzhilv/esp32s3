#ifndef __GY302_H_
#define __GY302_H_

#include "main.h"

#define BH1750_ADDRESS   0x46		//高电平 0x5C  低电平0x23
#define GY302_SCL 39
#define GY302_SDA 40

void MyI2C_Init(void);
void MyI2C_Start(void);
void MyI2C_Stop(void);
void MyI2C_SendByte(uint8_t Byte);
uint8_t MyI2C_ReceiveByte(void);
void MyI2C_SendAck(uint8_t AckBit);
uint8_t MyI2C_ReceiveAck(void);

void GY302_Init(void);
void GY302_Write_Byte(uint8_t Byte);
void GY302_Read_Byte(void);
float Brightness(void);



#endif
