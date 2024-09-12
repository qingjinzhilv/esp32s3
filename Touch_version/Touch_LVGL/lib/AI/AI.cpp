#include "AI.h"

// 替换为 access_token
const char* access_token = "24.4a3c793118da3909e35107104fa84db6.2592000.1728454945.282335-115514263";
String apiUrl = "https://aip.baidubce.com/rpc/2.0/ai_custom/v1/wenxinworkshop/chat/completions?access_token=" + String(access_token);



String getGPTAnswer(String inputText) 
{
  WiFiClientSecure client;
  client.setInsecure();  // 关闭 SSL 证书验证
  
  HTTPClient http;
  http.setTimeout(Time_Out);  // 超时时间设置
  
  // // 调试信息
  // Serial.println("Connecting to API...");
  // Serial.println("API URL: " + apiUrl);

  http.begin(client, apiUrl);
  http.addHeader("Content-Type", "application/json");

  String payload = "{\"messages\":[{\"role\": \"user\",\"content\": \"" + inputText + "\"}],\"disable_search\": false,\"enable_citation\": false}";
  // Serial.println("Payload: " + payload);  // 调试信息

  int httpResponseCode = http.POST(payload);

  // // 调试信息
  // Serial.printf("HTTP Response code: %d\n", httpResponseCode);

  if (httpResponseCode == HTTP_CODE_OK) 
  {
    String response = http.getString();
    http.end();
    // Serial.println("Response: " + response);  // 调试信息

    // Parse JSON response
    DynamicJsonDocument jsonDoc(1024);
    deserializeJson(jsonDoc, response);
    String outputText = jsonDoc["result"].as<String>();
    return outputText;
  } 
  else 
  {
    http.end();
    Serial.printf("Error %d\n", httpResponseCode);
    return "<error>";
  }
}
