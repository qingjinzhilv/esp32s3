#include "rocker.h"

uint8_t key_flag = 0, Right_count = 0, Left_count = 0, UP_Right_count = 0, UP_Left_count = 0;
// 摇杆数据
rocker_Data MyRockerData;

void Rocker_Task(void *pvParameters)
{
    while (1)
    {
        Rocker_GetData(&MyRockerData);
        vTaskDelay(100 / portTICK_RATE_MS);
    }
}

void Rocker_Init(void)
{
    pinMode(Right_VRX,ANALOG);
    pinMode(Right_VRY,ANALOG);
    pinMode(Left_VRX,ANALOG);
    pinMode(Left_VRY,ANALOG);
    analogReadResolution(12);

    pinMode(Left_SW,INPUT_PULLUP);
    pinMode(Right_SW,INPUT_PULLUP);
    pinMode(Key_UP_LEFT,INPUT_PULLUP);
    pinMode(Key_UP_RIGHT,INPUT_PULLUP);

    xTaskCreate(Rocker_Task, "Rocker_Task", 1024 * 2, NULL, 1, NULL);
}

void Rocker_GetData(rocker_Data *rocker_data)
{
    static uint8_t SW_Value = 0;
    rocker_data->rocker_Right_x = analogRead(Right_VRX);
    rocker_data->rocker_Right_y = analogRead(Right_VRY);

    rocker_data->rocker_Left_x = analogRead(Left_VRX);
    rocker_data->rocker_Left_y = analogRead(Left_VRY);


    //上左按键
    if (digitalRead(Key_UP_LEFT) == 0)
    {
        delay(20);
        if (digitalRead(Key_UP_LEFT) == 0)
        {
            if (++UP_Left_count > 9)
            {
                rocker_data->key_num = 30;
            }
        }
    }
    else
    {
        if (UP_Left_count > 0 && UP_Left_count <= 9)
        {
            rocker_data->key_num = 3;
        }
        UP_Left_count = 0;
    }
    //上右按键
    if (digitalRead(Key_UP_RIGHT) == 0)
    {
        delay(20);
        if (digitalRead(Key_UP_RIGHT) == 0)
        {
            if (++UP_Right_count > 9)
            {
                rocker_data->key_num = 40;
            }
        }
    }
    else
    {
        if (UP_Right_count > 0 && UP_Right_count <= 9)
        {
            rocker_data->key_num = 4;
        }
        UP_Right_count = 0;
    }
    //左摇杆按键
    if(digitalRead(Right_SW) == 0)
    {
        delay(20);
        if (digitalRead(Right_SW) == 0)
        {
            if (++Right_count > 9)
            {
                rocker_data->key_num = 10;
            }
        }
    }
    else
    {
        if (Right_count > 0 && Right_count <= 9)
        {
            rocker_data->key_num = 1;
        }
        Right_count = 0;
    }
    //右摇杆按键
    if (digitalRead(Left_SW) == 0)
    {
        delay(20);
        if (digitalRead(Left_SW) == 0)
        {
            if (++Left_count > 9)
            {
                rocker_data->key_num = 20;
            }
        }
    }
    else
    {
        if (Left_count > 0 && Left_count <= 9)
        {
            rocker_data->key_num = 2;
        }
        Left_count = 0;
    }
    
    // if (rocker_data->key_num < 10)
    // {
        if (SW_Value != rocker_data->key_num)
        {
            SW_Value = rocker_data->key_num;
        }
        else
        {
            rocker_data->key_num = 0;
        }
    // }
}
