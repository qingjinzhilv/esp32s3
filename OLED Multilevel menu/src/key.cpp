#include "key.h"

void key_GetNum(uint8_t *key)
{
    *key = 0;
    if(digitalRead(D2) == 0)
    {
        delay(20);
        if(digitalRead(D2) == 0)
        {
            *key = 2;
        }
    }

    if(digitalRead(D3) == 0)
    {
        delay(20);
        if(digitalRead(D3) == 0)
        {
            *key = 3;
        }
    }

    if(digitalRead(D4) == 0)
    {
        delay(20);
        if(digitalRead(D4) == 0)
        {
            *key = 4;
        }
    }

    if(digitalRead(D5) == 0)
    {
        delay(20);
        if(digitalRead(D5) == 0)
        {
            *key = 5;
        }
    }
}

