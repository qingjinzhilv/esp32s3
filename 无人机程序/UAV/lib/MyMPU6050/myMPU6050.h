#ifndef __MYMPU6050_H__
#define __MYMPU6050_H__

#include <Arduino.h>
#include "MyI2C.h"
#include "MPU6050_Reg.h"
#include "MadgwickAHRS.h"

typedef struct
{
	float Yaw;
	float Roll;
	float Pitch;
}MPU6050_Data;

extern MPU6050_Data My_MPU6050_Data;

void MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data);
uint8_t MPU6050_ReadReg(uint8_t RegAddress);

void MPU6050_Init(void);
uint8_t MPU6050_GetID(void);
void MPU6050_GetData(int16_t *AccX,int16_t *AccY,int16_t *AccZ,int16_t *GyroX,int16_t *GyroY,int16_t *GyroZ);

void dataGetERROR(void);
void getMPU6050Data(void);
void dataGetAndFilter(void);
void MPU6050_Test(void);
void MPU6050_Timer(TimerHandle_t pxTimer);
typedef struct MySensorMsg {
	float A[3];
	float G[3];
} MySensorMsg;

typedef struct MPU6050Params {
	uint8_t MPU6050dt;
	uint64_t preMillis;
	float MPU6050ERROE[6];
	unsigned long previousprintMillis; // 记录上一次打印数据时的时间
} MPU6050Params;


#endif

