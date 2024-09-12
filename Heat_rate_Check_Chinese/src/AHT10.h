#ifndef __AHT10_H__
#define __AHT10_H__
class AHT10
{
    private:
        int I2C_ADDR;
    public:
        float humidity,temperature;
        AHT10(int SCL,int SDA,int I2C_ADDR);
        void GetData();

};
#endif
