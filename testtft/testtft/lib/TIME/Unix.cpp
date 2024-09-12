#include <Arduino.h>
#include <stdio.h>
#include <time.h>
#include "Unix.h"
void Unix_To_Time(time_t UnixNum,char *buffer)
{	
	struct tm *timeinfo;
	timeinfo = localtime(&UnixNum);
	ShowTime(timeinfo,8,buffer);
}
void ShowTime(struct tm * timeinfo,int j,char *buffer)
{
int day,hour;
if(j>0)
{
	if((j+timeinfo->tm_hour)/24==1)
		day=timeinfo->tm_mday+1;
	else
		day=timeinfo->tm_mday;
	timeinfo->tm_hour=(j+timeinfo->tm_hour)%24;
}
if(j==0)
{
	day=timeinfo->tm_mday;
	timeinfo->tm_hour=(j+timeinfo->tm_hour)%24;
}
if(j<0)
{
	if((j+timeinfo->tm_hour)/24<0)
		day=timeinfo->tm_mday-1;
	else
		day=timeinfo->tm_mday;
	timeinfo->tm_hour=(j+timeinfo->tm_hour)%24;
 }
 strftime(buffer,256,"%H:%M:%S",timeinfo);
 //printf( "%4d-%02d-%02d %02d:%02d:%02d\n",1900+timeinfo->tm_year, 1+timeinfo->tm_mon,day,hour,timeinfo->tm_min,timeinfo->tm_sec);
}
