#include <ESP8266WiFi.h>
 
void setup() {
  Serial.begin(115200);  //设置串口波特率，以便打印信息
  delay(5000);           //延时5s
  WiFi.mode(WIFI_STA);   //设置为无线终端模式
  WiFi.disconnect();     //断开，不连接到任何一个接入点
  delay(100);
  Serial.println("STA建立完成");
}
 
void loop() {
  Serial.println("同步扫描开始");
  int n = WiFi.scanNetworks();    //开始同步扫描，将返回值
  Serial.println("同步扫描结束");   //存放在变量n中
  if (n == 0){
    Serial.println("找不到网络");
  }else{
    Serial.println("发现网络");

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
    wifi wifi_Inforation[50];
    
    for (int i = 0; i < n; i++) //逐个打印扫描到的接入点信息
    {
       //获取扫描到的接入点网络信息(网络名称，信号强度等）
       WiFi.getNetworkInfo(i, wifi_Inforation[i].ssid, wifi_Inforation[i].encryptionType, wifi_Inforation[i].RSSI, wifi_Inforation[i].BSSID, wifi_Inforation[i].channel, wifi_Inforation[i].isHidden);
       //打印扫描到的接入点网络信息(网络名称，信号强度等）
       Serial.printf("%d: %s, Ch:%d, (%ddBm) %s %s\n", i + 1, wifi_Inforation[i].ssid.c_str(), wifi_Inforation[i].channel, wifi_Inforation[i].RSSI, wifi_Inforation[i].encryptionType == ENC_TYPE_NONE ? "开放" : "加密", wifi_Inforation[i].isHidden ? "隐藏" : "显示");
    }
    
    Serial.println("");
    delay(5000);// 延时5s之后再次扫描
  }
}