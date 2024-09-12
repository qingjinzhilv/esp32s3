#include <Arduino.h>
#include <Crypto.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>

WiFiClient wifiClient;
PubSubClient espClient(wifiClient);

Ticker ticker;

int count = 0;
//连接wifi以及阿里云
const char *ssid = "别摆烂了哦";
const char *password = "00000001";
const char *mqttServer = "iot-06z00ghcnp54t9j.mqtt.iothub.aliyuncs.com";
const int port = 1883;

const char *clientid = "k0wjskKFIQw.ESP8266|securemode=2,signmethod=hmacsha256,timestamp=1710307088826|";
const char *mqttUser = "ESP8266&k0wjskKFIQw";
const char *mqttPassword = "c473f624fc38a9af115a5fb43fa3717f2547fa27b9c76812b5530706fbc05f73";

const char *aliyunpublish_user_update = "/k0wjskKFIQw/ESP8266/user/qingjin";
const char *aliyunpublish_Infortation = "/sys/k0wjskKFIQw/ESP8266/thing/event/property/post";

void callback(char *topic,byte *payload,unsigned int length);
void WiFi_Connet(void);
void SubscribeTopic(void);
void MQTT_Connect(void);
void pubMQTTmsg(void);
void Timer(void);
void Data_Analysis(String Data);
void pubInfortaion(void);


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

char subTopic[] = "/k0wjskKFIQw/ESP8266/user/1234";
void SubscribeTopic(void)
{
  if(espClient.subscribe(subTopic))
  {
    Serial.print("Subscribe Topic:");
    Serial.println(subTopic);
    espClient.setCallback(callback);
  }
  else
  {
    Serial.println("Subscribe Failed!!!");
  }
}

void callback(char *topic,byte *payload,unsigned int length)
{
  Serial.print("Message arrived[");
  Serial.print(topic);
  Serial.print("]");
  payload[length] = '\0';
  String message = String((char *)payload);
  Serial.println(message);
  Data_Analysis(message);
}

void Data_Analysis(String Data)
{
  uint8_t n = Data.lastIndexOf("LED");
  Serial.print("loction: ");
  Serial.println(n);
  char buf[1000] = {0};
  Data.toCharArray(buf,1000);
  Serial.print("Data: ");
  Serial.println(buf[n + 5]);
  if(buf[n + 5] == '1')
  {
    digitalWrite(D4,0);
  }
  else
  {
    digitalWrite(D4,1);
  }
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
      SubscribeTopic();
    }
    else
    {
      Serial.print("MQTT Server Connect failed.MQTT Statuas:");
      Serial.println(espClient.state());
      delay(2000);
    }
  }
}

char Msg_publish[] = "MQTT,I am esp8266!";
void pubMQTTmsg(void)
{
  if(!espClient.publish(aliyunpublish_user_update,Msg_publish))
  {
    Serial.println("Msg publish Failed.");
  }
}

char Temp_pubArray[] = "{\"params\":{\"temperature\":%.2lf}}";
float Temp = 0.00;
char LED_pubArray[] = "{\"params\":{\"LED\":%d}}";
_Bool LED_state = 1;
char pubArray[50];
void pubInfortaion(void)
{
  Temp = 45.33;
  sprintf(pubArray,Temp_pubArray,Temp);
  Serial.println(pubArray);
  if(!espClient.publish(aliyunpublish_Infortation,pubArray))
  {
    Serial.println("Msg publish Failed.");
  }
  sprintf(pubArray,LED_pubArray,LED_state);
  Serial.println(pubArray);
  if(!espClient.publish(aliyunpublish_Infortation,pubArray))
  {
    Serial.println("Msg publish Failed.");
  }
}

void Timer(void)
{
  count ++;
}

void setup()
{
  Serial.begin(115200);

  ticker.attach(1,Timer);

  pinMode(D4,OUTPUT);
  
  WiFi.mode(WIFI_STA);
  //WiFi连接
  WiFi_Connet();
  
  //连接MQTT
  MQTT_Connect();

  pubInfortaion();
}

void loop() 
{
  if(count >= 3)
  {
    count = 0;
    pubMQTTmsg();
  }
  if(espClient.connected())
  {
    espClient.loop();
  }
}



