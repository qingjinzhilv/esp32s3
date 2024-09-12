#include "rocker.h"

uint8_t key_flag = 0,Right_count = 0,Left_count = 0;

void Rocker_Init(void)
{
    pinMode(Left_SW,INPUT_PULLUP);

    pinMode(Right_SW,INPUT_PULLUP);
}

void Rocker_GetData(uint32_t *ADC_Data,uint8_t *SW)
{
    static _Bool SW_Value[2] = {0};
    SW[0] = 0;
    SW[1] = 0;
    ADC_Data[0] = analogRead(Right_VRX);
    ADC_Data[1] = analogRead(Right_VRY);

    ADC_Data[2] = analogRead(Left_VRX);
    ADC_Data[3] = analogRead(Left_VRY);

    if(key_flag)
    {
        key_flag = 0;
        if(digitalRead(Right_SW) == 0)
        {
            delay(20);
            if(digitalRead(Right_SW) == 0)
            {
                if(++ Right_count > 9)
                {
                    SW[0] = 10;
                }
            }
        }
        else
        {
            if(Right_count > 0 && Right_count <= 9)
            {
                SW[0] = 1;
            }
            Right_count = 0;
        }
        if(digitalRead(Left_SW) == 0)
        {
            delay(20);
            if(digitalRead(Left_SW) == 0)
            {
                if(++ Left_count > 9)
                {
                    SW[1] = 10;
                }
                
            }
        }
        else
        {
            if(Left_count > 0 && Left_count <= 9)
            {
                SW[1] = 1;
            }
            Left_count = 0;
        }
    }
    if(SW[0] < 10)
    {
        if(SW_Value[0] != SW[0])
        {
            SW_Value[0] = SW[0];
        }
        else
        {
            SW[0] = 0;
        }
    }
    if(SW[1] < 10)
    {
        if(SW_Value[1] != SW[1])
        {
            SW_Value[1] = SW[1];
        }
        else
        {
            SW[1] = 0;
        }
    }
}


