#ifndef __ROCKER_H_
#define __ROCKER_H_

#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

typedef struct
{
  uint16_t rocker_Left_x;
  uint16_t rocker_Left_y;
  uint16_t rocker_Right_x;
  uint16_t rocker_Right_y;
  uint8_t key_num;
}rocker_Data;




#define Right_VRX 7
#define Right_VRY 3
#define Right_SW 8

#define Left_VRX 6
#define Left_VRY 5
#define Left_SW 17


#define Key_UP_LEFT 16
#define Key_UP_RIGHT 18



enum {
    Right_Press = 1,Right_Long_Press = 10,
    Left_Press = 2,Left_Long_Press = 20,
    UP_Left_Press = 3,UP_Left_Long_Press = 30,
    UP_Right_Press = 4,UP_Right_Long_Press = 40};

void Rocker_Init(void);
void Rocker_GetData(rocker_Data *rocker_data);

extern rocker_Data MyRockerData;

#endif