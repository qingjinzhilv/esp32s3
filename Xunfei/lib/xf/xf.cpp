#include "xf.h"

// 接口认证信息中的APIPassword
String APIPassword = "lYdcVwsjEJlXvWZRjypM:zIRMKNQkOIuuZtPSPvDj";

// 请求地址
String APIurl = "https://spark-api-open.xf-yun.com/v1/chat/completions";

// String sendRequest(String inputText)
// {
//   HTTPClient http;
//   WiFiClientSecure client;
//   http.setTimeout(10000);

//   http.begin(client,APIurl);

//   //添加请求头
//   http.addHeader("Authorization", "Bearer " + APIPassword);
//   http.addHeader("Content-Type", "application/json");

//   //构造请求体
//   String requestBody = "{\"model\":\"generalv3.5\",\"stream\":true,\"messages\":[{\"role\":\"user\",\"content\":\"" + inputText + "\"}]}";

//   Serial.println(requestBody);
//   //发送请求
//   int httpCode = http.POST(requestBody);
//   String response;
//   if(httpCode == 200)
//   {
//     String outStr = http.getString();
//     Serial.println(outStr);
//     http.end();
//     DynamicJsonDocument jsonDoc(1024);
//     deserializeJson(jsonDoc, response);
//     String outputText = jsonDoc["content"].as<String>();
//     Serial.println(response);
//     //Serial.println("Request sent successfully");
//   }
//   else
//   {
//     http.end();
//     Serial.print("Error on sending request, code: ");
//     Serial.println(httpCode);
//     response = "<Error>";
//   }

//   return response;
// }
String str;
String sendRequest(String inputText)
{
    HTTPClient http;
    WiFiClientSecure client;
    client.setInsecure(); // 关闭 SSL 证书验证
    http.setTimeout(10000);

    http.begin(client, APIurl);

    // 添加请求头
    http.addHeader("Authorization", "Bearer " + APIPassword);
    http.addHeader("Content-Type", "application/json");

    // 构造请求体
    String requestBody = "{\"model\":\"generalv3.5\",\"stream\":true,\"messages\":[{\"role\":\"user\",\"content\":\"" + inputText + "\"}]}";

    Serial.println(requestBody);

    // 发送请求
    int httpCode = http.POST(requestBody);
    String response;

    if(httpCode == 200)
    {
        WiFiClient *stream = http.getStreamPtr(); // 使用 WiFiClient*
        String fullResponse;                      // 用于存储完整的响应数据

        while(stream->connected())
        {
            if (stream->available())
            {
                String line = stream->readStringUntil('\n');
                if (line.length() > 0 && line.startsWith("data: "))
                {
                    // 提取 "data: " 后面的内容
                    String data = line.substring(6); // 跳过 "data: "
                    fullResponse += data;     // 累加到完整响应中
                    // 创建一个 JSON 文档对象
                    StaticJsonDocument<512> doc;
                    // 解析 JSON 数据
                    DeserializationError error = deserializeJson(doc, data.c_str());
                    // 检查是否解析成功
                    if (error)
                    {
                        Serial.print("deserializeJson() failed: ");
                        Serial.println(error.c_str());
                    }
                    // 提取内容
                    str = doc["choices"][0]["delta"]["content"].as<String>();
                    // // 打印内容
                    // Serial.println("按段解析的数据->" + str);
                    if(str.length() > 0)
                    {
                        response += str;
                    }
                }
            }
        }
        http.end();
    }
    else
    {
        http.end();
        Serial.print("Error on sending request, code: ");
        Serial.println(httpCode);
        response = "<Error>";
    }

    return response;
}
