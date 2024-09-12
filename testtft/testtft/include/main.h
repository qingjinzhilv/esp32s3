
#ifndef __MAIN_H__
#define __MAIN_H__
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <time.h>
#include <Ticker.h>
#include "main.h"
#include "AHT21.h"

#define KEY 13
#define RAD 34
#define LED 26
#define BUZ 27

//填写WIFI入网信息
const char* ssid     = "HIEU_Lab_2";     // WIFI账户
const char* password = "12345678"; // WIFI密码


int temp = 25,ST=600;
int loc_time[] = {0,0,0};
u16_t AutoMode = 0;

TaskHandle_t TASK_HandleOne = NULL;
TFT_eSPI tft = TFT_eSPI(TFT_WIDTH, TFT_HEIGHT);
AHT21 aht(22, 21, 0x38);
Ticker t1,t2,t3;// t1 -> time, t2 -> light, t3 ->getdata 
void GETWIFI(void *param);
void Pin_Init();
void timer();
void SetTime(String data);
void GetS(void);
void PrintData(void);
void lightcon(void);

#endif