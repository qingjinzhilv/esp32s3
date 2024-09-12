#include <Arduino.h>
#include <Crypto.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>
#include "String.h"
#include "OLED.h"

WiFiClient wifiClient;
PubSubClient espClient(wifiClient);

Ticker ticker;

int count = 0;

char pubArray[50],showArray[30];

//连接wifi以及阿里云
const char *ssid = "别摆烂了哦";
const char *password = "00000001";
const char *mqttServer = "a1fmG70qBSY.iot-as-mqtt.cn-shanghai.aliyuncs.com";
const int port = 1883;

const char *clientid = "a1fmG70qBSY.LED1|securemode=2,signmethod=hmacsha256,timestamp=1710487538699|";
const char *mqttUser = "LED1&a1fmG70qBSY";
const char *mqttPassword = "1d4e24195066decfab5cbf095eb15701ef5d2659cb3fd33abcc09c68c91cdace";

const char *aliyunpublish_Infortation = "/sys/a1fmG70qBSY/LED1/thing/event/property/post";
const char *aliyunSub_Infortation = "/sys/a1fmG70qBSY/LED1/thing/event/property/post_reply";

void WiFi_Connet(void);
void MQTT_Connect(void);
void Timer(void);
void LED_Pub(void);
void Brightness_Pub(void);
void Alarm_F(void);
void Learn_Timer(void);
void Rx_Pro(char *topic,byte *payload,unsigned int length);
void Data_Analysis(String Data);


void WiFi_Connet(void)
{
  WiFi.begin(ssid,password);

  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.print("\n");
  Serial.println("WiFi Connected!");
}

void MQTT_Connect(void)
{
  //espClient.setKeepAlive(60);
  //设置服务器和端口
  espClient.setServer(mqttServer,port);
  
  while(!espClient.connected())
  {
    Serial.println("MQTT Connect.....");
    if(espClient.connect(clientid,mqttUser,mqttPassword))
    {
      Serial.println("MQTT Server Connected.");
      Serial.print("MQTT Address:");
      Serial.println(mqttServer);
      Serial.print("ClientID:");
      Serial.println(clientid);
      espClient.subscribe(aliyunSub_Infortation);
      espClient.setCallback(Rx_Pro);
    }
    else
    {
      Serial.print("MQTT Server Connect failed.MQTT Statuas:");
      Serial.println(espClient.state());
      delay(2000);
    }
  }
}

void Rx_Pro(char *topic,byte *payload,unsigned int length)
{
  Serial.print("Message arrived[");
  Serial.print(topic);
  Serial.print("[");
  payload[length] = '\0';
  String message = String((char *)payload);
  Serial.println(message);
  Data_Analysis(message);
}

void Data_Analysis(String Data)
{
  uint8_t n[4] = {0};
  n[0] = Data.lastIndexOf("powerstate");
  n[1] = Data.lastIndexOf("brightness");
  n[2] = Data.lastIndexOf("AlarmFrequencyLevel");
  n[3] = Data.lastIndexOf("learningTime");
  Serial.println(Data);
  Serial.printf("%d\t%d\t%d\t%d\r\n",n[0],n[1],n[2],n[3]);
  char buf[500] = {0};
  Data.toCharArray(buf,500);
  char at[10];

  strncpy(at,buf + n[0] + 12,1);
  Serial.printf("powerstate:%d\r\n",atoi(at));
  sprintf(showArray,"powerstate:%d  ",atoi(at));
  OLED_ShowString(1,1,showArray);

  strncpy(at,buf + n[1] + 12,3);
  Serial.printf("brightness:%d\r\n",atoi(at));
  sprintf(showArray,"brightness:%d  ",atoi(at));
  OLED_ShowString(2,1,showArray);

  strncpy(at,buf + n[2] + 21,1);
  Serial.printf("AlarmFrequencyLevel:%d\r\n",atoi(at));
  sprintf(showArray,"Alarm:%d  ",atoi(at));
  OLED_ShowString(3,1,showArray);
  
  strncpy(at,buf + n[3] + 14,3);
  Serial.printf("learningTime:%d\r\n",atoi(at));
  sprintf(showArray,"learn_Time:%d  ",atoi(at));
  OLED_ShowString(4,1,showArray);
}


char LED_pubArray[] = "{\"params\":{\"powerstate\":%d}}";
_Bool LED_state = 0;
void LED_Pub(void)
{
  sprintf(pubArray,LED_pubArray,LED_state);
  Serial.println(pubArray);
  if(!espClient.publish(aliyunpublish_Infortation,pubArray))
  {
    Serial.println("LED publish Failed.");
  }
}

char Brightness_pubArray[] = "{\"params\":{\"brightness\":%d}}";
int32_t brightness = 99;
void Brightness_Pub(void)
{
  sprintf(pubArray,Brightness_pubArray,brightness);
  Serial.println(pubArray);
  if(!espClient.publish(aliyunpublish_Infortation,pubArray))
  {
    Serial.println("Brightness publish Failed.");
  }
}

char Alarm_F_pubArray[] = "{\"params\":{\"AlarmFrequencyLevel\":%d}}";
uint8_t Alarm = 2;
void Alarm_F(void)
{
  sprintf(pubArray,Alarm_F_pubArray,Alarm);
  Serial.println(pubArray);
  if(!espClient.publish(aliyunpublish_Infortation,pubArray))
  {
    Serial.println("Alarm_F publish Failed.");
  }
}

char Learning_pubArray[] = "{\"params\":{\"learningTime\":%d}}";
uint8_t Learn_Time = 32;
void Learn_Timer(void)
{
  sprintf(pubArray,Learning_pubArray,Learn_Time);
  Serial.println(pubArray);
  if(!espClient.publish(aliyunpublish_Infortation,pubArray))
  {
    Serial.println("Learn_Time publish Failed.");
  }
}

_Bool LED_flag = 0,brightness_flag = 0,Alarm_flag = 0,Learn_flag = 0;
void Timer(void)
{
  count ++;
  if(count % 2 == 0 && count < 3)
  {
    LED_flag = 1;
  }
  else if(count % 3 == 0)
  {
    brightness_flag = 1;
  }
  else if(count % 4 == 0)
  {
    Alarm_flag = 1;
  }
  else if(count % 5 == 0)
  {
    count = 0;
    Learn_flag = 1;
  }
}

void setup()
{
  Serial.begin(115200);

  ticker.attach(1,Timer);

  OLED_Init();
  OLED_Clear();

  pinMode(D4,OUTPUT);
  
  WiFi.mode(WIFI_STA);
  //WiFi连接
  WiFi_Connet();
  
  //连接MQTT
  MQTT_Connect();

}

void loop() 
{
  if(espClient.connected())
  {
    espClient.loop();
  }
}



