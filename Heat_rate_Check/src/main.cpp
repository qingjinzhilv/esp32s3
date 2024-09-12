#include "main.h"

/* show_flag值对应界面值
 * 主界面 ············· 0
 * 菜单界面 ··········· 1
 * WLAN界面 ··········· 2
 * LED_Control界面 ···· 3
 * Timer_Set界面 ······ 4
 * WiFi_Add界面 ······· 5
 * heart rate界面 ..... 6

*/

// 0x0000 黑色
// 0xFFFF 白色
// 0xBDF7 浅灰色
// 0x7BEF 深灰色
// 0xF800 红色
// 0xFFE0 黄色
// 0xFBE0 橙色
// 0x79E0 棕色
// 0x7E0 绿色
// 0x7FF 青色
// 0x1F 蓝色
// 0xF81F 粉色

//WiFi名字和密码
const char* ssid     = "qingjin"; 
const char* password = "00000001";
WiFiMulti wifiMulti;
//定时器相关参数
Ticker t1,t2;
//时间请求相关参数
HTTPClient http;
WiFiClient httpclient;
//MQTT相关参数  //heartRate:12    location:hunan:
char haertRate_pubArray[] = "{\"params\":{\"heartRate\":%d}}";
char WiFi_pubArray[] = "{\"params\":{\"WiFi\":%d}}";
char weather_temp_pubArray[] = "{\"params\":{\"weather_temp\":%d}}";
char temp_pubArray[] = "{\"params\":{\"temp\":%.2lf}}";
char LED_pubArray[] = "{\"params\":{\"led\":%d}}";
char sop2_pubArray[] = "{\"params\":{\"sop2\":%d}}";
char hum_pubArray[] = "{\"params\":{\"hum\":%.2lf}}";
char lx_pubArray[] = "{\"params\":{\"lx\":%.2lf}}";
uint8_t Pub_flag = 0;
//屏幕控制参数
TFT_eSPI tft = TFT_eSPI();
//时间
uint8_t hh = 12,mm = 59,ss = 55;
//年月日
uint16_t Year = 0,Month = 0,Data = 0;
//WiFi状态
_Bool wifi_state = 0;
//天气
String weather = "Unknow";
int temp = 0;
//界面相关参数
uint8_t menu = 0,show_flag = 0;
uint8_t line = 1;
//菜单中的数量
#define menu_n 5
typedef struct main
{
  uint8_t next;
  String data;
}main;
main oled_menu[10] =
                   {
                    1,{"MENU"},
                    2,{"WLAN"},
                    3,{"LED_Control"},
                    4,{"Timer_Set"},
                    5,{"Heart rate"},
                    1,{"....."},
                   };

//WiFi页面相关参数
uint16_t WiFi_n = 0,wifi_line = 0;
typedef struct 
    {
      // 建立一系列变量，用于存储网络信息
      String ssid;            // 网络名称
      uint8_t encryptionType; // 网络加密类型
      int32_t RSSI;           // 网络信号强度
      uint8_t* BSSID;         // 网络接入点设备mac地址
      int32_t channel;        // 网络信道
      bool isHidden;          // 网络是否隐藏
    }wifi;
wifi wifi_Inforation[100];
main WiFi_Data[100];
//添加WiFi界面的相关参数
uint8_t data_change = 0;
int8_t keyboard_location = 0;
char password_1[100];
uint8_t buff = 0;
//按键控制相关参数
uint8_t key = 0,key_value = 0;
//LED页面相关变量
_Bool LED_flag = 0;
//时间设置界面相关变量
uint8_t settime[3] = {0};
uint8_t h_m_s = 1;    //标志位
//AHT10相关变量
AHT10 aht(20, 21, 0x38);
uint8_t AHT10_Update = 0;
//光照模块相关参数
double lx = 0.00;
//心率血氧相关参数
String hint = "Please press key2";
int32_t heartRate_value = 0,spo2_value = 0;
String heart_rate_efficacious = "Fail";
String sop2_efficacious = "Fail";
//蜂鸣器控制相关变量
uint8_t buzzer_flag = 0;
_Bool buzzer_time = 0;
//主界面函数
void interface(void)
{
  //时间 日期 显示
  tft.setTextColor(0xFBE0,0x0000);
  tft.setCursor(12,10,4);
  tft.printf("%.2d-%.2d-%.2d",hh,mm,ss);

  tft.setTextColor(TFT_WHITE,0x0000);
  tft.setCursor(25,35,2);
  tft.printf("%.4d/%.2d/%.2d",Year,Month,Data);
  //WiFi状态
  tft.setTextColor(0x7FF,0x0000);
  tft.setCursor(18,55,2);
  tft.printf("WiFi:%s",wifi_state ? "Connected":"Disconnet");
  //天气
  tft.setTextColor(TFT_GOLD,0x0000);
  tft.setCursor(40,75,2);
  tft.printf("%s",weather);
  tft.setTextColor(TFT_WHITE,0x0000);
  //气温
  tft.setCursor(36,95,2);
  tft.printf("Temp:%d°C",temp);
  //地点
  tft.setCursor(13,115,2);
  tft.printf("%s",reqLocation);
  //右下角的框
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.drawRoundRect(87,140,40,20,5,TFT_YELLOW);
  tft.setTextFont(2);
  tft.drawString("MENU",90,142);
}
//定时器函数 1s
void Timer_1(void)
{
  ss ++;
  if(ss >= 60)
  {
    ss = 0;
    mm ++;
    if(mm >= 60)
    {
      mm = 0;
      hh ++;
      if(hh >= 24)
      {
        hh = 0;
      }
    }
  }
  AHT10_Update ++;
  if(buzzer_flag) 
  {
    buzzer_time = !buzzer_time;
    buzzer_flag = 0;
  }
  else buzzer_time = 0;
  Pub_flag = 1;
}
//WiFi连接函数
void WiFiConnect(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if(WiFi.status() != WL_CONNECTED)
  {
    wifi_state = 0;
    delay(500);
    Serial.print(".");
  }
  else
  {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    wifi_state = 1;
  }
  
}
//时间获取函数
int httpCode = 0;
void GetTime(void)
{
  if(httpCode != 200)
  {
    // 连接苏宁网站的授时网页
    String GetUrl = "http://quan.suning.com/getSysTime.do";
    http.setTimeout(5000);
    http.begin(httpclient,GetUrl);
    delay(100);
    // 从网站获得网页内容
    httpCode = http.GET();
    
    if (httpCode > 0) 
    {
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK)
        {
          //读取响应内容
          String response = http.getString();
          Serial.println(response);
          char buf[100],n = 0,at[3];
          n = response.lastIndexOf("sysTime1\":\"");
          response.toCharArray(buf,100);
          strncpy(at,buf + n + 13,2);
          Year = 2000 + atoi(at);
          strncpy(at,buf + n + 15,2);
          Month = atoi(at);
          strncpy(at,buf + n + 17,2);
          Data = atoi(at);
          strncpy(at,buf + n + 19,2);
          hh = atoi(at);
          strncpy(at,buf + n + 21,2);
          mm = atoi(at);
          strncpy(at,buf + n + 23,2);
          ss = atoi(at);
        }
    } 
    else 
    {
        httpCode = 0;
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
  

}
//查找WiFi函数
void WiFi_Find(void)
{
  tft.fillScreen(TFT_BLACK);
  delay(100);
  tft.setTextColor(0x7FF,0x0000);
  tft.setCursor(18,55,2);
  tft.printf("WiFi Find....");
  // WiFi.mode(WIFI_STA);   //设置为无线终端模式
  if(WiFi.status() != WL_CONNECTED)
  {
    WiFi.disconnect();     //断开，不连接到任何一个接入点
  }
  delay(100);
  Serial.println("STA建立完成");
  Serial.println("同步扫描开始");
  int n = WiFi.scanNetworks();    //开始同步扫描，将返回值
  WiFi_n = n;
  Serial.println("同步扫描结束");   //存放在变量n中
  if (n == 0)
  {
    Serial.println("找不到网络");
  }
  else
  {
    Serial.println("发现网络");

    for (int i = 0; i < n; i++) //逐个打印扫描到的接入点信息
    {
       //获取扫描到的接入点网络信息(网络名称，信号强度等）
       WiFi.getNetworkInfo(i, wifi_Inforation[i].ssid, wifi_Inforation[i].encryptionType, wifi_Inforation[i].RSSI, wifi_Inforation[i].BSSID, wifi_Inforation[i].channel);
       //打印扫描到的接入点网络信息(网络名称，信号强度等）
      //  Serial.printf("%d: %s, Ch:%d, (%ddBm) %s\n", i + 1, wifi_Inforation[i].ssid.c_str(), wifi_Inforation[i].channel, wifi_Inforation[i].RSSI, wifi_Inforation[i].isHidden ? "隐藏" : "显示");
      WiFi_Data[i].next = i + 1;
      WiFi_Data[i].data = wifi_Inforation[i].ssid;
      delayMicroseconds(10);
    }
    WiFi_Data[n - 1].next = 0;
    
    // Serial.println("");
  }
  delay(100);
  tft.fillScreen(TFT_BLACK);
}
//主菜单函数
void Menu(void)
{
  if(show_flag == 1)
  {
    tft.setTextColor(TFT_WHITE,TFT_BLACK);
    tft.setCursor(30,10,4);
    tft.printf("%s",oled_menu[0].data);
    for(int i = 0;i < menu_n;i ++)
    {
      if(oled_menu[i].next == line)
      {
        // Serial.print(oled_menu[line - 1].next);
        tft.setTextColor(0xFBE0,TFT_BLACK);
        tft.setCursor(10,35 + (i*20),2);
        tft.printf("%d.%s",oled_menu[i].next,oled_menu[i + 1].data);
      }
      else
      {
        tft.setTextColor(TFT_WHITE,TFT_BLACK);
        tft.setCursor(10,35 + (i*20),2);
        tft.printf("%d.%s",oled_menu[i].next,oled_menu[i + 1].data);
      }
      
    }
    //右下角的框
    tft.setTextColor(TFT_WHITE,TFT_BLACK);
    tft.drawRoundRect(87,140,40,20,5,TFT_YELLOW);
    tft.setTextFont(2);
    tft.drawString("EXIT",94,142);
  }
  
}
//WiFi子函数
void WiFi_subinterface(void)
{
  if(WiFi_n > 0)
  {
    tft.setTextColor(TFT_WHITE,TFT_BLACK);
    tft.setCursor(35,10,4);
    tft.printf("%s",oled_menu[1].data);
    for(int i = 0;i < WiFi_n && i < 9;i ++)
    {
      tft.setCursor(0,35 + (i*15),1);
      tft.printf("%s","                                                  ");
    }
    uint16_t k = wifi_line;
    for(int i = 0;i < WiFi_n && i < 9;i ++)
    {
      if(WiFi_Data[wifi_line].next != 0)
      {
        if(wifi_line == k)
        {
          tft.setTextColor(0xFBE0,TFT_BLACK);
          tft.setCursor(10,35 + (i*15),1);
          tft.printf("%d.%s",WiFi_Data[wifi_line].next,WiFi_Data[wifi_line].data.c_str());
        }
        else
        {
          tft.setTextColor(TFT_WHITE,TFT_BLACK);
          tft.setCursor(10,35 + (i*15),1);
          tft.printf("%d.%s",WiFi_Data[wifi_line].next,WiFi_Data[wifi_line].data.c_str());
        }
      }
      wifi_line = WiFi_Data[wifi_line].next;
    }
    wifi_line = k;
  }
  else
  {
    tft.setTextColor(0x7FF,0x0000);
    tft.setCursor(0,75,2);
    tft.printf("WiFi is empty!");
  }
  //右下角的框
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.drawRoundRect(87,140,40,20,5,TFT_YELLOW);
  tft.setTextFont(2);
  tft.drawString("EXIT",94,142);
}
//LED控制函数
void LED_subinterface(void)
{
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.setCursor(20,10,2);
  tft.printf("%s",oled_menu[2].data);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  digitalWrite(9,LED_flag);
  if(LED_flag)
  {
    tft.setCursor(10,35,2);
    tft.print("state:");
    tft.setTextColor(TFT_GREEN,TFT_BLACK);
    tft.setCursor(80,35,2);
    tft.print("ON  ");
    tft.setTextColor(TFT_WHITE,TFT_BLACK);
  }
  else
  {
    tft.setCursor(10,35,2);
    tft.setTextColor(TFT_WHITE,TFT_BLACK);
    tft.print("state:");
    tft.setTextColor(TFT_GREEN,TFT_BLACK);
    tft.setCursor(80,35,2);
    tft.print("OFF ");
    tft.setTextColor(TFT_WHITE,TFT_BLACK);
  }
  
  //右下角的框
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.drawRoundRect(87,140,40,20,5,TFT_YELLOW);
  tft.setTextFont(2);
  tft.drawString("EXIT",94,142);
}
//时间设置子函数
void SetTime_subinterface(void)
{
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.setCursor(35,15,2);
  tft.printf("%s",oled_menu[3].data);

  for(int i = 1;i < 4;i ++)
  {
    if(h_m_s == i)
    {
      tft.setTextColor(0xFBE0,TFT_BLACK);
      tft.setCursor(35 + (i-1)*22,50,2);
      tft.printf("%.2d",settime[i - 1]);
      tft.setTextColor(TFT_WHITE,TFT_BLACK);
    }
    else
    {
      tft.setTextColor(TFT_WHITE,TFT_BLACK);
      tft.setCursor(35 + (i-1)*22,50,2);
      tft.printf("%.2d",settime[i - 1]);
    }
    tft.setTextColor(TFT_WHITE,TFT_BLACK);
    tft.setCursor(35 + 15,50,2);
    tft.printf("%c",'-');
    tft.setTextColor(TFT_WHITE,TFT_BLACK);
    tft.setCursor(57 + 15,50,2);
    tft.printf("%c",'-');
  }
  //右下角的框
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.drawRoundRect(87,140,40,20,5,TFT_YELLOW);
  tft.setTextFont(2);
  tft.drawString("EXIT",94,142);
}
//添加WiFi
void WiFi_Add(void)
{
  tft.setTextColor(TFT_GREEN,TFT_BLACK);
  tft.setCursor(0,10,1);
  tft.printf("ssid:");
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.printf("%s",WiFi_Data[wifi_line].data);
  tft.setTextColor(TFT_GREEN,TFT_BLACK);
  tft.setCursor(0,20,1);
  tft.printf("password:");
  tft.setTextColor(TFT_BLACK,TFT_WHITE);
  for(uint8_t i = 0;i < buff;i ++)
  {
    tft.printf("%c",password_1[i]);
  }
  uint16_t board = 0;
  board = key_board_ACSIL[key_board_n - 1].next + data_change*8;
  for(int k = 0;k < 5; ++ k)
  {
    for(int i = 0;i < 8;++ i)
    {
      if(board == keyboard_location)
      {
        tft.fillRoundRect(0 + i*16,40 + k * 21,15,20,5,TFT_GREENYELLOW);
        tft.setTextColor(TFT_BLACK,TFT_WHITE);
      }
      else
      {
        tft.fillRoundRect(0 + i *16,40 + k * 21,15,20,5,TFT_WHITE);
        tft.setTextColor(TFT_BLACK,TFT_WHITE);
      }
      tft.setTextColor(TFT_BLACK,TFT_WHITE);
      if(board != 94)
      {
        tft.setCursor(5 + i*16,45 + k*21,1);
        tft.printf("%c",key_board_ACSIL[board].data);
      }
      else if(board == 94)
      {
        tft.setCursor(5 + i*16,45 + k*21,1);
        tft.printf("%c",key_board_ACSIL[94].data);
        return;
      }
      board = key_board_ACSIL[board].next;
    }
  }
}
//心率血氧界面
void Heat_subinterface(void)
{
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.setCursor(35,15,2);
  tft.printf("%s",oled_menu[4].data);

  tft.setTextColor(TFT_RED,TFT_BLACK);
  tft.setCursor(10,35,1);
  tft.printf("%s",hint.c_str());

  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.setCursor(5,55,2);
  tft.printf("ECG:%dmmHg ",heartRate_value);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.setCursor(5,75,2);
  tft.printf("ECG state:%s  ",heart_rate_efficacious.c_str());

  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.setCursor(5,95,2);
  tft.printf("SPO2:%d%% ",spo2_value);
   tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.setCursor(5,115,2);
  tft.printf("SPO2 tate:%s  ",sop2_efficacious.c_str());

  //右下角的框
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.drawRoundRect(87,140,40,20,5,TFT_YELLOW);
  tft.setTextFont(2);
  tft.drawString("EXIT",94,142);
}
//蜂鸣器控制函数
void Buzzer_Control(void)
{
  if(((spo2 >= 95) && (spo2 <= 100) || (spo2 == 0) || (spo2 == -999))
  && ((aht.temperature > 18) && (aht.temperature < 30))
  && ((aht.humidity* 100 > 40) && (aht.humidity* 100 < 80)))
  {
    buzzer_flag = 0;
  }
  else
  {
    buzzer_flag = 1;
  }
  if(buzzer_time)
  {
    digitalWrite(8,1);
  }
  else
  {
    digitalWrite(8,0);
  }
}
//向阿里云发送消息
void Inforation_Pub(void)
{
  if(Pub_flag)
  {
    Pub_flag = 0;
    //心率
    InFormation_int_Pub(haertRate_pubArray,heartRate);
    //WiFi状态
    InFormation_int_Pub(WiFi_pubArray,wifi_state);
    //天气温度
    InFormation_int_Pub(weather_temp_pubArray,temp);
    // //温度
    InFormation_double_Pub(temp_pubArray,aht.temperature);
    // //小灯状态
    InFormation_int_Pub(LED_pubArray,LED_flag);
    // //血氧sop2
    InFormation_int_Pub(sop2_pubArray,spo2);
    // //湿度
    InFormation_double_Pub(hum_pubArray,aht.humidity * 100);
    // //光照强度
    InFormation_double_Pub(lx_pubArray,lx);
  }
}
//逻辑处理以及按键获取函数
void Key_Control(void)
{
  key_GetNum(&key);
  if(key < 10)
  {
    if(key != key_value)
    {
      key_value = key;
      // Serial.print(key);
    }
    else
    {
      key = 0;
    }
  }
  
  if(!show_flag) 
  {
    interface();
    if(key == 1)
    {
      show_flag = 1;
      tft.fillScreen(TFT_BLACK);
    }
    wifi_line = 0;
    line = 1;
    h_m_s = 1;
    buff = 0;
    memset(password_1,0,100);
  }
  else if(show_flag == 1)
  {
    if(key == 2)
    {
      line ++;
      if(line > menu_n)
      {
        line = 1;
      }
    }
    else if(key == 1)
    {
      show_flag = 0;
      tft.fillScreen(TFT_BLACK);
    }
    else if(key == 5)
    {
      tft.fillScreen(TFT_BLACK);
      if(oled_menu[line - 1].next == 1)
      {
        wifi_line = 0;
        WiFi_Find();
        WiFi_subinterface();
        show_flag = 2;
      }
      else if(oled_menu[line - 1].next == 2)
      {
        show_flag = 3;
      }
      else if(oled_menu[line - 1].next == 3)
      {
        settime[0] = hh;
        settime[1] = mm;
        settime[2] = ss;
        show_flag = 4;
      }
      else if(oled_menu[line - 1].next == 4)
      {
        show_flag = 6;
      }
    }
    wifi_line = 0;
    h_m_s = 1;
    buff = 0;
    memset(password_1,0,100);
  }
  else if(show_flag == 2)
  {
    if(key == 1)
    {
      show_flag = 1;
      tft.fillScreen(TFT_BLACK);
    }
    else if(key == 2)
    {
      wifi_line = WiFi_Data[wifi_line].next;
      // Serial.println(WiFi_Data[wifi_line].data);
      WiFi_subinterface();
    }
    if(key == 5)
    {
      tft.fillScreen(TFT_BLACK);
      show_flag = 5;
      WiFi_Add();
    }
    buff = 0;
    memset(password_1,0,100);
  }
  else if(show_flag == 3)
  {
    LED_subinterface();
    if(key == 1)
    {
      show_flag = 1;
      tft.fillScreen(TFT_BLACK);
    }
    else if(key == 2)
    {
      LED_flag = !LED_flag;
    }
  }
  else if(show_flag == 4)
  {
    SetTime_subinterface();
    if(key == 1)
    {
      show_flag = 1;
      tft.fillScreen(TFT_BLACK);
    }
    else if(key == 2)
    {
      h_m_s ++;
      if(h_m_s > 3)
      {
        h_m_s = 1;
      }
    }
    else if(key == 3)
    {
      if(h_m_s == 1)
      {
        settime[h_m_s - 1] ++;
        if(settime[h_m_s - 1] >= 24)
        {
          settime[h_m_s - 1] = 0;
        }
      }
      else if(h_m_s == 2)
      {
        settime[h_m_s - 1] ++;
        if(settime[h_m_s - 1] >= 60)
        {
          settime[h_m_s - 1] = 0;
        }
      }
      else if(h_m_s == 3)
      {
        settime[h_m_s - 1] ++;
        if(settime[h_m_s - 1] >= 60)
        {
          settime[h_m_s - 1] = 0;
        }
      }
    }
    else if(key == 4)
    {
      if(h_m_s == 1)
      {
        settime[h_m_s - 1] --;
        if(settime[h_m_s - 1] == 255)
        {
          settime[h_m_s - 1] = 23;
        }
      }
      else if(h_m_s == 2)
      {
        settime[h_m_s - 1] --;
        if(settime[h_m_s - 1] == 255)
        {
          settime[h_m_s - 1] = 59;
        }
      }
      else if(h_m_s == 3)
      {
        settime[h_m_s - 1] --;
        if(settime[h_m_s - 1] == 255)
        {
          settime[h_m_s - 1] = 59;
        }
      }
    }
    else if(key == 5)
    {
      hh = settime[0];
      mm = settime[1];
      ss = settime[2];
      show_flag = 0;
      tft.fillScreen(TFT_BLACK);
    }
  }
  else if(show_flag == 5)
  {
    if(key == 1)
    {
      show_flag = 2;
      tft.fillScreen(TFT_BLACK);
      WiFi_subinterface();
    }
    else if(key == 2)
    {
      // Serial.printf("ssid:%s\r\n",WiFi_Data[wifi_line].data.c_str());
      // Serial.printf("ssid:%s\r\n",password_1);
      tft.fillScreen(TFT_BLACK);
      WiFi.disconnect();
      WiFi.mode(WIFI_STA);
      WiFi.begin(WiFi_Data[wifi_line].data.c_str(),password_1);
      uint8_t test = 0,connect_1 = 0;
      while((WiFi.status() != WL_CONNECTED) && !connect_1)
      {
        Serial.print(".");
        delay(200);
        tft.setTextColor(0x7FF,0x0000);
        tft.setCursor(10,55,2);
        tft.printf("WiFi Connect...%d",test ++);
        if(WiFi.status() == WL_CONNECTED)
        {
          connect_1 = 1;
          show_flag = 0;
          Serial.println("okokokokokokokokokokokkkom");
          tft.fillScreen(TFT_BLACK);
          return;
        }
        if(test > 50)
        {
          tft.setTextColor(0x7FF,0x0000);
          tft.setCursor(10,55,2);
          tft.printf("WiFi Fail...");
          tft.fillScreen(TFT_BLACK);
          show_flag = 5;
          buff = 0;
          memset(password_1,0,100);
          WiFi_Add();
          return;
        }
      }
      // Serial.printf("ssid:%s\r\n",WiFi_Data[wifi_line].data.c_str());
      // Serial.printf("ssid:%s\r\n",password_1);
      buff = 0;
      memset(password_1,0,100);
    }
    else if(key == 3)
    {
      keyboard_location ++;
      if(keyboard_location > 94)
      {
        keyboard_location = 94;
      }
      WiFi_Add();
    }
    else if(key == 4)
    {
      keyboard_location --;
      if(keyboard_location <= 0)
      {
        keyboard_location = 0;
      }
      WiFi_Add();
    }
    else if(key == 5)
    {
      password_1[buff ++] = key_board_ACSIL[keyboard_location].data;
      // Serial.print("password_1->");
      // Serial.println(password_1);
      WiFi_Add();
    }
    else if(key == 6)
    {
      password_1[buff] = '*';
      buff --;
    }
    else if(key == 7)
    {
      // Serial.print("keyboard_location->");
      // Serial.println(keyboard_location);
      // Serial.print("data_change->");
      // Serial.println(data_change);
      data_change ++;
      if(data_change > 11)
      {
        data_change = 11;
      }
      else
      {
        uint8_t y = keyboard_location;
        keyboard_location += 8;
        if(keyboard_location >= 94)
        {
          keyboard_location = y;
        }
      }
      // Serial.print("keyboard_location->");
      // Serial.println(keyboard_location);
      // Serial.print("data_change->");
      // Serial.println(data_change);
      WiFi_Add();
    }
    else if(key == 8)
    {
      data_change --;
      if(data_change == 255)
      {
        data_change = 0;
      }
      else
      {
        int8_t key_1 = keyboard_location;
        keyboard_location -= 8;
        if(keyboard_location < 0)
        {
          keyboard_location = 0;
        }
      }
      WiFi_Add();
    }
  }
  else if(show_flag == 6)
  {
    Heat_subinterface();
    if(key == 1)
    {
      show_flag = 1;
      tft.fillScreen(TFT_BLACK);
    }
    else if(key == 2)
    {
      hint = "      Check...       ";
      sop2_efficacious = heart_rate_efficacious = "Check..";
      Heat_subinterface();
      MAX30102_GetData();
      if(validHeartRate)
      {
        heart_rate_efficacious = "succeed";
        delay(100);
        heartRate_value = heartRate;
      }
      else
      {
        heart_rate_efficacious = "Fail   ";
        heartRate_value = 0;
      }
      if(validSPO2)
      {
        sop2_efficacious = "succeed";
        spo2_value = spo2;
      }
      else
      {
        sop2_efficacious = "Fail   ";
        spo2 = 0;
      }
      hint = "Please press key2";
      Heat_subinterface();
    }
  }
}

//初始化函数
void Init(void)
{
  Serial.begin(115200);

  pinMode(1,INPUT_PULLUP);
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  digitalWrite(9,LED_flag);

  MAX30102_Init();

  WiFiConnect();
  MQTT_Connect();
  GetTime();
  
  t1.attach(1,Timer_1);
  t2.attach_ms(100,Timer_2);
  
  tft.init();                // 初始屏幕
  tft.fillScreen(TFT_BLACK); // 清屏 把屏幕设置为黑色

  Buil_Request();
  GY302_Init();
  
}

void setup()
{
  Init();
}

void loop() 
{
  Key_Control();
  Menu();
  if(httpCode != HTTP_CODE_OK)
  {
    delay(100);
    GetTime();
    Buil_Request();
  }
  if(WiFi.status() != WL_CONNECTED)
  {
    wifi_state = 0;
  }
  else
  {
    wifi_state = 1;
  }
  if(AHT10_Update > 1)
  {
    AHT10_Update = 0;
    lx = Brightness();
    Serial.printf("lightness->%.2lf[lx]\r\n",lx);
    aht.GetData();
    Serial.print("humidity->");
    Serial.print(aht.humidity * 100);
    Serial.println("%");
    Serial.print("temperature->");
    Serial.printf("%.2f°C\r\n",aht.temperature);
  }
  Buzzer_Control();
  MQTT_Keep();
  Inforation_Pub();
}
