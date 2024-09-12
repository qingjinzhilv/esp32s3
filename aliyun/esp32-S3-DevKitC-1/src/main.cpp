#include <Arduino.h>
#include <Crypto.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>

WiFiClient wifiClient;
PubSubClient espClient(wifiClient);

Ticker ticker;

int count = 0;

char pubArray[50];

//连接wifi以及阿里云
const char *ssid = "okok";
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


void WiFi_Connet(void)
{
  WiFi.begin(ssid,password);

  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000); //等待扫描
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

TaskHandle_t TASK_HandleOne = NULL;
void GETWIFI(void *param);

void setup()
{
  Serial.begin(115200);

  pinMode(20,OUTPUT);

  ticker.attach(1,Timer);

  pinMode(20,OUTPUT);
  
  // WiFi.mode(WIFI_MODE_STA);
  // //WiFi连接
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




