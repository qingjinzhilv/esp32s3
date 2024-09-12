#include "main.h"

WiFiClient wifiClient;
PubSubClient espClient(wifiClient);



//连接wifi以及阿里云
const char *mqttServer = "a1qnSfZPrQF.iot-as-mqtt.cn-shanghai.aliyuncs.com";
const int port = 1883;

const char *clientid = "a1qnSfZPrQF.esp32|securemode=2,signmethod=hmacsha256,timestamp=1711272542645|";
const char *mqttUser = "esp32&a1qnSfZPrQF";
const char *mqttPassword = "9e466e6d1402023a320ebffde29c666fff6ae93e7aed3ae0e6ee52529800f999";

const char *aliyunpublish_Infortation = "/sys/a1qnSfZPrQF/esp32/thing/event/property/post";
const char *aliyunSub_Infortation = "/sys/a1qnSfZPrQF/esp32/thing/event/property/post_reply";

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
  // Serial.print("Message arrived[");
  // Serial.print(topic);
  // Serial.print("[");
  payload[length] = '\0';
  String message = String((char *)payload);
  // Serial.println(message);
  Data_Analysis(message);
}

void Data_Analysis(String Data)
{
  uint8_t led_n = 0;
  led_n = Data.indexOf("led");
  char LED_at[3];
  char buf[100];
  Data.toCharArray(buf,100);
  if(led_n > 10 && led_n != 255)
  {
    strncpy(LED_at,buf + led_n + 5,1);
    // Serial.print("LED State->");
    LED_flag = atoi(LED_at);
    // Serial.println(LED_flag);
    digitalWrite(9,LED_flag);
  }
}



void InFormation_double_Pub(char Pub[],double data)
{
  char pubArray[50];
  sprintf(pubArray,Pub,data);
  // Serial.println(pubArray);
  if(!espClient.publish(aliyunpublish_Infortation,pubArray))
  {
    Serial.println("publish Failed.");
  }
}

void InFormation_int_Pub(char Pub[],int data)
{
  char pubArray[50];
  sprintf(pubArray,Pub,data);
  // Serial.println(pubArray);
  if(!espClient.publish(aliyunpublish_Infortation,pubArray))
  {
    Serial.println("publish Failed.");
  }
}

void MQTT_Keep(void)
{
    if(espClient.connected())
    {
        espClient.loop();
    }
}
