#ifndef AI_BEAN_BUNS_H
#define AI_BEAN_BUNS_H

#include <Arduino.h>
#include "ArduinoJson.h"
#include "HTTPClient.h"
#include "WiFi.h"
#include "WiFiClientSecure.h"
#include "base64.h"

extern String str;

extern String sum_str;
void getGPTAnswer(String inputText);

#endif
