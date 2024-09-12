#include <WiFi.h>
#include <My_espnow.h>
#include <esp_now.h>
#include "Arduino.h"
//对方MAC地址
uint8_t broadcastAddress[6] = {0x30,0x30,0xF9,0x72,0x2B,0xD8};
//本机的MAC地址  0x64,0xE8,0x33,0x58,0x2F,0xD0
//ESPNOW 对等体信息参数。
esp_now_peer_info_t peerInfo;

//espnow协议变量
My_espnow Myespnow;

UAV_Data My_UAV_Data;
RC_UAV_Data My_RC_UAV_Data;

espnow_send_param_t send_data;
espnow_send_param_t recv_data;

//创建一个新的类型变量
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t sendStatus) 
{
  if(sendStatus != ESP_NOW_SEND_SUCCESS)
  {
    Myespnow.espnow_sendData(send_data);
  }
}

//接收回调函数
void OnDataReceive(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{
  //复制
  memcpy(&recv_data,data,sizeof(recv_data));
  Serial.println("Received Data ->" + (recv_data.data));
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
  Serial.printf("向UAV发送的数据->%d\r\n",myData.data);
}

void MPU6050_parsing(char *buf)
{
  int16_t Pitch = 0,Roll = 0,Yaw = 0;
  char *start = buf + 1;
  // Serial.printf("+1后的数据->%s\r\n",start);
	if(*start == '-')
	{
		start ++;
		while(*start != '|')
		{
			Pitch *= 10;
			Pitch += toNum(start[0]);
			start ++;
		}
		Pitch = -Pitch;
	}
	else
	{
		while(*start != '|')
		{
			Pitch *= 10;
			Pitch += toNum(start[0]);
			start ++;
		}
	}
	start ++;
  // Serial.printf("解析完Pitch后的数据+1后的数据->%s\r\n",start);
	if(*start == '-')
	{
		start ++;
		while(*start != '|')
		{
			Roll *= 10;
			Roll += toNum(start[0]);
			start ++;
		}
		Roll = -Roll;
	}
	else
	{
		while(*start != '|')
		{
			Roll *= 10;
			Roll += toNum(start[0]);
			start ++;
		}
	}
  char *end = start + 1;
  // Serial.printf("解析完Roll后的数据+1后的数据->%s\r\n",end);
	if(*end == '-')
	{
		end ++;
		while(*end != '|')
		{
			Yaw *= 10;
			Yaw += toNum(end[0]);
			end ++;
		}
		Yaw = -Yaw;
	}
	else
	{
		while(*end != '|')
		{
			Yaw *= 10;
			Yaw += toNum(end[0]);
			end ++;
		}
	}
  My_UAV_Data.UAV_MPU6050_Data.Pitch = Pitch;
  My_UAV_Data.UAV_MPU6050_Data.Roll = Roll;
  My_UAV_Data.UAV_MPU6050_Data.Yaw = Yaw;
  // Serial.printf("UAV MPU6050数据：Pitch:%d Roll:%d Yaw:%d\r\n",Pitch,Roll,Yaw);
}

void My_espnow_Data_parsing(String data)
{
  char buf[25] = {0};
  data.toCharArray(buf,sizeof(data));
  if(buf[0] == 'l' && buf[3] == 'k')
  {
    init_ok = true;
  }
  //解锁数据
  else if(buf[0] == 'l' && buf[3] == 'k')
  {
    My_UAV_Data.unlock = toNum(buf[4]);
  }
  //低压数据 VBATxxx
  else if(buf[0] == 'V' && buf[3] == 'T')
  {
    My_UAV_Data.UAV_Battery_Voltage = toNum(buf[4])*100 + toNum(buf[5])*10 + toNum(buf[6]);
  }
  //陀螺仪数据 MPU6050数据：Mxxxxxx   xxPitch xxRoll xxYaw
  else if(buf[0] == 'M')
  {
    MPU6050_parsing(buf);
  }
  //油门值
  else if(buf[0] == 'V' && buf[2] == 'X')
  {
    char *start = buf + 3;
    uint16_t Throttle_value = 0;
    Serial.printf("油门值->%s\r\n",start);
    while(*start != '|')
    {
      Throttle_value *= 10;
      Throttle_value += toNum(start[0]);
      start++;
    }
    My_UAV_Data.Throttle_value = Throttle_value;
  }
}



