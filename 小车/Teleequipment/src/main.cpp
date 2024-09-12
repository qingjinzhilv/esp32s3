#include <Arduino.h>
#include <OLED.h>
#include "My_espnow.h"
#include "rocker.h"

//开始任务
TaskHandle_t Start_task_handler;
//OLED显示任务
TaskHandle_t OLED_task_handler;
//消息接收
TaskHandle_t Inforation_handler;
//输入MAC
TaskHandle_t MAC_Input_handler;
//软件定时器
TimerHandle_t Time_100ms_handle;
//小车控制函数句柄
TaskHandle_t Car_handler;
//队列句柄
QueueHandle_t Car_Queue;


void Start_task(void *pv);
void OLED(void *pv);
void Inforation(void *pv);
void Input_MAC(void *pv);
void Timer_100ms(TimerHandle_t pxTime);
void Show_Hint(void);
void Car_Control(void *pv);

uint32_t Data[4] = {0};
uint8_t SW_Data[2] = {0};
char ShowArray[30];

int8_t MAC_Location = 0;
_Bool rocker_flag = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(06,OUTPUT);
  pinMode(07,OUTPUT);
  // digitalWrite(06,1);
  OLED_Init();
  OLED_Clear();
  // espnow_Init();
  Rocker_Init();
  OLED_ShowString(1,1,(char *)"Start Start");
  xTaskCreate((TaskFunction_t)Start_task,
              (char *)"start_task",
              (uint32_t)8 * 1024,
              NULL,
              (UBaseType_t)1,
              &Start_task_handler
              );
  // vTaskStartScheduler();
  OLED_ShowString(1,1,(char *)"end end");
  OLED_Clear();
}


void loop()
{
}

void Start_task(void *pv)
{
  //进入临界区
  vPortEnterCritical();
  Time_100ms_handle = xTimerCreate("Timer",100,pdTRUE,(void *)1,Timer_100ms);
  if(Time_100ms_handle == NULL)
  {
    OLED_ShowString(1,1,(char *)"Timer ERROR");
    return;
  }
  xTimerStart(Time_100ms_handle,0);
  //创建队列
  Car_Queue = xQueueCreate(1,sizeof(int));
  if(Car_Queue != NULL)
  {
    Serial.println("Car Queue Successed!");
  }
  else
  {
    return;
  }
  xTaskCreate(OLED,
              "OLED",
              8 * 1024,
              NULL,
              3,
              &OLED_task_handler
              );
  vTaskSuspend(OLED_task_handler);
  xTaskCreate(Inforation,
              "Inforation",
              8 * 1024,
              NULL,
              3,
              &Inforation_handler);
  vTaskSuspend(Inforation_handler);
  xTaskCreate(Input_MAC,
              "Input_MAC",
              8 * 1024,
              NULL,
              3,
              &MAC_Input_handler);
  xTaskCreate(Car_Control,
              "Car_Control",
              8 * 1024,
              NULL,
              3,
              &Car_handler);
  vTaskSuspend(Car_handler);
  //删除任务
  vTaskDelete(Start_task_handler);
  //恢复任务
  vTaskResume(Inforation_handler);
  OLED_Clear();
  //退出临界区
  vPortExitCritical();
}

void OLED(void *pv)
{
  int Car_Dir = 0;
  while(1)
  {
    // OLED_ShowNum(2,1,Data[0],4);
    // OLED_ShowNum(2,8,Data[1],4);
    // OLED_ShowNum(3,1,Data[2],4);
    // OLED_ShowNum(3,8,Data[3],4);
    // OLED_ShowNum(4,1,SW_Data[0],1);
    // OLED_ShowNum(4,8,SW_Data[1],1);
    //MAC显示
    sprintf(ShowArray,"MAC->%.2X:%.2X:%.2X:%.2X",broadcastAddress[0],broadcastAddress[1],broadcastAddress[2],broadcastAddress[3]);
    OLED_ShowString(3,1,ShowArray);
    sprintf(ShowArray,"%.2X:%.2X",broadcastAddress[4],broadcastAddress[5]);
    OLED_ShowString(4,1,ShowArray);
    //前进
    if(Data[0] < 50 && Data[1] > 3500 && Data[1] < 3650)
    {
      Car_Dir = 1;
      BaseType_t Car_err = xQueueSend(Car_Queue,&Car_Dir,0);
      if(Car_err == pdFALSE)
      {
        OLED_ShowString(4,1,(char *)"U_Queue Error");
        delay(250);
        OLED_ShowString(4,1,(char *)"               ");
      }
      OLED_ShowString(2,1,(char *)"       UP       ");
    }
    //左走
    else if(Data[0] < 3600 && Data[0] > 3500 && Data[1] > 4000)
    {
      Car_Dir = 2;
      BaseType_t Car_err = xQueueSend(Car_Queue,&Car_Dir,0);
      if(Car_err == pdFALSE)
      {
        
        OLED_ShowString(4,1,(char *)"L_Queue Error");
        delay(250);
        OLED_ShowString(4,1,(char *)"               ");
      }
      OLED_ShowString(2,1,(char *)"      Left      ");
    }
    //后退
    else if(Data[0] > 4000 && Data[1] > 3500 && Data[1] < 3650)
    {
      Car_Dir = 3;
      BaseType_t Car_err = xQueueSend(Car_Queue,&Car_Dir,0);
      if(Car_err == pdFALSE)
      {
        OLED_ShowString(4,1,(char *)"B_Queue Error");
        delay(250);
        OLED_ShowString(4,1,(char *)"               ");
      }
      OLED_ShowString(2,1,(char *)"      Down      ");
    }
    //右走
    else if(Data[0] < 3600 && Data[0] > 3500 && Data[1] < 50)
    {
      Car_Dir = 4;
      BaseType_t Car_err = xQueueSend(Car_Queue,&Car_Dir,0);
      if(Car_err == pdFALSE)
      {
        OLED_ShowString(4,1,(char *)"R_Queue Error");
        delay(250);
        OLED_ShowString(4,1,(char *)"               ");
      }
      OLED_ShowString(2,1,(char *)"      Right     ");
    }
    //停止
    else if(Data[0] > 3500 && Data[0] < 3700 && Data[1] > 3500 && Data[1] < 3700)
    {
      Car_Dir = 5;
      BaseType_t Car_err = xQueueSend(Car_Queue,&Car_Dir,0);
      if(Car_err == pdFALSE)
      {
        OLED_ShowString(4,1,(char *)"R_Queue Error");
        delay(250);
        OLED_ShowString(4,1,(char *)"               ");
      }
      OLED_ShowString(2,1,(char *)"      Stop      ");
    }
    delay(100);
  }
}

void Inforation(void *pv)
{
  while(1)
  {
    espnow_Serial_Send();
    Rocker_GetData(Data,SW_Data);
    espnow_Serial_Send();
    if(send_flag)
    {
      send_flag = 0;
      OLED_Clear();
      vTaskResume(MAC_Input_handler);
      vTaskSuspend(OLED_task_handler);
    }
    
    delay(10);
  }
}

void Input_MAC(void *pv)
{
  while(1)
  {
    sprintf(ShowArray,"MAC->%.2X:%.2X:%.2X:%.2X",broadcastAddress[0],broadcastAddress[1],broadcastAddress[2],broadcastAddress[3]);
    OLED_ShowString(1,1,ShowArray);
    sprintf(ShowArray,"%.2X:%.2X",broadcastAddress[4],broadcastAddress[5]);
    OLED_ShowString(2,1,ShowArray);
    // OLED_ShowNum(1,1,Data[0],4);
    // OLED_ShowNum(1,8,Data[1],4);
    // OLED_ShowNum(2,1,Data[2],4);
    // OLED_ShowNum(2,8,Data[3],4);
    // OLED_ShowNum(3,1,SW_Data[0],2);
    // OLED_ShowNum(3,8,SW_Data[1],2);
    if(Data[0] < 3600 && Data[0] > 3500 && Data[1] < 10)
    {
      MAC_Location ++;
      if(MAC_Location > 5)
      {
        MAC_Location = 5;
      }
    }
    else if(Data[0] < 3600 && Data[0] > 3500 && Data[1] == 4095)
    {
      MAC_Location --;
      if(MAC_Location < 0)
      {
        MAC_Location = 0;
      }
    }
    if(Data[0] < 10 && Data[1] > 3550 && Data[1] < 3650)
    {
      broadcastAddress[MAC_Location] += 0x01;
    }
    else if(Data[0] == 4095 && Data[1] > 3560 && Data[1] < 3660)
    {
      broadcastAddress[MAC_Location] -= 0x01;
    }
    if(SW_Data[0] == 10 && SW_Data[1] == 10)
    {
      if(espnow_Init(broadcastAddress))
      {
        OLED_Clear();
        OLED_ShowString(1,1,(char *)"   Connected!   ");
        vTaskResume(OLED_task_handler);
        vTaskResume(Car_handler);
        vTaskSuspend(MAC_Input_handler);
      }
      else
      {
        OLED_Clear();
        OLED_ShowString(1,1,(char *)"   disonnect   ");
        delay(1000);
        OLED_Clear();
      }
    }
    Show_Hint();
    delay(110);
  }
}
//显示提示函数
void Show_Hint(void)
{
  OLED_ShowString(2,6,(char *)"           ");
  OLED_ShowString(3,1,(char *)"     ");
  switch(MAC_Location)
  {
    case 0:
      OLED_ShowString(2,6,(char *)"^^");
      
    break;
    case 1:
      OLED_ShowString(2,9,(char *)"^^");
    break;
    case 2:
      OLED_ShowString(2,12,(char *)"^^");
    break;
    case 3:
      OLED_ShowString(2,15,(char *)"^^");
    break;
    case 4:
      OLED_ShowString(3,1,(char *)"^^");
    break;
    case 5:
      OLED_ShowString(3,4,(char *)"^^");
    break;
  }
}

void Car_Control(void *pv)
{
  int rec_Dir;
  static int8_t last_rec_Dir = 0;
  BaseType_t err;
  while(1)
  {
    if(uxQueueMessagesWaiting(Car_Queue))
    {
      err = xQueueReceive(Car_Queue,&rec_Dir,0);
      if(err == pdTRUE)
      {
        Serial.println(rec_Dir);
        if(last_rec_Dir != rec_Dir)
        {
          last_rec_Dir = rec_Dir;
          espnow_sendData(rec_Dir,"");
        }
        else
        {
          rec_Dir = 0;
        }
        
      }
      else
      {
        Serial.println("Car Receive ERROR");
      }
    }
    delay(10);
  }
}

void Timer_100ms(TimerHandle_t xTime)
{
  key_flag ++;
}