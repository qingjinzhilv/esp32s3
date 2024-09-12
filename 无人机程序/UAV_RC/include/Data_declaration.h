#ifndef DATA_DECLARATION_H_
#define DATA_DECLARATION_H_

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "string.h"
#include "math.h"
#include "My_espnow.h"
#include "myMPU6050.h"

#define UAV_MAX 599
#define UAV_MIN 49

//消息发送判断枚举
enum{
    LOCK = '0',
    THROTTLE_ADD = '1',
    THROTTLE_REDUCE = '2',
};

//初始化标志位
extern bool init_ok;

//电压检测数据
extern uint32_t VBAT_Value;
//陀螺仪数据
extern MPU6050_Data My_MPU6050_Data;
//按键值
extern uint8_t key;


#endif
