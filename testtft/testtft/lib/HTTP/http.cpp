#include "http.h"

// #define DEBUG //选择是否打印Http日志 注释后将仅打印错误消息

String Http_Get(const char *ip)
{
    String payload;
    HTTPClient http;
#ifdef DEBUG
    Serial.print("[HTTP] begin...\n");
    Serial.print(ip);
    Serial.print("\n");
#endif
    
    http.begin(ip); // 访问服务器地址
#ifdef DEBUG
    Serial.print("[HTTP] GET...\n");
#endif
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0)
    {
// HTTP header has been send and Server response header has been handled
#ifdef DEBUG
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
#endif
        // file found at server
        if (httpCode == HTTP_CODE_OK)
        {
            payload = http.getString();
#ifdef DEBUG
            Serial.println(payload);
#endif
        }
    }
    else
    {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
    return payload;
}
String Http_Post(const char *ip)
{
    // todo
    return "没写业务代码有个🔨返回值";
}