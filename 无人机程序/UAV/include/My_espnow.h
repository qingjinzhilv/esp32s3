#ifndef __MY_ESPNOW_H_
#define __MY_ESPNOW_H_

#include <iostream>
#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "Data_declaration.h"
#include "myMPU6050.h"



// C++声明定义
using namespace std;

#define toNum(x) ((x) - '0')

enum{
  //解锁标识 lock-1  lock-0
  __unlock = 0,
  __lock = 1,
  //低压报警标识 VBAT_LOW-1  VBAT_LOW-0
  VBAT_LOW = 0,
  VBAT_HIGH = 1,
};

/*
   解锁状态：lock-0/1
   目标角度数据：Mxx|xx|xx|   xxPitch xxRoll xxYaw
   油门值：VAXXX|
*/
//接收到的遥控数据
typedef struct UAV_RC_Data
{
  bool unlock;         				//解锁状态
  uint16_t throttle_value;      //油门值
  MPU6050_Data UAV_target_angle;   //目标角度数据
}UAV_RC_Data;

/*
   解锁状态：lock-0/1
   低压状态：VBATxxx
   MPU6050数据：Mxx|xx|xx|   xxPitch xxRoll xxYaw
   油门值：VAXXX|
*/
//向遥控发送的数据
typedef struct UAV_Data
{
  String UAV_Str;         				//遥控器数据 字符串形式  
  bool unlock;         				    //解锁状态
  uint32_t UAV_Battery_Voltage;       //低压报警标识
  MPU6050_Data UAV_MPU6050_Data;   //MPU6050数据
  uint16_t Throttle_value;      //油门值
}UAV_Data;

extern UAV_RC_Data My_UAV_RC_Data;
extern UAV_Data My_UAV_Data;


typedef struct espnow_send_param_t 
{
  String data;         //发送数据
} espnow_send_param_t;

class My_espnow
{
  public:
    uint8_t espnow_Init(uint8_t MAC[]);
    void espnow_sendData(espnow_send_param_t myData);
    void espnow_Serial_Send(void);
};
bool My_espnow_Init(void);
void My_espnow_sendData(espnow_send_param_t myData);
void espnow_task(void *arg);

void espnow_Receive_Task(void *pvParameters);

void My_espnow_Data_parsing(String data);


#endif
