#include <ESP8266WiFi.h>
#include <espnow.h>
#include "Arduino.h"
//对方MAC地址
uint8_t broadcastAddress[] = {0x60,0x55,0xF9,0x73,0x25,0xA8};
//本机的MAC地址 EC:FA:BC:1E:3A:AE
 
typedef struct struct_message {
  int a;
  String d;
} struct_message;
 
//创建一个新的类型变量
struct_message ReData;
void espnoe_Receive(u8 *mac_addr, u8 *Re_data, u8 len) 
{
  //复制
  memcpy(&ReData, Re_data, sizeof(ReData));
  switch(ReData.a)
  {
    //前进
    case 1:
      Serial.printf("D->Up\r\n");
    break;
    //左走
    case 2:
      Serial.printf("D->Left\r\n");
    break;
    case 3:
      Serial.printf("D->Down\r\n");
    break;
    case 4:
      Serial.printf("D->Right\r\n");
    break;
    default:
      Serial.printf("D->Stop\r\n");
    break;
  }
}
 
void espnow_Init(void)
{
  //将设备设置为Wi-Fi站点
  WiFi.mode(WIFI_STA);
 
  //立即初始化ESP
  if (esp_now_init() != 0) 
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  else
  {
    Serial.println("Success initializing ESP-NOW");
  }
 //创建ESP8266角色为控制角色 从设备
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  //设置回调函数，用来发送数据  即注册发送数据函数
  esp_now_register_recv_cb(espnoe_Receive);
 
  // //与另一个ESP-NOW设备配对以发送数据 从设备
  // esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  Serial.println("");
  Serial.println(WiFi.macAddress());
}
 
// void espnow_sendData() 
// {
//   //配置要发送的值
//   myData.a = 4;
//   //发送消息
//   esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
// }