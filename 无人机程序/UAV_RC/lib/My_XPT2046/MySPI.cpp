#include "MySPi.h"

void MySPI_W_SS(uint8_t BitValue)  //SS也可以写成CS
{
	digitalWrite(SPI_CS_PIN,(bool)BitValue);          //bool是表示高电平还是低电平
}
void MySPI_W_SCK(uint8_t BitValue)
{
	digitalWrite(SPI_CLK_PIN,(bool)BitValue);
}
void MySPI_W_MOSI(uint8_t BitValue)
{
	digitalWrite(SPI_MOSI_PIN,(bool)BitValue);
}
uint8_t MySPI_R_MISO(void)
{
	return digitalRead(SPI_MISO_PIN);
}
void MySPI_Init(void)
{
	pinMode(SPI_CS_PIN,OUTPUT);
	pinMode(SPI_CLK_PIN,OUTPUT);
	pinMode(SPI_MOSI_PIN,OUTPUT);
	pinMode(SPI_MISO_PIN,INPUT);
	//初始化之后引脚默认电平        MOSI没有明确规定，可以不管  MISO是输入引脚,不用输出电平   
	MySPI_W_SS(1);         //SS初始化后默认置高电平,即默认不选中从机
	MySPI_W_SCK(0);        //因为计划使用SPI模式0，所以默认电平为低电平
	
}

												//针对模式0

//起始信号
void MySPI_Start(void)   
{
	MySPI_W_SS(0);
}

//终止信号
void MySPI_Stop(void)
{
	MySPI_W_SS(1);
}

//交换字节   方法1
/*
在SS下降沿之后，开始交换字节，SCK先有下降沿，然后再移出数据
顺序：先SS下降沿或SCK下降沿，再移出数据
可看成先后执行的逻辑，即先SS下降沿，再移出数据，再SCK上升沿，再移入数据，再SCK下降沿，再移出数据
*/
uint8_t MySPI_SwapByta(uint8_t ByteSend)  //传进来的数据,是要通过交换一个字节的时序发送出去
{
	uint8_t ByteReceive = 0x00;    //用于接收
	uint8_t i;
	
	for(i = 0;i < 8;i ++)
	{
		MySPI_W_MOSI(ByteSend & 0x80 >> i);  //发送的位是ByteSend的高8位，所以&0x80  但要保证这个函数是非0即1的特征
		MySPI_W_SCK(1);                      //使SCK产生上升沿，然后主机和从机同时移入数据  把从机刚才放在MISO的数据位读进来
		if(MySPI_R_MISO() == 1)
		{
			ByteReceive |= 0x80 >> i;       
		}
		MySPI_W_SCK(0);                      //产生下降沿
	}
	
	return ByteReceive;    //返回一个通过交换一个字节接收的数据
}

//交换字节   方法2
//uint8_t MySPI_SwapByta(uint8_t ByteSend)  
//{ 
//	uint8_t i;
//	
//	for(i = 0;i < 8;i ++)
//	{
//		MySPI_W_MOSI(ByteSend & 0x80);
//		ByteSend <<= 1;
//		MySPI_W_SCK(1);
//		if(MySPI_R_MISO() == 1)
//		{
//			ByteSend |= 0x01;
//		}
//		MySPI_W_SCK(0);
//	}
//	
//	return ByteSend;   
//}

