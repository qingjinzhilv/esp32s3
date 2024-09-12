#ifndef __AHT21_H__
#define __AHT21_H__
class AHT21
{
    private:
        int I2C_ADDR;
    public:
        float humidity,temperature;
        AHT21(int SCL,int SDA,int I2C_ADDR);
        void GetData();

};
#endif
