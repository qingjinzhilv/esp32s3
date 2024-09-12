#include <Arduino.h>
#include "TFT_eSPI.h"
#include "SPI.h"
#include "Rocker.h"
#include "LED.h"
#include "VBAT.h"
#include "My_WIFI.h"
#include "myMPU6050.h"
#include "My_espnow.h"
#include "Data_declaration.h"

// 屏幕初始化 打印变量
TFT_eSPI tft = TFT_eSPI();
// 显示数组
char ShowArray[50] = {0};
// WiFi变量
WIFI MyWiFi;
// espnow通信的结构体
espnow_send_param_t espnow_send_param;
// 电量过低取反闪烁变量
bool temp_1 = false;
// 1s定时器句柄
TimerHandle_t xTimer_1s;

// 消息发送队列句柄
QueueHandle_t xQueue_espnow_send_handle;

// 摇杆 按键发送信息任务
void Rocker_Send_Task(void *pvParameters)
{
  char flag;
  while (1)
  {
    xQueueReceive(xQueue_espnow_send_handle, &flag, portMAX_DELAY);
    // 解锁
    if (flag == LOCK)
    {
      My_RC_UAV_Data.unlock = !My_RC_UAV_Data.unlock;
      My_UAV_Data.unlock = My_RC_UAV_Data.unlock;
      espnow_send_param.data = "lock-" + String(My_RC_UAV_Data.unlock);
      My_espnow_sendData(espnow_send_param);
    }
    // 增大油门
    if (flag == THROTTLE_ADD)
    {
      My_RC_UAV_Data.throttle_value += 50;
      if(My_RC_UAV_Data.throttle_value >= UAV_MAX)
      {
        My_RC_UAV_Data.throttle_value = UAV_MAX;
      }
      espnow_send_param.data = "VAX" + String(My_RC_UAV_Data.throttle_value) + '|';
      My_espnow_sendData(espnow_send_param);
    }
    // 减小油门
    else if (flag == THROTTLE_REDUCE)
    {
      if (My_RC_UAV_Data.throttle_value >= 99)
      {
        My_RC_UAV_Data.throttle_value -= 50;
      }
      if (My_RC_UAV_Data.throttle_value <= UAV_MIN)
      {
        My_RC_UAV_Data.throttle_value = UAV_MIN;
      }
      espnow_send_param.data = "VAX" + String(My_RC_UAV_Data.throttle_value) + '|';
      My_espnow_sendData(espnow_send_param);
    }
    // delay(100); // 延时100ms
  }
}

void Rocker_Send_Task(TimerHandle_t *xTimer)
{
  
}

void setup()
{
  Serial.begin(115200);
  tft.begin();
  tft.setRotation(3);                     // 显示方向
  tft.fillScreen(TFT_BLACK);              // 用黑色填充屏幕
  tft.setTextColor(TFT_WHITE, TFT_BLACK); // 将文本颜色设置为黑底白字
  tft.setCursor(0, 0);                    // 将光标位置设置为左上角
  tft.setTextSize(2);                     // 设置字号
  tft.printf("Hello World!");             // 显示

  Rocker_Init();                           // 初始化摇杆&&按键
  LED_Init();                              // 初始化LED
  VBAT_Init();                             // 初始化电池监测
  MyWiFi.WIFI_Connect("倾尽", "00000001"); // 连接WIFI
  My_espnow_Init();                        // 初始化espnow
  // //时间获取函数
  // My_GetTime.GetTime_Set();
  // My_GetTime.GetTime_Data_Analyze(My_GetTime.GetTime_Str());
  // 初始油门值
  My_RC_UAV_Data.throttle_value = 99;

  // 创建一个摇杆信息发送任务
  xQueue_espnow_send_handle = xQueueCreate(2, sizeof(char *));
  if (xQueue_espnow_send_handle == NULL)
  {
    Serial.println("创建队列失败");
    return;
  }
  // 1s定时器
  xTimer_1s = xTimerCreate("Rocker_Send_Timer", 1000 / portTICK_RATE_MS, pdFALSE, (void *)0, Rocker_Send_Task);
  if (xTimer_1s == NULL)
  {
    Serial.println("创建定时器失败");
    return;
  }
  if (xTimerStart(xTimer_1s, 0) != pdPASS)
  {
    Serial.println("启动定时器失败");
    return;
  }
  xTaskCreate(Rocker_Send_Task, "Rocker_Send_Task", 1024 * 2, NULL, 0, NULL);
  init_ok = false; // 初始化成功标志位
  Serial.println("初始化完成");
}

void loop()
{
  sprintf(ShowArray, "L X:%d   Y:%d   ", MyRockerData.rocker_Left_x, MyRockerData.rocker_Left_y);
  tft.setCursor(0, 20); // 设置光标位置
  tft.println(ShowArray);

  sprintf(ShowArray, "R X:%d   Y:%d   ", MyRockerData.rocker_Right_x, MyRockerData.rocker_Right_y);
  tft.setCursor(0, 40); // 设置光标位置
  tft.println(ShowArray);
  // 按键值显示
  sprintf(ShowArray, "Key->%d ", MyRockerData.key_num);
  tft.setCursor(0, 60); // 设置光标位置
  tft.println(ShowArray);
  // 解锁 低压显示
  sprintf(ShowArray, "unlock->%d VBAT->%.2lfV  ", My_UAV_Data.unlock, (float)My_UAV_Data.UAV_Battery_Voltage / 100.00f);
  tft.setCursor(0, 80); // 设置光标位置
  tft.println(ShowArray);
  // 陀螺仪数据显示
  sprintf(ShowArray, "Pitch->%.3d  ", (int16_t)My_UAV_Data.UAV_MPU6050_Data.Pitch);
  tft.setCursor(0, 100); // 设置光标位置
  tft.println(ShowArray);
  sprintf(ShowArray, "Roll->%.3d  ", (int16_t)My_UAV_Data.UAV_MPU6050_Data.Roll);
  tft.setCursor(0, 120); // 设置光标位置
  tft.println(ShowArray);
  sprintf(ShowArray, "Yaw->%.3d   ", (int16_t)My_UAV_Data.UAV_MPU6050_Data.Yaw);
  tft.setCursor(0, 140); // 设置光标位置
  tft.println(ShowArray);
  // 油门值
  sprintf(ShowArray, "Throttle_value->%d      ", My_UAV_Data.Throttle_value);
  tft.setCursor(0, 160); // 设置光标位置
  tft.println(ShowArray);

  //低压交响
  if (My_UAV_Data.UAV_Battery_Voltage <= 380 && init_ok && My_UAV_Data.UAV_Battery_Voltage != 0)
  {
    temp_1 = !temp_1;
    LED_Buzzer(Buzzer_Pin,temp_1);
  }
  else LED_Buzzer(Buzzer_Pin,1);
    

  // 长按左上角按键解锁
  if (MyRockerData.key_num == UP_Left_Long_Press)
  {
    char flag = LOCK;
    xQueueSend(xQueue_espnow_send_handle, &flag, 0);
  }
  // 摇杆控制油门
  if (MyRockerData.rocker_Right_x <= 10 && MyRockerData.rocker_Right_y > 1900 && MyRockerData.rocker_Right_y < 2100)
  {
    char flag = THROTTLE_ADD;
    xQueueSend(xQueue_espnow_send_handle, &flag, 0);
  }
  else if (MyRockerData.rocker_Right_x > 4090 && MyRockerData.rocker_Right_y > 1900 && MyRockerData.rocker_Right_y < 2100)
  {
    char flag = THROTTLE_REDUCE;
    xQueueSend(xQueue_espnow_send_handle, &flag, 0);
  }
  delay(100); // 延时100ms
}
