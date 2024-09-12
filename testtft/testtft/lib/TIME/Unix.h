#ifndef __UNIX_H__
#define __UNIX_H__
#include <Arduino.h>
#include <stdio.h>
#include <time.h>

void Unix_To_Time(time_t UnixNum, char *buffer);
void ShowTime(struct tm *timeinfo, int j, char *buffer);

#endif
