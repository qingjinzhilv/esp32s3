#include "MAX9814.h"

#define APIKey "Kd0q9dUv3Dgx8nwS0QluEqTS"
#define SecretKey "kZj6lwS0ceOV7vITLOSdHSRYjXVI8esh"
String token = "";

#define key 16 // 端口0
#define ADC 3  // 端口39
#define led 17 // 端口2

hw_timer_t *timer = NULL;

#define data_len 16000
uint16_t adc_data[data_len]; // 16000个数据，8K采样率，即2分钟，录音时间为2分钟，想要实现更长时间的语音识别，就要改这个数组大小
                             // 和下面data_json数组的大小，改大一些。

uint8_t adc_start_flag = 0;    // 开始标志
uint8_t adc_complete_flag = 0; // 完成标志

char data_json[45000]; // 用于储存json格式的数据,大一点,JSON编码后数据字节数变成原来的4/3,所以得计算好,避免出现越界

void MAX9814_init(void)
{
    pinMode(ADC, ANALOG);
    pinMode(key, INPUT_PULLUP);
    pinMode(led, OUTPUT);
    //获取token
    gain_token();
    // 定时器初始化 采集声音信息所用
    timer = timerBegin(0, 80, true);   //  80M的时钟 80分频 1M
    timerAlarmWrite(timer, 125, true); //  1M  计125个数进中断  8K
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmEnable(timer);
    timerStop(timer); // 先暂停
}

uint32_t time1, time2;
void MAX9814_speech_recognition(void)
{
    HTTPClient http_client;
    if (digitalRead(key) == 0) // 按键按下
    {
        Serial.printf("开始识别\r\n\r\n");
        digitalWrite(led, HIGH);
        adc_start_flag = 1;
        timerStart(timer);

        // time1=micros();
        while (!adc_complete_flag) // 等待采集完成
        {
            ets_delay_us(10);
        }
        // time2=micros()-time1;

        timerStop(timer);
        adc_complete_flag = 0; // 清标志

        digitalWrite(led, LOW);

        // Serial.printf("time:%d\r\n",time2);  //打印花费时间

        memset(data_json, '\0', strlen(data_json)); // 将数组清空
        strcat(data_json, "{");
        strcat(data_json, "\"format\":\"pcm\",");
        strcat(data_json, "\"rate\":8000,");       // 采样率    如果采样率改变了，记得修改该值，只有16000、8000两个固定采样率
        strcat(data_json, "\"dev_pid\":1537,");    // 中文普通话
        strcat(data_json, "\"channel\":1,");       // 单声道
        strcat(data_json, "\"cuid\":\"123456\","); // 识别码    随便打几个字符，但最好唯一
        strcat(data_json, "\"token\":\"");
        strcat(data_json, token.c_str());           // 使用token变量
        strcat(data_json, "\",");                   // 加入字符串结束引号
        strcat(data_json, "\"len\":32000,"); // 数据长度  如果传输的数据长度改变了，记得修改该值，该值是ADC采集的数据字节数，不是base64编码后的长度
        strcat(data_json, "\"speech\":\"");
        String base64_data = base64::encode((uint8_t *)adc_data, sizeof(adc_data)); // base64编码数据
        Serial.println(base64_data);
        Serial.println(base64_data.length());
        Serial.println(sizeof(adc_data));
        strcat(data_json, base64_data.c_str()); // base64编码数据
        strcat(data_json, "\"");
        strcat(data_json, "}");
        // Serial.println(data_json);

        int httpCode;
        http_client.begin("http://vop.baidu.com/server_api");
        http_client.addHeader("Content-Type", "application/json");
        httpCode = http_client.POST(data_json);

        if (httpCode > 0)
        {
            if (httpCode == HTTP_CODE_OK)
            {
                String payload = http_client.getString();
                Serial.println(payload);
                // 创建一个JSON文档
                StaticJsonDocument<256> doc; // 根据你的JSON大小调整这里的容量

                // 解析JSON字符串
                DeserializationError error = deserializeJson(doc, payload);
                // 从JSON中提取所需的字段
                const char *result = doc["result"][0]; // 获取result数组中的第一个元素

                // 输出结果
                Serial.println(result); // 输出: 介绍一下湖南社会经。
            }
        }
        else
        {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http_client.errorToString(httpCode).c_str());
        }
        http_client.end();

        while (!digitalRead(key))
            ;
        Serial.printf("Recognition complete\r\n");
    }
}

uint32_t num = 0;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
void IRAM_ATTR onTimer()
{
    // 递增计数器并设置 ISR 的时间
    portENTER_CRITICAL_ISR(&timerMux);
    if (adc_start_flag == 1)
    {
        // Serial.println("");
        adc_data[num] = analogRead(ADC);
        num++;
        if (num >= data_len)
        {
            adc_complete_flag = 1;
            adc_start_flag = 0;
            num = 0;
            // Serial.println(Complete_flag);
        }
    }
    portEXIT_CRITICAL_ISR(&timerMux);
}

void gain_token(void) // 获取token
{
    HTTPClient http_client;
    int httpCode;
    // wss://iat-api.xfyun.cn/v2/iat?authorization
    //  注意，要把下面网址中的your_apikey和your_secretkey替换成自己的API Key和Secret Key
    const char *url = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=" APIKey "&client_secret=" SecretKey;
    http_client.begin(url);
    httpCode = http_client.GET();
    Serial.printf("httpCode:%d\r\n", httpCode);
    if (httpCode == HTTP_CODE_OK)
    {
        String payload = http_client.getString();
        // Serial.println(payload);
        // 创建一个JSON文档
        StaticJsonDocument<512> doc; // 根据你的JSON大小调整这里的容量

        // 解析JSON字符串
        DeserializationError error = deserializeJson(doc, payload);
        // 从JSON中提取refresh_token
        const char *refresh_token = doc["refresh_token"]; // 获取refresh_token
        token = refresh_token;
        // 输出结果
        Serial.println("token->" + String(refresh_token));
    }
    else
    {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http_client.errorToString(httpCode).c_str());
    }
    http_client.end();
}
