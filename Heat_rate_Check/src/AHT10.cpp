#include "AHT10.h"
#include <Arduino.h>
#include <Wire.h>

AHT10::AHT10(int SCL,int SDA,int I2C_ADDR)
{
    Wire1.begin(SDA,SCL);
    this->I2C_ADDR = I2C_ADDR;
}
void AHT10::GetData()
{
    
    Wire1.beginTransmission(I2C_ADDR);
    Wire1.write(0xAC);
    Wire1.write(0x33);
    Wire1.write(0x00);
    Wire1.endTransmission();
    delay(10);
    Wire1.requestFrom(I2C_ADDR, 6);
    byte data[6];
    for (int i = 0; i < 6; i++)
    {
        data[i] = Wire1.read();
    }
    this->humidity = ((data[1] << 12) | (data[2] << 4) | (data[3] >> 4))/1048576.0;
    this->temperature = ((((data[3] & 0x0F) << 16) | (data[4] << 8) | data[5])/1048576.0*200)-50;
}
