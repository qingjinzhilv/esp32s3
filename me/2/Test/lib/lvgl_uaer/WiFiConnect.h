#ifndef __WIFICONNECT_H_
#define __WIFICONNECT_H_

extern String ssid;
extern String password;

//连接WiFi任务创建
void WiFi_Creat(void);
//查找WiFi任务创建
void WiFi_Find_Create(void);
// 断开WiFi连接
void Cancel_WiFi(void);

#endif

