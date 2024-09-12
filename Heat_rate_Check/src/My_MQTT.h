#ifndef __MYMQTT_H_
#define __MYMQTT_H_

#include "main.h"


void MQTT_Connect(void);
void Rx_Pro(char *topic,byte *payload,unsigned int length);
void Data_Analysis(String Data);
void MQTT_Keep(void);

void InFormation_double_Pub(char Pub[],double data);
void InFormation_int_Pub(char Pub[],int data);

#endif