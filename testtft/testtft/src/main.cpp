// 内部库文件
#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <Ticker.h>
// 外部库文件
#include <TFT_eSPI.h>
#include <ArduinoJson.h>
// 本地库文件
#include "main.h"
#include "img.h"
#include "Unix.h"
#include "http.h"

char buffer[16];

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  xTaskCreate(
      GETWIFI,
      "GETWIFI",
      8 * 1024,
      NULL,
      3,
      &TASK_HandleOne);
  tft.init();
  tft.setRotation(1);
  // tft.fillScreen(TFT_BLACK);
  // tft.pushImage(16, 0, 128, 128, HIEU);
  // delay(1000);
  tft.fillScreen(TFT_BLACK);
  t1.attach_ms(1000, timer);
  t2.once(ST, lightcon);
  t3.attach_ms(2000, GetS);
  SetTime(Http_Get("http://192.168.31.189:1234/GetTime/TimeJson"));
  Pin_Init();
}
void loop()
{
  aht.GetData();
  delay(200);
  tft.setCursor(0, 0, 2);
  tft.print("TEMP=");
  tft.print(aht.temperature);
  tft.print("C   ");
  tft.setCursor(0, 16, 2);
  tft.print("HUMI=");
  tft.print((int)(aht.humidity * 100));
  tft.print("%   ");
  tft.setCursor(0, 32, 2);
  if (aht.temperature > temp)
  {
    tft.print("temp>");
    tft.print(temp);
    tft.print("!!!!");
    digitalWrite(27, HIGH);
  }
  else
  {
    tft.print("           ");
    digitalWrite(27, LOW);
  }
  PrintData();
  tft.setCursor(0, 96, 1);
  tft.print("KEY is ");
  digitalRead(KEY) ? tft.print("HIGH ") : tft.print("LOW ");
  tft.print("RAD is ");
  digitalRead(RAD) ? tft.print("HIGH ") : tft.print("LOW  ");
  tft.setCursor(0, 104, 1);
  tft.print("Light is ");
  tft.print(4095 - analogRead(36));
  tft.print("   ");
  tft.setCursor(0, 112, 1);
  tft.print(AutoMode == 0 ? "Auto" : "Manual");
  tft.print("           ");
  if (!digitalRead(KEY))
  {
    digitalWrite(LED, !digitalRead(LED));
    String data;
    digitalRead(LED) ? data = "http://192.168.31.189:1234/brake?mode=true" : data = "http://192.168.31.189:1234/brake?mode=false";
    Http_Get(data.c_str());
    while (!digitalRead(KEY))
      ;
    // AutoMode = ~AutoMode;
    // Serial.print(AutoMode);
  }
  if (digitalRead(RAD))
    t2.once(ST, lightcon);
  if (WiFi.status() == WL_CONNECTED)
  {
  }
  // delay(1000);
}
void GETWIFI(void *param)
{
  WiFi.begin(ssid, password);
  for (uint8_t t = 4; t > 0; t--)
  {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  vTaskDelete(TASK_HandleOne);
}
void Pin_Init()
{
  pinMode(LED, OUTPUT);
  pinMode(KEY, INPUT);
  pinMode(BUZ, OUTPUT);
  pinMode(RAD, INPUT);
}
void timer()
{
  loc_time[2]++;
  if (loc_time[2] >= 60)
  {
    loc_time[2] = 0;
    loc_time[1]++;
  }
  if (loc_time[1] >= 60)
  {
    loc_time[1] = 0;
    loc_time[0]++;
  }
  if (loc_time[0] > 24)
    loc_time[0] = 0;
}
void SetTime(String data)
{
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, data);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
  loc_time[0] = doc["hr"];
  loc_time[1] = doc["min"];
  loc_time[2] = doc["sec"];
}
void GetS(void)
{
  String data = "http://192.168.31.189:1234/Updata/At?id=1001&light=" + String(4095 - analogRead(36)) + "&temp=" + String(aht.temperature);
  data = Http_Get(data.c_str());
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, data.c_str());
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
  int st = doc["st"];
  if (st != ST)
    t2.once(st, lightcon);
  ST = st;
  temp = doc["tempthreshold"];
  if (Http_Get("http://192.168.31.189:1234/Read?id=1001").equals("true"))
  {
    if (!digitalRead(LED))
    {
      AutoMode = ~AutoMode;
      Serial.print(AutoMode);
    }
    digitalWrite(LED, HIGH);
  }
  else
  {
    if (digitalRead(LED))
    {
      AutoMode = ~AutoMode;
      Serial.print(AutoMode);
    }
    digitalWrite(LED, LOW);
  }
  if (aht.temperature > temp)
    Http_Get("http://192.168.31.189:1234/Set/Fire?id=1001&fire=true");
  else
    Http_Get("http://192.168.31.189:1234/Set/Fire?id=1001&fire=false");
}
void PrintData(void)
{
  tft.setCursor(0, 48, 2);
  loc_time[0] < 10 ? tft.printf("0%d", loc_time[0]) : tft.printf("%d", loc_time[0]);
  tft.print(":");
  loc_time[1] < 10 ? tft.printf("0%d", loc_time[1]) : tft.printf("%d", loc_time[1]);
  tft.print(":");
  loc_time[2] < 10 ? tft.printf("0%d", loc_time[2]) : tft.printf("%d", loc_time[2]);
  tft.print("         ");

  tft.setCursor(0, 64, 2);
  tft.print("ST = ");
  tft.print(ST);
  tft.print(" TEMP = ");
  // tft.setTextColor(TFT_GREEN);
  tft.print(temp);
  // tft.setTextColor(TFT_WHITE);
  tft.print("   ");
}
void lightcon(void)
{
  if (digitalRead(LED) && AutoMode == 0)
  {
    if ((4095 - analogRead(36)) > 2000 && (loc_time[0] > 9 && loc_time[0] < 18))
    {
      Serial.print("Auto(Day)!");
      Http_Get("http://192.168.31.189:1234/Set/Light_mode?id=1001&light_open=true");
    }
    else if ((loc_time[0] <=9 || loc_time[0] >= 19))
    {
      digitalWrite(LED, LOW);
      Serial.print("Auto(Night)!");
      Http_Get("http://192.168.31.189:1234/brake?mode=false");
    }
  }
  else
  {
    Http_Get("http://192.168.31.189:1234/Set/Light_mode?id=1001&light_open=false");
  }
}