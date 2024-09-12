#include "MyI2C.h"

void MyI2C_W_SCL(uint8_t BitValue)   //引脚封装改名
{
    digitalWrite(SCL_PIN,BitValue);
    delayMicroseconds(10);
}

void MyI2C_W_SDA(uint8_t BitValue)
{
	digitalWrite(SDA_PIN,BitValue);
    delayMicroseconds(10);
}

uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
    BitValue = digitalRead(SDA_PIN);
    delayMicroseconds(10);
    return BitValue;
}

void MyI2C_Init(void)
{
    pinMode(SCL_PIN,OUTPUT | INPUT);
    pinMode(SDA_PIN,OUTPUT | INPUT);
    digitalWrite(SCL_PIN,1);
    digitalWrite(SDA_PIN,1);  //SCL SDA处于高电平，此时I2C总线处于空闲状态
}

void MyI2C_Start(void)   //起始条件时SDA比SCL先低电平
{
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);    //SCL高电平时，SDA下降沿为起始条件
	MyI2C_W_SDA(0);   //先拉低SDA电平
	MyI2C_W_SCL(0);
	
}

void MyI2C_Stop(void)    //终止条件是SCL比SDA先高低平
{
	MyI2C_W_SDA(0);     //SCL高电平时，SDA上升沿为终止条件
	MyI2C_W_SCL(1);    //先拉高SCL电平
	MyI2C_W_SDA(1);
	
}

//发送一个字节，是发8位
void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for(i = 0;i < 8;i++)
	{
		MyI2C_W_SDA(Byte & (0x80 >> i));   //>>i右移i位
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
	}
	
//		MyI2C_W_SDA(Byte & 0x80); 
//      相当于
//		if((Byte & 0x80) == 0)
//		{
//			MyI2C_W_SDA(0);
//		}
//		else
//		{
//			MyI2C_W_SDA(1);
//		}
//		MyI2C_W_SCL(1);
//		MyI2C_W_SCL(0);

}

//接收一个字节，是接收8位
uint8_t MyI2C_ReceiveByte(void)  //接收数据时，SCL必须为高电平，然后SDA不能改变
{
	uint8_t i,Byte = 0x00;
	MyI2C_W_SDA(1);
	
	for(i = 0;i< 8;i++)
	{
		MyI2C_W_SCL(1);
		if(MyI2C_R_SDA() == 1)
		{
			Byte |= (0x80 >> i);   //把Byte最高位置1   >>i 意思是右移i位
		}
		MyI2C_W_SCL(0);  //读取一位之后，把SCL拉低
	}
	return Byte;
}

//发送应答就是发1位
void MyI2C_SendAck(uint8_t AckBit)  
{									//函数进来时，SCL低电平
	MyI2C_W_SDA(AckBit);       //主机把AckBit放到SDA上
	MyI2C_W_SCL(1);  		   //SCL高电平，从机读取应答
	MyI2C_W_SCL(0);    //SCL低电平，进入下一个时序单元
}

//接收应答就是收1位
uint8_t MyI2C_ReceiveAck(void)  //
{									//函数进来时，SCL低电平
	uint8_t AckBit;
	MyI2C_W_SDA(1);       //主机主机释放SDA，防止干扰从机   同时，从机把答应位放在SDA上   
	MyI2C_W_SCL(1);       //SCL高电平，主机读取应答位
	AckBit = MyI2C_R_SDA();  //即使之前主机把SDA置1了，之后再读取SDA，读到的值也有可能是0   读到0代表从机给了应答，读到1代表从机没给应答
	MyI2C_W_SCL(0);  //读取一位之后，把SCL拉低
	return AckBit;
}
