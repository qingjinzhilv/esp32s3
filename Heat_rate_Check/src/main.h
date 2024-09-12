#ifndef __MAIN_H_
#define __MAIN_H_

//自带库
#include <Arduino.h>
#include "String.h"
#include "main.h"
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include "Ticker.h"
#include "Wire.h"
//自建
#include "Weather.h"
#include "key.h"
#include "keyboard.h"
#include "AHT10.h"
#include "GY302.h"
#include "My_MAX30102.h"
#include "My_MQTT.h"
//外部
#include "TFT_eSPI.h"
#include <SPI.h>
#include <ArduinoJson.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "spo2_algorithm.h"
#include <Crypto.h>
#include <WiFi.h>
#include <PubSubClient.h>

extern String weather;
extern int temp;
extern _Bool LED_flag,wifi_state;

extern const char *ssid,*password;


//主界面函数
void interface(void);
//定时器函数
void Timer_1(void);
//WiFi连接函数
void WiFiConnect(void);
//获取时间函数
void GetTime(void);
//菜单界面函数
void Menu(void);
//界面子函数
void WiFi_subinterface(void);
void LED_subinterface(void);
void SetTime_subinterface(void);
//子界面的子界面
void WiFi_Add(void);
//蜂鸣器控制函数
void Buzzer_Control(void);

//初始化
void Init(void);


#endif
