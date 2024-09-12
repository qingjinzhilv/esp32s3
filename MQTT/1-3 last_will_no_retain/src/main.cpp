#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//连接的WiFi和服务端地址的信息
const char *ssid = "别摆烂了哦";
const char *password = "00000001";
const char *mqttServer = "test.ranye-iot.net";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

//遗嘱设置
const char *willMsg = "o no!";    //遗嘱信息
const int willQos = 0;            //遗嘱Qos等级 可以用0 1   Qos越高等级也越高
const bool willRetain = false;    //遗嘱保留    false不保留  true保留   即在意外断开服务器时，并且服务器也意外关闭，服务器重新连接后还能收到相应的信息

void connectWifi()
{
  WiFi.begin(ssid,password);

  while(WiFi.status() != WL_CONNECTED)
  {
    delay(200);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.println("");
}

//连接MQTT
void connectMQTTserver()
{
  //客户端ID
  String clientID = "qingjin" + WiFi.macAddress();

  //建立遗嘱主题
  String willString = "qingjin" + WiFi.macAddress() + "-Will";
  char willTopic[willString.length() + 1];
  strcpy(willTopic,willString.c_str());

  //连接MQTT服务器
  //函数的入口参数：
  if(mqttClient.connect(clientID.c_str(),willTopic,willQos,willRetain,willMsg))
  {
    Serial.println("MQTT Server Connected!");
    Serial.print("Server Address: ");Serial.println(mqttServer);
    Serial.print("Client Address: ");Serial.println(clientID);
    Serial.print("Will Topic: ");Serial.println(willTopic);
  }
  else
  {
    Serial.print("MQTT Server Connect Failed.Client State:");
    Serial.println(mqttClient.state());
    delay(1000);
  }
}



void setup()
{
  Serial.begin(9600);
  
  //设置wifi模式为无线终端模式
  WiFi.mode(WIFI_STA);

  //连接WiFi
  connectWifi();

  //设置连接的MQTT服务器地址和端口号
  mqttClient.setServer(mqttServer,1883);

  //设置心跳间隔时间  以s为单位
  mqttClient.setKeepAlive(10);

  //连接MQTT服务器
  connectMQTTserver();  

}

void loop()
{
  //如果未能成功连接MQTT服务器，将会重新来连接服务器
  if(!mqttClient.connected())
  {
    connectMQTTserver();
  }

  //处理信息以及心跳
  mqttClient.loop();
}
