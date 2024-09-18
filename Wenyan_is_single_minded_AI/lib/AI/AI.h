#ifndef AI_H_
#define AI_H_

#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define Time_Out 60000  //超时时间

//文言一心AI
String getGPTAnswer(String inputText);


#endif /* AI_H_ */
