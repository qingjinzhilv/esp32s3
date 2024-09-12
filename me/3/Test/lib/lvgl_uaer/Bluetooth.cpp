/*
  视频: https://www.youtube.com/watch?v=oCMOYS71NIU
  基于 IDF 的 Neil Kolban 示例: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
  由 Evandro Copercini 移植到 Arduino ESP32
  创建一个BLE服务器，一旦我们收到连接，将会周期性发送通知
  T使用步骤：
  1. 创建一个 BLE Server
  2. 创建一个 BLE Service
  3. 创建一个 BLE Characteristic
  4. 创建一个 BLE Descriptor
  5. 开始服务
  6. 开始广播
*/
#include <head.h>

uint8_t txValue = 0;                         //后面需要发送的值
BLEServer *pServer = NULL;                   //BLEServer指针 pServer
BLECharacteristic *pTxCharacteristic = NULL; //BLECharacteristic指针 pTxCharacteristic
bool deviceConnected = false;                //本次连接状态
bool oldDeviceConnected = false;             //上次连接状态d
// 有关生成 UUID 的信息，请参阅以下内容: https://www.uuidgenerator.net/
#define SERVICE_UUID "12a59900-17cc-11ec-9621-0242ac130002" // UART service UUID
#define CHARACTERISTIC_UUID_RX "12a59e0a-17cc-11ec-9621-0242ac130002"
#define CHARACTERISTIC_UUID_TX "12a5a148-17cc-11ec-9621-0242ac130002"
 
// 获取蓝牙连接状态
class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
      //本次连接状态
      deviceConnected = true;
    };
    void onDisconnect(BLEServer *pServer)
    {
      //本次连接状态
      deviceConnected = false;
    }
};

//接收消息回调函数
class MyCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        std::string rxValue = pCharacteristic->getValue(); //接收信息
 
        if (rxValue.length() > 0)
        { 
            //向串口输出收到的值
            Serial.print("RX: ");
            for (int i = 0; i < rxValue.length(); i++)
            {
                Serial.print(rxValue[i]);
            }
            Serial.println();
        }
    }
};


void Bluetooth_Init(String name)
{
    // 创建一个 BLE 设备
    BLEDevice::init(name.c_str());//在这里面是ble的名称
    // 创建一个 BLE 服务
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks()); //设置连接状态回调函数
    BLEService *pService = pServer->createService(SERVICE_UUID);
    // 创建一个 BLE 特征
    pTxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);
    pTxCharacteristic->addDescriptor(new BLE2902());
    BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
    pRxCharacteristic->setCallbacks(new MyCallbacks()); //设置接收消息回调函数
    pService->start();                  // 开始服务
    pServer->getAdvertising()->start(); // 开始广播
    Serial.println(" 等待一个客户端连接，且发送通知... ");
}

void Bluetooth_connect_judg(void)
{
    //已连接
    if (deviceConnected)
    {
        pTxCharacteristic->setValue(&txValue, sizeof(txValue)); // 设置要发送的值为1
        pTxCharacteristic->notify();              // 广播
        txValue++;                                // 指针数值自加1
        delay(500);                              // 如果有太多包要发送，蓝牙会堵塞
    }
 
    // 断开连接
    if (!deviceConnected && oldDeviceConnected)
    {
        delay(500);                  // 留时间给蓝牙缓冲
        pServer->startAdvertising(); // 重新广播
        Serial.println("蓝牙已断开！");
        Serial.println("开始广播 ");
        oldDeviceConnected = deviceConnected;
    }
 
    // 正在连接
    if (deviceConnected && !oldDeviceConnected)
    {
        //在连接上做一些事情
        oldDeviceConnected = deviceConnected;
        Serial.println("正在连接......");
    }
}

void Bluetooth_Send(String data)
{

}

