#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>

//连接的WiFi和服务端地址的信息
const char *ssid = "别摆烂了哦";
const char *password = "00000001";
const char *mqttServer = "test.ranye-iot.net";

Ticker ticker;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

//MQTT服务端连接用户密码
const char *mqttUserName = "test-user";
const char *mqttpassword = "ranye-iot";

int count = 0;     //Ticker计数变量名


void connectWiFi()
{
  WiFi.begin(ssid,password);

  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }
  Serial.println();
  Serial.println("WiFi connected!");
  Serial.println("");
}

void connectMQTTServer()
{
  String clientId = "qingjin" + WiFi.macAddress();

  if(mqttClient.connect(clientId.c_str(),mqttUserName,mqttpassword))
  {
    Serial.println("MQTT Server Connected.");
    Serial.print("Server Address: ");
    Serial.println(mqttServer);
    Serial.print("Client: ");
    Serial.println(clientId);
  }
  else
  {
    Serial.print("MQTT Server Connect failed.Client state:");
    Serial.println(mqttClient.state());
    delay(3000);
  }
}

void pubMQTTmsg()
{
  
  String topicString = "test-user/Pub" + WiFi.macAddress();
  char publishTopic[topicString.length() + 1];
  strcpy(publishTopic,topicString.c_str());

  static int value = 0;
  String messageString = "Hello World " + String(value ++);
  char PublishMsg[messageString.length() + 1];
  strcpy(PublishMsg,messageString.c_str());

  if(mqttClient.publish(publishTopic,PublishMsg))
  {
    Serial.print("Publish Topic:");Serial.println(publishTopic);
    Serial.print("Publish message:");Serial.println(PublishMsg);
  }
  else
  {
    Serial.println("Message Publish Failed.");
  }
}

void tickerCount()
{
  count ++;
}


void setup()
{
  Serial.begin(9600);

  //Ticker定时器对象
  ticker.attach(1,tickerCount);

  //设置WiFi工作模式
  WiFi.mode(WIFI_STA);

  //连接WiFi
  connectWiFi();

  //设置MQTT服务器和端口
  mqttClient.setServer(mqttServer,1883);

  //连接MQTT服务器
  connectMQTTServer();
}

void loop()
{
  //判端是否连接服务器
  if(!mqttClient.connected())
  {
    connectMQTTServer();
  }
  //保持心跳
  mqttClient.loop();
  if(count >= 3)
  {
    count = 0;
    pubMQTTmsg();
  }
}






