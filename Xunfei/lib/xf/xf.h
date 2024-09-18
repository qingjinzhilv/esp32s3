#ifndef _XF_H_
#define _XF_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Arduino.h"
#include "HTTPClient.h"
#include "WiFiClientSecure.h"
#include "ArduinoJson.h"

extern String *AI_answer;
extern String str;
String sendRequest(String inputText);



#endif
