#ifndef __MY_MAX30102_H_
#define __MY_MAX30102_h_

#include "main.h"


//引脚使用
//SCL ------ 42
//SDA ------ 41
//在#include "Pins_Arduino.h" 里面修改引脚
#include "Pins_Arduino.h"

//最大亮度
#define MAX_BRIGHTNESS 255

extern int32_t spo2; //血氧值
extern int8_t validSPO2; //指示器显示 SPO2 计算是否有效
extern int32_t heartRate; //心率值
extern int8_t validHeartRate; //指示心率计算是否有效的指示器

void MAX30102_Init(void);
void MAX30102_GetData(void);


#endif

