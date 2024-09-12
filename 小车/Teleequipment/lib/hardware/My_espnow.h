#ifndef __MY_ESPNOW_H_
#define __MY_ESPNOW_H_

#include "Arduino.h"
extern uint8_t broadcastAddress[6];
extern uint8_t send_flag;

uint8_t espnow_Init(uint8_t MAC[]);
void espnow_sendData(int int_data,String str_data);
void espnow_Serial_Send(void);


#endif
