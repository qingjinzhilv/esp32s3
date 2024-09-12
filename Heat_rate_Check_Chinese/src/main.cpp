#include "main.h"
//汉字库
#include "Chinese.h"

/* show_flag值对应界面值
 * 主界面 ············· 0
 * 菜单界面 ··········· 1
 * WLAN界面 ··········· 2
 * LED_Control界面 ···· 3
 * Timer_Set界面 ······ 4
 * WiFi_Add界面 ······· 5

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
                    1,{"菜单"},
                    2,{"网络"},
                    3,{"小灯控制"},
                    4,{"时间设置"},
                    5,{"测试"},
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
_Bool LED_flag = 1;
//时间设置界面相关变量
uint8_t settime[3] = {0};
uint8_t h_m_s = 1;    //标志位
//主界面函数
void interface(void)
{
  // //时间 日期 显示
  tft.setTextColor(TFT_RED,TFT_BLACK);
  tft.fillRoundRect(0,6,128,28,1,TFT_BLACK);
  tft.setTextColor(0xFBE0,0x0000);
  tft.loadFont(Chinese_32);
  tft.setCursor(7,10);
  tft.printf("%.2d-%.2d-%.2d",hh,mm,ss);
  tft.setTextColor(0xFFFACD,0x0000);
  tft.loadFont(Chinese_20);
  tft.setCursor(8,35);
  tft.printf("%.2d年%.2d月%.2d日",Year,Month,Data);
  tft.unloadFont();
  //WiFi状态
  tft.loadFont(Chinese_20);
  tft.setTextColor(TFT_GOLD,0x0000);
  tft.setCursor(18,55);
  tft.printf("状态:%s",wifi_state ? "连接":"断开");
  tft.unloadFont();
  //天气
  tft.loadFont(Chinese_20);
  tft.setTextColor(0x7FF,0x0000);
  tft.setCursor(10,75,2);
  tft.printf("天气:%s",weather);
  tft.setTextColor(TFT_WHITE,0x0000);
  tft.unloadFont();
  //气温
  tft.loadFont(Chinese_20);
  tft.setTextColor(0xE6E6FA,0x0000);
  tft.setCursor(30,95,2);
  tft.printf("温度:%d度",temp);
  tft.unloadFont();
  //地点
  tft.loadFont(Chinese_20);
  tft.setTextColor(0xE6E6FA,0x0000);
  tft.setCursor(18,119);
  tft.printf("%s",(reqLocation == "Hunan changsha") ? "湖南 长沙":"未知");
  tft.unloadFont();
  //右下角的框
  tft.loadFont(Chinese_20);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.drawRoundRect(87,135,40,21,5,TFT_YELLOW);
  tft.setTextFont(2);
  tft.drawString("菜单",88,140);
  tft.unloadFont();
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
}
//WiFi连接函数
void WiFiConnect(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  else
  {
    Serial.println("");
    Serial.println("WiFi connected");
    // Serial.println("IP address: ");
    // Serial.println(WiFi.localIP());
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
          // Serial.println(response);
          char buf[200],n = 0,at[10];
          n = response.lastIndexOf("sysTime1\":\"");
          response.toCharArray(buf,200);
          strncpy(at,buf + n + 13,2);
          Year = atoi(at);
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
          // Serial.printf("%.4d-%.2d-%.2d %.2d-%.2d-%.2d\r\n",Year,Month,Data,hh,mm,ss);
        }
    } 
    else 
    {
        httpCode = 0;
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
    delay(500);
  }
  

}
//查找WiFi函数
void WiFi_Find(void)
{
  tft.fillRoundRect(0,0,128,160,1,TFT_BLACK);
  delay(100);
  tft.setTextColor(0x7FF,0x0000);
  tft.loadFont(Chinese_20);
  tft.setCursor(18,55,2);
  tft.printf("查找WiFi....");
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
    
    Serial.println("");
  }
  delay(100);
  tft.fillRoundRect(0,0,128,160,1,TFT_BLACK);
}
//主菜单函数
void Menu(void)
{
  if(show_flag == 1)
  {
    tft.setTextColor(TFT_WHITE,TFT_BLACK);
    tft.loadFont(Chinese_20);
    tft.setCursor(40,10);
    tft.printf("%s",oled_menu[0].data);
    for(int i = 0;i < menu_n;i ++)
    {
      if(oled_menu[i].next == line)
      {
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
    tft.drawRoundRect(87,135,40,21,5,TFT_YELLOW);
    tft.setTextFont(2);
    tft.drawString("退出",88,140);
    tft.unloadFont();
  }
  
}
//WiFi子函数
void WiFi_subinterface(void)
{
  if(WiFi_n > 0)
  {
    tft.setTextColor(TFT_WHITE,TFT_BLACK);
    tft.loadFont(Chinese_20);
    tft.setCursor(42,10,4);
    tft.printf("%s",oled_menu[1].data);
    tft.unloadFont();
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
    tft.printf("未找到!");
  }
  //右下角的框
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.drawRoundRect(87,135,40,21,5,TFT_YELLOW);
  tft.loadFont(Chinese_20);
  tft.drawString("退出",88,140);
  tft.unloadFont();
}
//LED控制函数
void LED_subinterface(void)
{
  tft.loadFont(Chinese_20);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.setCursor(20,10,2);
  tft.printf("%s",oled_menu[2].data);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  
  if(!LED_flag)
  {
    digitalWrite(9,!LED_flag);
    tft.setCursor(10,35,2);
    tft.print("状态:");
    tft.setTextColor(TFT_GREEN,TFT_BLACK);
    tft.loadFont(Chinese_20);
    tft.setCursor(80,35,2);
    tft.print("开启");
    tft.setTextColor(TFT_WHITE,TFT_BLACK);
  }
  else
  {
    digitalWrite(9,LED_flag);
    digitalWrite(9,!LED_flag);
    tft.setCursor(10,35,2);
    tft.setTextColor(TFT_WHITE,TFT_BLACK);
    tft.print("状态:");
    tft.setTextColor(TFT_GREEN,TFT_BLACK);
    tft.loadFont(Chinese_20);
    tft.setCursor(80,35,2);
    tft.print("关闭");
    tft.setTextColor(TFT_WHITE,TFT_BLACK);
  }
  
   //右下角的框
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.drawRoundRect(87,135,40,21,5,TFT_YELLOW);
  tft.setTextFont(2);
  tft.drawString("退出",88,140);
  tft.unloadFont();
}
//时间设置子函数
void SetTime_subinterface(void)
{
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.loadFont(Chinese_20);
  tft.setCursor(30,15);
  tft.printf("%s",oled_menu[3].data);
  tft.loadFont(Chinese_20);
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
    tft.unloadFont();
  }
   //右下角的框
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.loadFont(Chinese_20);
  tft.drawRoundRect(87,135,40,21,5,TFT_YELLOW);
  tft.setTextFont(2);
  tft.drawString("退出",88,140);
  tft.unloadFont();
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
      tft.fillRoundRect(0,0,128,160,1,TFT_BLACK);
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
      tft.fillRoundRect(0,0,128,160,1,TFT_BLACK);
    }
    else if(key == 5)
    {
      tft.fillRoundRect(0,0,128,160,1,TFT_BLACK);
      if(oled_menu[line - 1].next == 1)
      {
        wifi_line = 0;
        WiFi_Find();
        WiFi_subinterface();
        show_flag = 2;
      }
      if(oled_menu[line - 1].next == 2)
      {
        show_flag = 3;
      }
      if(oled_menu[line - 1].next == 3)
      {
        settime[0] = hh;
        settime[1] = mm;
        settime[2] = ss;
        show_flag = 4;
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
      tft.fillRoundRect(0,0,128,160,1,TFT_BLACK);
    }
    else if(key == 2)
    {
      wifi_line = WiFi_Data[wifi_line].next;
      Serial.println(WiFi_Data[wifi_line].data);
      WiFi_subinterface();
    }
    if(key == 5)
    {
      tft.fillRoundRect(0,0,128,160,1,TFT_BLACK);
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
      tft.fillRoundRect(0,0,128,160,1,TFT_BLACK);
    }
    else if(key == 2)
    {
      LED_flag = !LED_flag;
      tft.fillRoundRect(0,0,128,160,1,TFT_BLACK);
    }
    
  }
  else if(show_flag == 4)
  {
    SetTime_subinterface();
    if(key == 1)
    {
      show_flag = 1;
      tft.fillRoundRect(0,0,128,160,1,TFT_BLACK);
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
      tft.fillRoundRect(0,0,128,160,1,TFT_BLACK);
    }
  }
  else if(show_flag == 5)
  {
    if(key == 1)
    {
      show_flag = 2;
      tft.fillRoundRect(0,0,128,160,1,TFT_BLACK);
      WiFi_subinterface();
    }
    else if(key == 2)
    {
      Serial.printf("ssid:%s\r\n",WiFi_Data[wifi_line].data.c_str());
      Serial.printf("ssid:%s\r\n",password_1);
      tft.fillRoundRect(0,0,128,160,1,TFT_BLACK);
      WiFi.disconnect();
      WiFi.mode(WIFI_STA);
      WiFi.begin(WiFi_Data[wifi_line].data.c_str(),password_1);
      uint8_t test = 0,connect_1 = 0;
      while((WiFi.status() != WL_CONNECTED) && !connect_1)
      {
        Serial.print(".");
        delay(200);
        tft.setTextColor(0x7FF,0x0000);
        tft.loadFont(Chinese_20);
        tft.setCursor(10,55,2);
        tft.printf("连接中...");
        tft.unloadFont();
        if(WiFi.status() == WL_CONNECTED)
        {
          connect_1 = 1;
          show_flag = 0;
          Serial.println("连接成功！");
          tft.fillRoundRect(0,0,128,160,1,TFT_BLACK);
          return;
        }
        if(test > 50)
        {
          tft.setTextColor(0x7FF,0x0000);
          tft.loadFont(Chinese_20);
          tft.setCursor(10,55,2);
          tft.printf("连接失败！");
          show_flag = 2;
          tft.unloadFont();
          tft.fillRoundRect(0,0,128,160,1,TFT_BLACK);
          return;
        }
      }
      Serial.printf("ssid:%s\r\n",WiFi_Data[wifi_line].data.c_str());
      Serial.printf("ssid:%s\r\n",password_1);
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
}
//初始化函数
void Init(void)
{
  Serial.begin(115200);

  pinMode(1,INPUT_PULLUP);
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);
  pinMode(9,OUTPUT);

  WiFiConnect();
  GetTime();
  
  t1.attach(1,Timer_1);
  t2.attach_ms(100,Timer_2);
  
  tft.init();                // 初始屏幕
  tft.fillRoundRect(0,0,128,160,1,TFT_BLACK); // 清屏 把屏幕设置为黑色
  Buil_Request();
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
    tft.fillRoundRect(0,40,128,160,1,TFT_BLACK);
    delay(100);
    GetTime();
    Buil_Request();
  }
  if(WiFi.status() != WL_CONNECTED)
  {
    wifi_state = 0;
    tft.fillRoundRect(0,6,128,29,1,TFT_BLACK);
  }
  else
  {
    wifi_state = 1;
  }
}
