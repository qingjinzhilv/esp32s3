#ifndef MAX9814_H_
#define MAX9814_H_

#include <Arduino.h>
#include "base64.h"
#include "HTTPClient.h"
#include "cJSON.h"
#include "ArduinoJson.h"

void IRAM_ATTR onTimer();
void gain_token(void);

void MAX9814_init(void);
String MAX9814_speech_recognition(void);
// void MAX9814_speech_recognition(void);


String uint16ArrayToString(const uint16_t* array, size_t size);

#endif /* MAX9814_H_ */
