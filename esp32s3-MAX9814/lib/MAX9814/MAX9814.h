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
void MAX9814_speech_recognition(void);


#endif /* MAX9814_H_ */
