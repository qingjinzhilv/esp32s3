#ifndef __BLUETOOTH_H_
#define __BLUETOOTH_H_

#include <Arduino.h>
//蓝牙初始化
void Bluetooth_Init(String name);
//蓝牙连接状态判断以及发送信息
void Bluetooth_connect_judg(void);

#endif

