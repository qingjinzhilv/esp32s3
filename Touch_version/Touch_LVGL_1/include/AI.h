#ifndef AI_H_
#define AI_H_

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "lvgl.h"
#include "ui.h"
#include "AI.h"

#define Time_Out 60000  //超时时间

extern String AI_Data_Str;
extern String AI_Data_Str_child;
//文言一心AI
String getGPTAnswer(String inputText);


#endif /* AI_H_ */
