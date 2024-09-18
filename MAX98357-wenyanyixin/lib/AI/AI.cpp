#include "AI.h"

// 替换为 access_token
const char* access_token = "24.4a3c793118da3909e35107104fa84db6.2592000.1728454945.282335-115514263";
String apiUrl = "https://aip.baidubce.com/rpc/2.0/ai_custom/v1/wenxinworkshop/chat/completions?access_token=" + String(access_token);


// String getGPTAnswer(String inputText) 
// {
//   WiFiClientSecure client;
//   client.setInsecure();  // 关闭 SSL 证书验证
  
//   HTTPClient http;
//   // http.setTimeout(Time_Out);  // 超时时间设置
  
//   // // 调试信息
//   // Serial.println("Connecting to API...");
//   // Serial.println("API URL: " + apiUrl);

//   http.begin(client, apiUrl);
//   http.addHeader("Content-Type", "application/json");

//   String payload = "{\"messages\":[{\"role\": \"user\",\"content\": \"" + inputText + "\"}],\"stream\": true,\"disable_search\": false,\"enable_citation\": false}";
//   // Serial.println("Payload: " + payload);  // 调试信息

//   int httpResponseCode = http.POST(payload);

//   // // 调试信息
//   // Serial.printf("HTTP Response code: %d\n", httpResponseCode);
//   if(httpResponseCode == HTTP_CODE_OK) 
//   {
//     String response = http.getString();
//     http.end();
//     // Serial.println("Response: " + response);  // 调试信息

//     // 解析 JSON 响应
//     DynamicJsonDocument jsonDoc(1024);
//     deserializeJson(jsonDoc, response);
//     String outputText = jsonDoc["result"].as<String>();
//     Serial.println(outputText);
//     return outputText;
//   } 
//   else 
//   {
//     http.end();
//     Serial.printf("Error %d\n", httpResponseCode);
//     return "<error>";
//   }
// } //


String AI_Data_Str;
String AI_Data_Str_child;
String getGPTAnswer(String inputText) 
{
  AI_Data_Str = "";
  WiFiClientSecure client;
  client.setInsecure();  // 关闭 SSL 证书验证

  HTTPClient http;
  http.begin(client, apiUrl);
  http.addHeader("Content-Type", "application/json");

  String payload = "{\"messages\":[{\"role\": \"user\",\"content\": \"" + inputText + "\"}],\"stream\": true,\"disable_search\": false,\"enable_citation\": false}";
  int httpResponseCode = http.POST(payload);


  if (httpResponseCode == HTTP_CODE_OK) 
  {
    WiFiClient* stream = http.getStreamPtr();  // 使用 WiFiClient*

    bool isEnd = false;

    while(stream->connected() && !isEnd) 
    {
      if (stream->available()) 
      {
        String line = stream->readStringUntil('\n');
        if (line.length() > 0) 
        {
          // 解析当前行
          if (line.startsWith("data: ")) 
          {
            line.remove(0, 6);  // 移除 "data: " 前缀
            DynamicJsonDocument jsonDoc(1024);
            DeserializationError error = deserializeJson(jsonDoc, line);

            if (error) 
            {
              Serial.print(F("deserializeJson() failed: "));
              Serial.println(error.c_str());
              continue;
            }
            String partResult = jsonDoc["result"].as<String>();
            AI_Data_Str += partResult;
            AI_Data_Str_child = partResult;
            Serial.println("Partial Result: " + partResult);  // 实时打印部分结果

            isEnd = jsonDoc["is_end"].as<bool>();
            if(isEnd) 
            {
              Serial.println("End of response.");
            }
          }
        }
      }
    }
    http.end();
  } 
  else 
  {
    Serial.printf("Error %d\n", httpResponseCode);
    http.end();
    AI_Data_Str = "<error>";
  }
  return AI_Data_Str;
}

