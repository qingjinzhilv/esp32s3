#ifndef __WEATHER_H_
#define __WEATHER_H_

#include "main.h"
#define reqLocation  "Hunan changsha"     // 城市

void Buil_Request(void);
void httpRequest(String reqRes);
void parseInfo(WiFiClient client);

#endif
