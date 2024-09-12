#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>
#include <Arduino.h>

//设置wifi和连接的MQTT服务器地址信息
const char *ssid = "别摆烂了哦";
const char *password = "00000001";
const char *mqttServer = "test.ranye-iot.net";

//定义客户端相关信息
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

//ESP连接wifi
void connectWifi()
{
  WiFi.begin(ssid,password);

  //等待wifi连接
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected!");
  Serial.println("");
}

void connectMQTTServer()
{
  //设置MQTT客户端ID
  String cliented = "qingjin" + WiFi.macAddress();

  //连接MQTT服务器
  if(mqttClient.connect(cliented.c_str()))
  {
    Serial.println("MQTT Server Connected.");
    Serial.println("Serial Address: ");
    Serial.println(mqttServer);
    Serial.println("ClientID: ");
    Serial.println(cliented);
  }
  else
  {
    Serial.println("MQTT Server Connect Failed.Client State:");
    Serial.println(mqttClient.state());
    delay(1000);
  }
}

//发布信息
void pubRetMQTTmsg()
{
  //订阅主题
  String topicString = "qingjin-Ret" + WiFi.macAddress();
  char publishTopic[topicString.length() + 1];
  strcpy(publishTopic,topicString.c_str());

  //建立发布的保留信息
  //若发布的信息为空，即删除保留的信息
  String messageString = "qingjinzhilv";
  char publishMsg[messageString.length() + 1];
  strcpy(publishMsg,messageString.c_str());

  //实现esp向主题发布retain信息
  //三个参数分是  发布的主题  发布的信息 是否保留信息
  if(mqttClient.publish(publishTopic,publishMsg,true))
  {
    Serial.println("Publish Topic:");Serial.println(publishTopic);
    Serial.println("Publish Retained message:");Serial.println(publishMsg);
  }
  else
  {
    Serial.println("Message Publis Failed.");
  }
}







void setup()
{
  Serial.begin(9600);

  //设置WiFi模式
  WiFi.mode(WIFI_STA);

  //连接WiFi
  connectWifi();

  //设置MQTT服务器和端口号
  mqttClient.setServer(mqttServer,1883);
  
  //连接MQTT服务器
  connectMQTTServer();

  if(mqttClient.connected())
  {
    pubRetMQTTmsg();
  }
}

void loop()
{
  mqttClient.loop();  //保持心跳
}
