#ifndef __WEATHER_H_
#define __WEATHER_H_

#include "Arduino.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

extern char *reqLocation;     // 城市

void Buil_Request(void);
void httpRequest(String reqRes);
void parseInfo(WiFiClient client);

#endif
