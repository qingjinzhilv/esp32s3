#ifndef AI_H_
#define AI_H_

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

extern String AI_Data_Str;
extern String AI_Data_Str_child;

//文言一心AI
String getGPTAnswer(String inputText);
String getAnswer(String* lines) ;

#endif /* AI_H_ */
