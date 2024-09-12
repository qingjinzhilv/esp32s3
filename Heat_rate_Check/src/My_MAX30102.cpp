#include "My_MAX30102.h"

MAX30105 particleSensor;

uint32_t irBuffer[100]; //红外LED传感器数据
uint32_t redBuffer[100];  //红色 LED 传感器数据


int32_t bufferLength = 100; //数据长度 样本长度
int32_t spo2; //SPO2值
int8_t validSPO2; //指示器显示 SPO2 计算是否有效
int32_t heartRate; //心率值
int8_t validHeartRate; //指示心率计算是否有效的指示器


void MAX30102_Init(void)
{
    //MAX30102初始化
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //使用原始引脚 即11 12
  {
    Serial.println(F("MAX30105 was not found. Please check wiring/power."));
    while (1);
  }

  Serial.println(F("Attach sensor to finger with rubber band. Press any key to start conversion"));


  byte ledBrightness = 10; //光的亮度选项：0~255
  byte sampleAverage = 4; //样本平均值: 1, 2, 4, 8, 16, 32
  byte ledMode = 2; //led模式: 1 = 只有红色, 2 = 红色+红外线, 3 = 红色+红外线+绿色
  byte sampleRate = 100; //采样率: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //脉冲宽度: 69, 118, 215, 411
  int adcRange = 4096; //adc范围: 2048, 4096, 8192, 16384
  //使用这些设置配置传感器
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); 
}

void MAX30102_GetData(void)
{
  bufferLength = 100; //缓冲区长度为 100 可存储 4 秒以 25sps 运行的样本
  //读取前 100 个样本，并确定信号范围
  for (byte i = 0 ; i < bufferLength ; i++)
  {
    while (particleSensor.available() == false) //判断是否有新数据
    {
      particleSensor.check(); //检查传感器是否有新数据
    }

    redBuffer[i] = particleSensor.getRed();
    irBuffer[i] = particleSensor.getIR();
    particleSensor.nextSample(); //我们完成了这个样本，因此进入下一个样本

    Serial.print(F("red="));
    Serial.print(redBuffer[i], DEC);
    Serial.print(F(", ir="));
    Serial.println(irBuffer[i], DEC);
  }

  //计算前 100 个样本（样本的前 4 秒）后的心率和 SpO2
  maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

  //不断从MAX30102中取样。 心率和 SpO2 每 1 秒计算一次
  uint8_t ok = 4;
  while (ok --)
  {
    //将前 25 组样本转储到内存中，并将最后 75 组样本移到顶部
    for (byte i = 25; i < 100; i++)
    {
      redBuffer[i - 25] = redBuffer[i];
      irBuffer[i - 25] = irBuffer[i];
    }

    //在计算心率之前取 25 组样本。
    for (byte i = 75; i < 100; i++)
    {
      while (particleSensor.available() == false) //判断是否有新数据
        {
          particleSensor.check(); //检查传感器是否有新数据
        }


      redBuffer[i] = particleSensor.getRed();
      irBuffer[i] = particleSensor.getIR();
      particleSensor.nextSample(); //我们完成了这个样本，因此进入下一个样本

      Serial.print(F("red="));
      Serial.print(redBuffer[i], DEC);
      //IR表示脉搏
      Serial.print(F(", ir="));
      Serial.print(irBuffer[i], DEC);
      //，BPM表示心率，Avg BPM表示平均心率
      //心率值
      Serial.print(F(", HR="));
      Serial.print(heartRate, DEC);
      //指示心率计算是否有效的指示器
      Serial.print(F(", HRvalid="));
      Serial.print(validHeartRate, DEC);
      //血氧值
      Serial.print(F(", SPO2="));
      Serial.print(spo2, DEC);
      //指示血氧值计算是否有效的指示器
      Serial.print(F(", SPO2Valid="));
      Serial.println(validSPO2, DEC);
    }
    //收集 25 个新样本后，重新计算 HR 和 SP02
    maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
  }
}

