#include "main.h"

// SCL -> D1
// SDA -> D2

uint8_t menu = 0,show_flag = 1;
uint8_t line = 2;
typedef struct main
{
  uint8_t next;
  char *data;
}main;
main oled_menu[10] = 
                 {{1,"      MAIN      "},
                  {2,"   powerstate   "},
                  {3,"   brightness   "},
                  {4,"     Alarm      "},
                  {5,"   learn_Timr   "},
                  {6,"   learn_Timr1   "},
                  {1,"   learn_Timr2   "},
                 };


char showArray[30];

void PinInit(void)
{
  pinMode(D2,INPUT_PULLUP);
  pinMode(D3,INPUT_PULLUP);
  pinMode(D4,INPUT_PULLUP);
  pinMode(D5,INPUT_PULLUP);
}



void OLED_menu(void)
{
   OLED_ShowString(1,1,oled_menu[0].data);
   int k = menu;
  for(int i = 2;i < 5;i ++)
  {
    OLED_ShowString(i,1,oled_menu[menu].data);
    menu = oled_menu[menu].next;
  }
  menu = k;
}

void Line_change(void)
{
  OLED_ShowChar(1,1,' ');
  OLED_ShowChar(1,2,' ');
  OLED_ShowChar(2,1,' ');
  OLED_ShowChar(2,2,' ');
  OLED_ShowChar(3,1,' ');
  OLED_ShowChar(3,2,' ');
  OLED_ShowChar(4,1,' ');
  OLED_ShowChar(4,2,' ');
  if(line)
  {
    OLED_ShowChar(line,1,'-');
    OLED_ShowChar(line,2,'>');
  }
}

void Subinterface(void)
{
  int k = menu;
  if(menu == 1)
  {
    if(line == 2)
    {
      menu = 5;
    }
    if(line == 3)
    {
      menu = 6;
    }
  }
  for(int i = 1;i < 3;i ++)
  {
    menu = oled_menu[menu].next;
  }

  switch(menu)
  {
    case 1:
      OLED_ShowString(1,1,oled_menu[menu].data);
      OLED_ShowString(2,1,(char *)"     one     ");
    break;
    case 2:
      OLED_ShowString(1,1,oled_menu[menu].data);
      OLED_ShowString(2,1,"     Two     ");
    break;
    case 3:
      OLED_ShowString(1,1,oled_menu[menu].data);
      OLED_ShowString(2,1,"     Three     ");
    break;
    case 4:
      OLED_ShowString(1,1,oled_menu[menu].data);
      OLED_ShowString(2,1,"     Four      ");
    break;
    case 5:
      OLED_ShowString(1,1,oled_menu[menu].data);
      OLED_ShowString(2,1,"     Five      ");
    break;
    case 6:
      OLED_ShowString(1,1,oled_menu[menu].data);
      OLED_ShowString(2,1,"     Six      ");
    break;
  }
  menu = k;
}

void key_pro(void)
{
    static uint8_t key_value = 0,key = 0;
    key_GetNum(&key);
    if(key != key_value)
    {
        key_value = key;
    }
    else
    {
        key = 0;
    }
    switch (key)
    {
        case 2:
          if(show_flag == 1)
          {
            line ++;
            if(line > 4)
            {
              line = 4;
              menu = oled_menu[menu].next;
            }
            Serial.printf("%d\r\n",menu);
          }
          else if(show_flag == 2)
          {
            show_flag = 1;
          }
          OLED_menu();
          Line_change();
        break;

        case 3:
            show_flag = 2;
            OLED_Clear();
            Subinterface();
        break;

        case 4:
            
        break;

        case 5:
            
        break;
    
        default:

        break;
    }
}


void setup()
{
  Serial.begin(115200);

  PinInit();
  
  OLED_Init();
  OLED_Clear();

  menu = oled_menu[0].next;

  OLED_menu();
  Line_change();
}

void loop() 
{
  key_pro();
}



