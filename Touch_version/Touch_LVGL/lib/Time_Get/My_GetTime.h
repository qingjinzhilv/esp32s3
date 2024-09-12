#ifndef MY_GETTIME_H
#define MY_GETTIME_H

/*要链接的文件->  amcewen/HttpClient@^2.2.0
 *
*/

#include <iostream>
#include "Arduino.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "string.h"
#include "math.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"



using namespace std;



typedef struct MyTime_t
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
}MyTime_t;

class GetTime_t 
{
    private:
        String GetTime_Str(void);
    public:
        MyTime_t My_Time_Data;
        void GetTime_Init(void);
        void GetTime_Set(void);
        void GetTime_Set(uint16_t Year,uint8_t Month,uint8_t Day);
        void GetTime_Set(uint8_t Hour,uint8_t Minute,uint8_t Second);
        void GetTime_Set(uint16_t Year,uint8_t Month,uint8_t Day,uint8_t Hour,uint8_t Minute,uint8_t Second);
        
        void GetTime_Data_Analyze(void);
};

void Time_Task(TimerHandle_t xTimer);

extern GetTime_t My_GetTime;

#endif
