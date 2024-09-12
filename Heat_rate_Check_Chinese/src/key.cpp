#include "key.h"

uint8_t key_flag = 0,key_count1 = 0,duble_clik1 = 0,dtime1 = 0;
uint8_t key_count2 = 0,duble_clik2 = 0,dtime2 = 0;
uint8_t key_count3 = 0,duble_clik3 = 0,dtime3 = 0;
uint8_t key_count4 = 0,duble_clik4 = 0,dtime4 = 0;

void key_GetNum(uint8_t *key)
{
    *key = 0;
    if(dtime1 > 3)
    {
        if(duble_clik1 == 1)     //单击
        {
            *key = 1;
        }
        else                    //双击
        {
            *key = 5;
        }
        dtime1 = 0;
        duble_clik1 = 0;
    }

    if(dtime2 > 3)
    {
        if(duble_clik2 == 1)     //单击
        {
            *key = 2;
        }
        else                    //双击
        {
            *key = 6;
        }
        dtime2 = 0;
        duble_clik2 = 0;
    }

    if(dtime3 > 3)
    {
        if(duble_clik3 == 1)     //单击
        {
            *key = 3;
        }
        else                    //双击
        {
            *key = 7;
        }
        dtime3 = 0;
        duble_clik3 = 0;
    }

    if(dtime4 > 3)
    {
        if(duble_clik4 == 1)     //单击
        {
            *key = 4;
        }
        else                    //双击
        {
            *key = 8;
        }
        dtime4 = 0;
        duble_clik4 = 0;
    }

    if(key_flag > 0)
    {
        if(digitalRead(1) == 0)
        {
            delay(10);
            if(digitalRead(1) == 0)
            {
                if(++ key_count1 >= 10)
                {
                    *key = 10;
                }
                
            }
        }
        else
        {
            if(key_count1 > 0 && key_count1 < 10)
            {
                duble_clik1 ++;
            }
            key_count1 = 0;
        }

        if(digitalRead(2) == 0)
        {
            delay(10);
            if(digitalRead(2) == 0)
            {
                if(++ key_count2 >= 10)
                {
                    *key = 20;
                }
                
            }
        }
        else
        {
            if(key_count2 > 0 && key_count2 < 10)
            {
                duble_clik2 ++;
            }
            key_count2 = 0;
        }

        if(digitalRead(3) == 0)
        {
            delay(10);
            if(digitalRead(3) == 0)
            {
                if(++ key_count3 >= 10)
                {
                    *key = 30;
                }
                
            }
        }
        else
        {
            if(key_count3 > 0 && key_count3 < 10)
            {
                duble_clik3 ++;
            }
            key_count3 = 0;
        }

        if(digitalRead(4) == 0)
        {
            delay(10);
            if(digitalRead(4) == 0)
            {
                if(++ key_count4 >= 10)
                {
                    *key = 40;
                }
                
            }
        }
        else
        {
            if(key_count4 > 0 && key_count4 < 10)
            {
                duble_clik4 ++;
            }
            key_count4 = 0;
        }
    }

    
    
}

void Timer_2(void)
{
    key_flag ++;
    if(duble_clik1) dtime1 ++;
    if(duble_clik2) dtime2 ++;
    if(duble_clik3) dtime3 ++;
    if(duble_clik4) dtime4 ++;
}

