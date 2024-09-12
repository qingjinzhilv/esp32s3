#ifndef __MY_GETTIME_H
#define __MY_GETTIME_H

#include <iostream>
#include "Arduino.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "string.h"
#include "math.h"


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
    public:
        MyTime_t My_Time_Data;
        void GetTime_Set(void);
        void GetTime_Set(uint16_t Year,uint8_t Month,uint8_t Day);
        void GetTime_Set(uint8_t Hour,uint8_t Minute,uint8_t Second);
        void GetTime_Set(uint16_t Year,uint8_t Month,uint8_t Day,uint8_t Hour,uint8_t Minute,uint8_t Second);
        String GetTime_Str(void);
        void GetTime_Data_Analyze(String time_str);
};


#endif // GETTIME_H
