#ifndef __TIMEINITGET_H_
#define __TIMEINITGET_H_

#include "Arduino.h"
#include "time.h"
#include "ui.h"
#include "WiFiConnect.h"

#define NTP1 "ntp1.aliyun.com"
#define NTP2 "ntp2.aliyun.com"
#define NTP3 "ntp3.aliyun.com"

//获取时间任务创建
void GetTime_Create(void);

void Time_Init(void);

#endif
