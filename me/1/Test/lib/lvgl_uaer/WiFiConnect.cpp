#include "head.h"
#include "TimeInitGet.h"


void WiFi_Connect(void *pv);

String ssid = "qingjin";
String password = "00000001";

TaskHandle_t WiFi_Connect_handle;

//连接WiFi任务创建
void WiFi_Creat(void)
{
    Serial.println("WiFi_Creat");
    xTaskCreate((TaskFunction_t)WiFi_Connect,(char *)"WiFi_Connect",(uint32_t)8 * 1024,NULL,(UBaseType_t)10,&WiFi_Connect_handle);
}

void WiFi_Connect(void *pv)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(),password.c_str());
    Serial.println("WiFi_Connect");
    while(WiFi.status() != WL_CONNECTED)
    {
        delay(250);
    }
    configTime(8 * 3600,0,NTP1,NTP2,NTP3);
    GetTime_Create();
    vTaskDelete(WiFi_Connect_handle);
}
