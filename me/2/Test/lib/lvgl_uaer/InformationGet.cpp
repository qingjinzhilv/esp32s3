#include "InformationGet.h"

extern const lv_img_dsc_t * ui_imgset_[40];
const char *reqLocation = "Hunan changsha";
const char *host = "api.seniverse.com";     // 将要连接的服务器地址
const int httpPort = 80;                    // 将要连接的服务器端口
// 心知天气HTTP请求所需信息
String reqUserKey = "SyFTVqlmyAMyZl81d";   // 私钥

String reqUnit = "c";                      // 摄氏/华氏

uint8_t low_weather = 0;

//函数声明
void SetClock(void *pv);
void Timer(TimerHandle_t xTimer);
void SetWeather(void *pv);


TimerHandle_t Timer_handle;
TaskHandle_t GetTime_Handle;
TaskHandle_t SetWeather_Handle;

uint16_t h_m_s[7] = {0};

const String WDAY_NAMES[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};                //星期

//获取时间任务创建
void GetTime_Create(void)
{
  Serial.println("GetTime_Create");
  xTaskCreate((TaskFunction_t)SetClock,(char *)"SetClock",(uint32_t)8 * 1024,NULL,(UBaseType_t)11,&GetTime_Handle);
}

//获取天气任务创建
void GetWeather_Create(void)
{
  Serial.println("GetWeather_Create");
  xTaskCreate((TaskFunction_t)SetWeather,(char *)"SetWeather",(uint32_t)8 * 1024,NULL,(UBaseType_t)9,&SetWeather_Handle);
}

//创建软件定时器
void Time_Init(void)
{
    Serial.println("TimerCreate");
    Timer_handle = xTimerCreate("Timer",1000,pdTRUE,(void *)1,Timer); 
    if(Timer_handle == NULL)
    {
      return;
    }   
    xTimerStart(Timer_handle,0);
}

//软件定时器回调函数
void Timer(TimerHandle_t xTimer)
{
  h_m_s[0] ++;
  if(h_m_s[0] >= 60)
  {
    h_m_s[0] = 0;
    h_m_s[1] ++;
    if(h_m_s[1] >= 60)
    {
      h_m_s[1] = 0;
      h_m_s[2] ++;
      if(h_m_s[2] >= 24)
      {
        h_m_s[2] = 0;
      }
      lv_label_set_text_fmt(ui_hour, "%.2d",h_m_s[2]);
    }
    lv_label_set_text_fmt(ui_minute, "%.2d",h_m_s[1]);
  }
  lv_label_set_text_fmt(ui_second, "%.2d",h_m_s[0]);
}

struct tm timeinfo;
void SetClock(void *pv)
{
  delay(1000);
  //如果获取失败，就开启联网模式，获取时间
  while(!getLocalTime(&timeinfo))
  {
    Serial.println("获取时间中");
    delay(250);
    // //创建连接WiFi的任务
    // WiFi_Creat();
  }
  // else 
  // {
    Serial.println("timeinfo Start");
    //秒
    h_m_s[0] = timeinfo.tm_sec;
    //分
    h_m_s[1] = timeinfo.tm_min;
    //时
    h_m_s[2] = timeinfo.tm_hour;
    //日
    h_m_s[3] = timeinfo.tm_mday;
    //月
    h_m_s[4] = timeinfo.tm_mon + 1;
    //年 
    h_m_s[5] = timeinfo.tm_year + 1900;
    //周
    h_m_s[6] = timeinfo.tm_wday;
    //时
    lv_label_set_text_fmt(ui_hour, "%.2d",h_m_s[2]);
    //分
    lv_label_set_text_fmt(ui_minute, "%.2d",h_m_s[1]);
    //秒
    lv_label_set_text_fmt(ui_second, "%.2d",h_m_s[0]);
    //年 月 日 ui_Label7
    lv_label_set_text_fmt(ui_YearMonDa, "%.4d/%.2d/%.2d",h_m_s[5],h_m_s[4],h_m_s[3]);
    //星期
    lv_label_set_text_fmt(ui_week, "%s",WDAY_NAMES[h_m_s[6]]);

    Serial.printf("%.4d-%.2d-%.2d %.2d:%.2d:%.2d %.1d\r\n",
                h_m_s[5],h_m_s[4],h_m_s[3],
                h_m_s[2],h_m_s[1],h_m_s[0],
                h_m_s[6]);

    Serial.println("timeinfo end");
  // }
  vTaskDelete(NULL);
}

void Buil_Request(void)
{
  // 建立心知天气API当前天气请求资源地址
  String reqRes = "/v3/weather/daily.json?key=SyFTVqlmyAMyZl81d&location=Hunan Changsha&language=en&unit=c&start=0&days=3";
  // 向心知天气服务器服务器请求信息并对信息进行解析
  httpRequest(reqRes);
  //建立获取实时生活指数
  String reqRes1 = "/v3/life/suggestion.json?key=" + reqUserKey +
                  + "&location=" + reqLocation + 
                  "&language=en";
  // 向心知天气服务器服务器请求信息并对信息进行解析                               
  httpRequest1(reqRes1);

  String reqRes2 = "/v3/life/suggestion.json?key=" + reqUserKey +
                  + "&location=" + reqLocation + 
                  "&language=en";
  // 向心知天气服务器服务器请求信息并对信息进行解析                               
  httpRequest1(reqRes2);
}

// 向心知天气服务器服务器请求信息并对信息进行解析
void httpRequest(String reqRes)
{
  WiFiClient client;
  // 建立http请求信息
  String httpRequest = String("GET ") + reqRes + " HTTP/1.1\r\n" + 
                              "Host: " + host + "\r\n" + 
                              "Connection: close\r\n\r\n";
  Serial.println(""); 
  Serial.print("Connecting to "); Serial.print(host);
  // while(!client.connect(host, 80))
  // {
  //   Serial.println("获取天气中");
  //   delay(250);
  // }
  // 尝试连接服务器
  if(client.connect(host, 80))
  {
    Serial.println(" Success!");
 
    // 向服务器发送http请求信息
    client.print(httpRequest);
    Serial.println("Sending request: ");
    Serial.println(httpRequest);  
 
    // 获取并显示服务器响应状态行 
    String status_response = client.readStringUntil('\n');
    Serial.print("status_response: ");
    Serial.println(status_response);
 
    // 使用find跳过HTTP响应头
    if (client.find("\r\n\r\n")) 
    {
      Serial.println("Found Header End. Start Parsing.");
    }
    
    // 利用ArduinoJson库解析心知天气响应信息
    parseInfo(client); 
  } 
  else 
  {
    Serial.println(" connection failed!");
  }   
  //断开客户端与服务器连接工作
  client.stop(); 
}

// 利用ArduinoJson库解析心知天气响应信息
void parseInfo(WiFiClient client) 
{
  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(3) + JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(6) + 3 * JSON_OBJECT_SIZE(14) + 860;

  DynamicJsonDocument doc(capacity);

  deserializeJson(doc, client);

  JsonObject results_0 = doc["results"][0];
  //返回指定days天数的结果
  JsonArray results_0_daily = results_0["daily"];

  JsonObject results_0_daily_0 = results_0_daily[0];
  //日期 今天
  const char* results_0_daily_0_date = results_0_daily_0["date"];
  //白天天气现象文字
  const char* results_0_daily_0_text_day = results_0_daily_0["text_day"];
  //白天天气现象代码
  const char* results_0_daily_0_code_day = results_0_daily_0["code_day"];
  //晚间天气现象文字
  const char* results_0_daily_0_text_night = results_0_daily_0["text_night"];
  //晚间天气现象代码
  const char* results_0_daily_0_code_night = results_0_daily_0["code_night"];
  //当天最高温度
  const char* results_0_daily_0_high = results_0_daily_0["high"];
  //当天最低温度
  const char* results_0_daily_0_low = results_0_daily_0["low"];
  //降水量，单位mm
  const char* results_0_daily_0_rainfall = results_0_daily_0["rainfall"];
  //降水概率，范围0~100，单位百分比（目前仅支持国外城市）
  const char* results_0_daily_0_precip = results_0_daily_0["precip"];
  //风向
  const char* results_0_daily_0_wind_direction = results_0_daily_0["wind_direction"];   
  //风向角度，范围0~360，0为正北，90为正东，180为正南，270为正西
  const char* results_0_daily_0_wind_direction_degree = results_0_daily_0["wind_direction_degree"]; 
  //风速，单位为km/h公里每小时或mph英里每小时
  const char* results_0_daily_0_wind_speed = results_0_daily_0["wind_speed"];
  //风力等级，请参考：http://baike.baidu.com/view/465076.htm
  const char* results_0_daily_0_wind_scale = results_0_daily_0["wind_scale"];
  //相对湿度，0~100，单位为百分比
  const char* results_0_daily_0_humidity = results_0_daily_0["humidity"];

  //明天
  JsonObject results_0_daily_1 = results_0_daily[1];
  const char* results_0_daily_1_date = results_0_daily_1["date"];
  const char* results_0_daily_1_text_day = results_0_daily_1["text_day"];
  const char* results_0_daily_1_code_day = results_0_daily_1["code_day"];
  const char* results_0_daily_1_text_night = results_0_daily_1["text_night"];
  const char* results_0_daily_1_code_night = results_0_daily_1["code_night"];
  const char* results_0_daily_1_high = results_0_daily_1["high"];
  const char* results_0_daily_1_low = results_0_daily_1["low"];
  const char* results_0_daily_1_rainfall = results_0_daily_1["rainfall"];
  const char* results_0_daily_1_precip = results_0_daily_1["precip"];
  const char* results_0_daily_1_wind_direction = results_0_daily_1["wind_direction"];
  const char* results_0_daily_1_wind_direction_degree = results_0_daily_1["wind_direction_degree"];
  const char* results_0_daily_1_wind_speed = results_0_daily_1["wind_speed"];
  const char* results_0_daily_1_wind_scale = results_0_daily_1["wind_scale"];
  const char* results_0_daily_1_humidity = results_0_daily_1["humidity"];

  //后天
  JsonObject results_0_daily_2 = results_0_daily[2];
  const char* results_0_daily_2_date = results_0_daily_2["date"];
  const char* results_0_daily_2_text_day = results_0_daily_2["text_day"];
  const char* results_0_daily_2_code_day = results_0_daily_2["code_day"];
  const char* results_0_daily_2_text_night = results_0_daily_2["text_night"];
  const char* results_0_daily_2_code_night = results_0_daily_2["code_night"];
  const char* results_0_daily_2_high = results_0_daily_2["high"];
  const char* results_0_daily_2_low = results_0_daily_2["low"];
  const char* results_0_daily_2_rainfall = results_0_daily_2["rainfall"];
  const char* results_0_daily_2_precip = results_0_daily_2["precip"];
  const char* results_0_daily_2_wind_direction = results_0_daily_2["wind_direction"];
  const char* results_0_daily_2_wind_direction_degree = results_0_daily_2["wind_direction_degree"];
  const char* results_0_daily_2_wind_speed = results_0_daily_2["wind_speed"];
  const char* results_0_daily_2_wind_scale = results_0_daily_2["wind_scale"];
  const char* results_0_daily_2_humidity = results_0_daily_2["humidity"];

  const char* results_0_last_update = results_0["last_update"];

  // 从以上信息中摘选几个通过串口监视器显示
  //日期
  String results_0_daily_0_date_str = results_0_daily_0["date"].as<String>();
  //白天天气现象文字
  String  results_0_daily_0_text_day_str = results_0_daily_0["text_day"].as<String>();
  //白天天气现象代码
  int results_0_daily_0_code_day_int = results_0_daily_0["code_day"].as<int>();
  //晚间天气现象文字
  String results_0_daily_0_text_night_str = results_0_daily_0["text_night"].as<String>();
  //晚间天气现象代码
  int results_0_daily_0_code_night_int = results_0_daily_0["code_night"].as<int>();

  //当天最高温度
  int results_0_daily_0_high_int = results_0_daily_0["high"].as<int>();
  //当天最低温度
  int results_0_daily_0_low_int = results_0_daily_0["low"].as<int>();

  //风向文字
  String results_0_daily_0_wind_direction_str = results_0_daily_0["wind_direction"].as<String>();
  //风向角度，范围0~360
  int results_0_daily_0_wind_direction_degree_int = results_0_daily_0["wind_direction_degree"].as<int>();
  //风速，单位km/h（当unit=c时）、mph（当unit=f时）
  float results_0_daily_0_wind_speed_float = results_0_daily_0["wind_speed"].as<float>();
  //风力等级
  int results_0_daily_0_wind_scale_int = results_0_daily_0["wind_scale"].as<int>();

  //降水量，单位mm
  float results_0_daily_0_rainfall_float = results_0_daily_0["rainfall"].as<float>();
  //相对湿度，0~100，单位为百分比
  int results_0_daily_0_humidity_int = results_0_daily_0["humidity"].as<int>();
  
  
  //数据更新时间（该城市的本地时间）
  String results_0_last_update_str = results_0["last_update"].as<String>();
  
  String day2Text = results_0_daily_2["text_day"].as<String>();
  Serial.println(F("======The Weahter of the day after tomorrow ======="));
  Serial.println(day2Text);
  
  Serial.println(F("======Today Weahter ======="));
  Serial.print(F("DATE: "));
  Serial.println(results_0_daily_0_date_str);

  //白天天气+气温
  Serial.print(F("Day Weather: "));
  Serial.print(results_0_daily_0_text_day_str);
  Serial.print(F(" "));
  Serial.println(results_0_daily_0_code_day_int);
  if(results_0_daily_0_code_day_int > 38)
  {
    results_0_daily_0_code_day_int = 39;
  }
  //设置气温
  lv_label_set_text_fmt(ui_Temp, "%d°",results_0_daily_0_code_day_int);
  //设置天气图片
  lv_img_set_src(ui_Image1, ui_imgset_[results_0_daily_0_code_day_int]);

  //晚上天气+气温
  Serial.print(F("Night Weather: "));
  Serial.print(results_0_daily_0_text_night_str);
  Serial.print(F(" "));
  Serial.println(results_0_daily_0_code_night_int);
  //最高气温
  Serial.print(F("High: "));
  Serial.println(results_0_daily_0_high_int);
  //最低气温
  Serial.print(F("LOW: "));
  Serial.println(results_0_daily_0_low_int);
  lv_label_set_text_fmt(ui_MaxMinWea, "Max%d° Min%d°",results_0_daily_0_high_int,results_0_daily_0_low_int);
  
  //风向文字
  Serial.print(F("Wind_direction: "));
  Serial.println(results_0_daily_0_wind_direction_str);
  //风向角度
  Serial.print(F("wind_direction_degree: "));
  Serial.println(results_0_daily_0_wind_direction_degree_int);
  //风速
  Serial.print(F("wind_speed: "));
  Serial.println(results_0_daily_0_wind_speed_float);
  //风力等级
  Serial.print(F("wind_scale: "));
  Serial.println(results_0_daily_0_wind_scale_int);
  //降水量
  Serial.print(F("rainfall: "));
  Serial.println(results_0_daily_0_rainfall_float);
  //相对湿度
  Serial.print(F("humidity: "));
  Serial.println(results_0_daily_0_humidity_int);

  //上一次天气更新时间
  Serial.print(F("Last Update: "));
  Serial.println(results_0_last_update_str);
  Serial.println(F("=============================="));
}

// 向心知天气服务器服务器请求信息并对信息进行解析
void httpRequest1(String reqRes)
{
  WiFiClient client;
 
  // 建立http请求信息
  String httpRequest = String("GET ") + reqRes + " HTTP/1.1\r\n" + 
                              "Host: " + host + "\r\n" + 
                              "Connection: close\r\n\r\n";
  Serial.println(""); 
  Serial.print("Connecting to "); Serial.print(host);
  // while(!client.connect(host, 80))
  // {
  //   Serial.println("获取生活信息中");
  //   delay(250);
  // }
  // 尝试连接服务器
  if (client.connect(host, 80))
  {
    Serial.println(" Success!");
 
    // 向服务器发送http请求信息
    client.print(httpRequest);
    Serial.println("Sending request: ");
    Serial.println(httpRequest);  
 
    // 获取并显示服务器响应状态行 
    String status_response = client.readStringUntil('\n');
    Serial.print("status_response: ");
    Serial.println(status_response);
 
    // 使用find跳过HTTP响应头
    if (client.find("\r\n\r\n")) 
    {
      Serial.println("Found Header End. Start Parsing.");
    }
    
    // 利用ArduinoJson库解析心知天气响应信息
    parseInfo1(client); 
  }
   
  //断开客户端与服务器连接工作
  client.stop(); 
}
 
// 利用ArduinoJson库解析心知天气响应信息  洗车 穿衣 感冒 运动 旅行 紫外强度 上一次更新时间
void parseInfo1(WiFiClient client)
{
  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 6*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 2*JSON_OBJECT_SIZE(6) + 400;
  DynamicJsonDocument doc(768);
  
  deserializeJson(doc, client);
  
  JsonObject results_0 = doc["results"][0];
  
  JsonObject results_0_suggestion = results_0["suggestion"];
  
  const char* results_0_suggestion_car_washing_brief = results_0_suggestion["car_washing"]["brief"];
  const char* results_0_suggestion_car_washing_details = results_0_suggestion["car_washing"]["details"];
  
  const char* results_0_suggestion_dressing_brief = results_0_suggestion["dressing"]["brief"];
  const char* results_0_suggestion_dressing_details = results_0_suggestion["dressing"]["details"]; 
  
  const char* results_0_suggestion_flu_brief = results_0_suggestion["flu"]["brief"]; 
  const char* results_0_suggestion_flu_details = results_0_suggestion["flu"]["details"]; 
  
  const char* results_0_suggestion_sport_brief = results_0_suggestion["sport"]["brief"]; 
  const char* results_0_suggestion_sport_details = results_0_suggestion["sport"]["details"];
  
  const char* results_0_suggestion_travel_brief = results_0_suggestion["travel"]["brief"];
  const char* results_0_suggestion_travel_details = results_0_suggestion["travel"]["details"]; 
  
  const char* results_0_suggestion_uv_brief = results_0_suggestion["uv"]["brief"]; 
  const char* results_0_suggestion_uv_details = results_0_suggestion["uv"]["details"]; 
  
  const char* results_0_last_update = results_0["last_update"];
 
  // 通过串口监视器显示以上信息
  String results_0_suggestion_car_washing_brief_str = results_0_suggestion["car_washing"]["brief"];
    
  String results_0_suggestion_dressing_brief_str = results_0_suggestion["dressing"]["brief"];
   
  String results_0_suggestion_flu_brief_str = results_0_suggestion["flu"]["brief"]; 
  
  String results_0_suggestion_sport_brief_str = results_0_suggestion["sport"]["brief"]; 
  
  String results_0_suggestion_travel_brief_str = results_0_suggestion["travel"]["brief"]; 
  
  String results_0_suggestion_uv_brief_str = results_0_suggestion["uv"]["brief"]; 
   
  const char* results_0_last_update_str = results_0["last_update"]; 
  
  Serial.println(F("======Life Info======="));
  //是否适合洗车
  Serial.print(F("Car Washing: "));
  Serial.println(results_0_suggestion_car_washing_brief_str);
  //穿衣
  Serial.print(F("Dressing: "));
  Serial.println(results_0_suggestion_dressing_brief_str);
  //感冒
  Serial.print(F("Flu: "));
  Serial.println(results_0_suggestion_flu_brief_str);
  //运动
  Serial.print(F("Sport: "));
  Serial.println(results_0_suggestion_sport_brief_str);
  //旅行
  Serial.print(F("Travel: "));
  Serial.println(results_0_suggestion_travel_brief_str);
  //紫外线强度  
  Serial.print(F("UV: "));
  Serial.println(results_0_suggestion_uv_brief_str);
  //上一次更新时间    
  Serial.print(F("Last Update: "));
  Serial.println(results_0_last_update_str);
  Serial.println(F("======================"));
}


//获取天气温度 生活参数任务
void SetWeather(void *pv)
{
  Buil_Request();
  vTaskDelete(NULL);
}
