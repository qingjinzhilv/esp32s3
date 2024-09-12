#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>
#include <Arduino.h>

void connectMQTTserver();
void receiveCallback(char* topic, byte* payload, unsigned int length);
void connectWifi();
void subscribeTopic();
void pubMQTTmsg();
void tickerCount();
 
// 设置wifi接入信息(请根据您的WiFi信息进行修改)
const char* ssid = "别摆烂了哦";
const char* password = "00000001";
const char* mqttServer = "test.ranye-iot.net";

uint8_t count = 0;
 
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
 
const int subQoS = 1;     // 客户端订阅主题时使用的QoS级别（截止2020-10-07，仅支持QoS = 1，不支持QoS = 2）
const bool cleanSession = false; // 清除会话（如QoS>0必须要设为false）
 
const char* willTopic = "willTopic"; // 遗嘱主题名称
const char* willMsg = "willMsg";     // 遗嘱主题信息
const int willQos = 0;               // 遗嘱QoS
const int willRetain = false;        // 遗嘱保留
 
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // 设置板上LED引脚为输出模式
  digitalWrite(LED_BUILTIN, HIGH);  // 启动后关闭板上LED
  Serial.begin(9600);               // 启动串口通讯
  
  //设置ESP8266工作模式为无线终端模式
  WiFi.mode(WIFI_STA);
  
  // 连接WiFi
  connectWifi();
  
  // 设置MQTT服务器和端口号
  mqttClient.setServer(mqttServer, 1883);
  mqttClient.setCallback(receiveCallback);
 
  // 连接MQTT服务器
  connectMQTTserver();
}
 
void loop() {
  // 如果开发板未能成功连接服务器，则尝试连接服务器
  if (!mqttClient.connected()) {
    connectMQTTserver();
  }
 
   // 处理信息以及心跳
   mqttClient.loop();
   
}
 
// 连接MQTT服务器并订阅信息
void connectMQTTserver(){
  // 根据ESP8266的MAC地址生成客户端ID（避免与其它ESP8266的客户端ID重名）
  String clientId = "client-" + WiFi.macAddress();
 
  /* 连接MQTT服务器
  boolean connect(const char* id, const char* user, 
                  const char* pass, const char* willTopic, 
                  uint8_t willQos, boolean willRetain, 
                  const char* willMessage, boolean cleanSession); 
  若让设备在离线时仍然能够让qos1工作，则connect时的cleanSession需要设置为false                
  */
  if (mqttClient.connect(clientId.c_str(), NULL, NULL, willTopic, willQos, willRetain, willMsg, cleanSession)) { 
    Serial.print("MQTT Server Connected. ClientId: ");
    Serial.println(clientId);
    subscribeTopic(); // 订阅指定主题
  } else {
    Serial.print("MQTT Server Connect Failed. Client State:");
    Serial.println(mqttClient.state());
    delay(5000);
  }   
}
 
// 收到信息后的回调函数
void receiveCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message Received [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println("");
  Serial.print("Message Length(Bytes) ");
  Serial.println(length);
 
  if ((char)payload[0] == '1') {     // 如果收到的信息以“1”为开始
    digitalWrite(BUILTIN_LED, LOW);  // 则点亮LED。
  } else {                           
    digitalWrite(BUILTIN_LED, HIGH); // 否则熄灭LED。
  }
}
 
// 订阅指定主题
void subscribeTopic(){
  // 建立订阅主题。主题名称以Taichi-Maker-Sub为前缀，后面添加设备的MAC地址。
  // 这么做是为确保不同设备使用同一个MQTT服务器测试消息订阅时，所订阅的主题名称不同
  String topicString = "Taichi-Maker-Sub-" + WiFi.macAddress();
  char subTopic[topicString.length() + 1];  
  strcpy(subTopic, topicString.c_str());
  
  // 通过串口监视器输出是否成功订阅主题以及订阅的主题名称
  // 请注意subscribe函数第二个参数数字为QoS级别。这里为QoS = 1
  if(mqttClient.subscribe(subTopic, subQoS)){
    Serial.print("Subscribed Topic: ");
    Serial.println(subTopic);
  } else {
    Serial.print("Subscribe Fail...");
  }  
}
 
// ESP8266连接wifi
void connectWifi(){
 
  WiFi.begin(ssid, password);
 
  //等待WiFi连接,成功连接后输出成功信息
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected!");  
  Serial.println(""); 
}
