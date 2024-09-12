#ifndef __INFORNATIONGET_H_
#define __INFORNATIONGET_H_

#include "Arduino.h"
#include "time.h"
#include "ui.h"
#include "WiFiConnect.h"
#include "stddef.h"
#include <ArduinoJson.h>
#include "WiFi.h"
#include "WiFiClient.h"

#define NTP1 "ntp1.aliyun.com"
#define NTP2 "ntp2.aliyun.com"
#define NTP3 "ntp3.aliyun.com"

//获取时间任务创建
void GetTime_Create(void);
//软件定时器创建
void Time_Init(void);

//获取天气任务创建
void GetWeather_Create(void);

void Buil_Request(void);
void parseInfo(WiFiClient client);
void httpRequest(String reqRes);

void parseInfo1(WiFiClient client);
void httpRequest1(String reqRes);

#endif
