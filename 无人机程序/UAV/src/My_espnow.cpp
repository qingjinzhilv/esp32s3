#include <WiFi.h>
#include <My_espnow.h>
#include <esp_now.h>
#include "Arduino.h"
//对方MAC地址
uint8_t broadcastAddress[6] = {0x64,0xE8,0x33,0x58,0x2F,0xD0};
//本机的MAC地址  0x30,0x30,0xF9,0x72,0x2B,0xD8
//ESPNOW 对等体信息参数。
esp_now_peer_info_t peerInfo;

//espnow协议变量
My_espnow Myespnow;

UAV_Data My_UAV_Data;
UAV_RC_Data My_UAV_RC_Data;

espnow_send_param_t send_data;
espnow_send_param_t recv_data;

//创建一个新的类型变量
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t sendStatus) 
{
  if(sendStatus != ESP_NOW_SEND_SUCCESS)
  {
    My_espnow_sendData(send_data);
  }
}

//接收回调函数
void OnDataReceive(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{
  //复制
  memcpy(&recv_data,data,sizeof(recv_data));
  My_espnow_Data_parsing(recv_data.data);
}


//初始化
uint8_t My_espnow::espnow_Init(uint8_t MAC[])
{
  //将设备设置为Wi-Fi站点
  WiFi.mode(WIFI_STA);
 
  //立即初始化ESP
  if(esp_now_init() != 0) 
  {
    Serial.println("Error initializing ESP-NOW");
    return 0;
  }
  //设置回调函数，用来发送数据  即注册发送数据函数
  esp_now_register_send_cb(OnDataSent);
  //设置回调函数，用来接收数据  即注册接收数据函数
  esp_now_register_recv_cb(OnDataReceive);
  //与另一个ESP-NOW设备配对以发送数据 从设备
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  memcpy(peerInfo.peer_addr,MAC,6);
  esp_now_add_peer(&peerInfo);
  Serial.println("");
  Serial.println(WiFi.macAddress());
  xTaskCreate(espnow_Receive_Task,"espnow_Receive_Task",1024*4,NULL,1,NULL);
  return 1;
}
// 发送数据函数
void My_espnow::espnow_sendData(espnow_send_param_t myData)
{
  send_data = myData;
   //发送消息
  esp_now_send(broadcastAddress,(uint8_t *)&myData, sizeof(myData));
}

void My_espnow::espnow_Serial_Send(void)
{
  delay(100);
}
//初始化espnow 外部使用
bool My_espnow_Init(void)
{
  
  return Myespnow.espnow_Init(broadcastAddress);
}

void My_espnow_sendData(espnow_send_param_t myData)
{
  Myespnow.espnow_sendData(myData);
}

void espnow_Receive_Task(void *pvParameters)
{
  espnow_send_param_t UAV_Send_RC;
  while(1)
  {
    //解锁状态
    UAV_Send_RC.data = "lock" + String(My_UAV_Data.unlock);
    My_espnow_sendData(UAV_Send_RC);
    delay(100);

    //低压状态
    char VBAT_Str[10] = {0};
    sprintf(VBAT_Str,"%.3d",My_UAV_Data.UAV_Battery_Voltage);
    UAV_Send_RC.data = "VBAT" + String(VBAT_Str);
    My_espnow_sendData(UAV_Send_RC);
    delay(100);

    //陀螺仪数据
    char mpu_data[15] = {0};
    sprintf(mpu_data,"%d|%d|%d|",(int)My_UAV_Data.UAV_MPU6050_Data.Pitch,(int)My_UAV_Data.UAV_MPU6050_Data.Roll,(int)My_UAV_Data.UAV_MPU6050_Data.Yaw);
    UAV_Send_RC.data = "M" + String(mpu_data);
    My_espnow_sendData(UAV_Send_RC);
    delay(100);

    //油门值
    UAV_Send_RC.data = "VAX" + String(My_UAV_Data.Throttle_value) + '|';
    My_espnow_sendData(UAV_Send_RC);
  }
}

void My_espnow_Data_parsing(String data)
{
  // Serial.printf("原始数据->%s\r\n",data);
  char buf[255] = {0};
  //解锁状态
  data.toCharArray(buf,sizeof(buf));
  if(buf[0] == 'l' && buf[3] == 'k')
  {
    //解析数据
    My_UAV_RC_Data.unlock = toNum(buf[5]);
  }
  //油门
  else if(buf[0] == 'V' && buf[2] == 'X')
  {
    char *start = buf + 3;
    uint16_t Throttle_value = 0;
    while(*start != '|')
    {
      Throttle_value *= 10;
      Throttle_value += toNum(start[0]);
      start++;
    }
    My_UAV_RC_Data.throttle_value = Throttle_value;
  }
  // Serial.printf("解锁状态->%d\r\n",My_UAV_RC_Data.unlock);
}
