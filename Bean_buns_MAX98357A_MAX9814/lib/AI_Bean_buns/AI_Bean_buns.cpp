#include "AI_Bean_buns.h"

// 替换为您的 OpenAI API 密钥
const char *doubao_apiKey = "ddc306c5-5e52-4fc8-8640-6b7e5d106011";

String inputText = "你好，豆包！";
String apiUrl = "https://ark.cn-beijing.volces.com/api/v3/chat/completions";

// String answer;
// String getGPTAnswer(String inputText)
// {
//   HTTPClient http;
//   http.setTimeout(20000);
//   http.begin(apiUrl);
//   http.addHeader("Content-Type", "application/json");
//   String token_key = String("Bearer ") + doubao_apiKey;
//   http.addHeader("Authorization", token_key);
//   String payload = "{\"model\":\"ep-20240914184638-mp4kq\",\"messages\":[{\"role\":\"user\",\"content\":\"" + inputText + "\"}],\"temperature\": 0.3}";

//   int httpResponseCode = http.POST(payload);
//   if (httpResponseCode == 200)
//   {
//     String response = http.getString();
//     http.end();
//     Serial.println(response);

//     // Parse JSON response
//     DynamicJsonDocument jsonDoc(1024);
//     deserializeJson(jsonDoc, response);
//     String outputText = jsonDoc["choices"][0]["message"]["content"];
//     return outputText;
//     // Serial.println(outputText);
//   }
//   else
//   {
//     http.end();
//     Serial.printf("Error %i \n", httpResponseCode);
//     return "<error>";
//   }
// }

String str;
String sum_str;
void getGPTAnswer(String inputText)
{
    sum_str = "";
    HTTPClient http;
    WiFiClientSecure client;
    client.setInsecure(); // 关闭 SSL 证书验证
    http.setTimeout(20000);
    http.begin(client,apiUrl);
    http.addHeader("Content-Type", "application/json");
    String token_key = String("Bearer ") + doubao_apiKey;
    http.addHeader("Authorization", token_key);
    String payload = "{\"model\":\"ep-20240914184638-mp4kq\",\"stream\":true,\"messages\":[{\"role\":\"user\",\"content\":\"" + inputText + "\"}],\"temperature\": 0.3}";

    int httpResponseCode = http.POST(payload);
    String response;
    if(httpResponseCode == 200)
    {
        WiFiClient *stream = http.getStreamPtr(); // 使用 WiFiClient*
        while(stream->connected() || stream->available())
        {
            if (stream->available())
            {
                String line = stream->readStringUntil('\n');
                if (line.length() > 0 && line.startsWith("data: "))
                {
                    // 提取 "data: " 后面的内容
                    String data = line.substring(6); // 跳过 "data: "
                    // Serial.println(data);
                    // 创建一个 JSON 文档对象
                    StaticJsonDocument<512> doc;
                    // 解析 JSON 数据
                    DeserializationError error = deserializeJson(doc, data.c_str());
                    // 检查是否解析成功
                    if(error)
                    {
                        Serial.print("deserializeJson() failed: ");
                        Serial.println(error.c_str());
                        break;
                    }
                    else
                    {
                        // 提取内容
                        str = doc["choices"][0]["delta"]["content"].as<String>();
                        // // 打印内容
                        // Serial.println("按段解析的数据->" + str);
                        if(str.length() > 0)
                        {
                            sum_str += str;
                        }
                    }
                    
                }
            }
        }
        Serial.println("数据接收完毕");
        http.end();
    }
    else
    {
        http.end();
        Serial.printf("Error %i \n", httpResponseCode);
        sum_str =  "<error>";
    }
}







