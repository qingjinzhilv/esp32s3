#include <WiFi.h>
#include <My_espnow.h>
#include <esp_now.h>
#include "Arduino.h"
//对方MAC地址
uint8_t broadcastAddress[6] = {0xEC,0xFA,0xBC,0x1E,0x3A,0xAE};
//本机的MAC地址  60:55:F9:73:25:A8
uint8_t send_flag = 0;
 
esp_now_peer_info_t peerInfo;

typedef struct struct_message {
  int a;
  String d;
} struct_message;
 
//创建一个新的类型变量
struct_message myData;
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t sendStatus) 
{
  Serial.print("Last Packet Send Status: ");//将消息是否发送成功打印出来
  if (sendStatus == ESP_NOW_SEND_SUCCESS) 
  {
    Serial.println("Delivery success->");
    Serial.println(myData.a);
    Serial.println(myData.d);
  }
  else 
  {
    send_flag = 1;
    Serial.println("Delivery fail");
  }
}
 
uint8_t espnow_Init(uint8_t MAC[])
{
  //将设备设置为Wi-Fi站点
  WiFi.mode(WIFI_STA);
 
  //立即初始化ESP
  if (esp_now_init() != 0) 
  {
    Serial.println("Error initializing ESP-NOW");
    return 0;
  }
  //设置回调函数，用来发送数据  即注册发送数据函数
  esp_now_register_send_cb(OnDataSent);
 
  //与另一个ESP-NOW设备配对以发送数据 从设备
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  memcpy(peerInfo.peer_addr,MAC,6);
  esp_now_add_peer(&peerInfo);
  Serial.println("");
  Serial.println(WiFi.macAddress());
  return 1;
}
 
void espnow_sendData(int int_data,String str_data) 
{
  //配置要发送的值
  myData.a = int_data;
  myData.d = str_data;
  //发送消息
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
}

void espnow_Serial_Send(void)
{
  int int_data = 0;
  String Str_data;
  if(Serial.available())
  {
    if(Serial.available())
    {
      if(Serial.find("int"))
      {
        int_data = Serial.parseInt();
        if(Serial.find("str"))
        {
            Str_data = Serial.readString();
        }
      }
    }
    espnow_sendData(int_data,Str_data);
  }
  delay(100);
}