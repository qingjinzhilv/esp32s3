#include "myMPU6050.h"

#define MPU6050_ADDRESS    0xD0   //MPU6050的从机地址

//封装指定地址写一个字节    指定地址就是设置了MPU6050的当前地址指针
void MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data)
{
	MyI2C_Start();                      //起始
	MyI2C_SendByte(MPU6050_ADDRESS);    //发送一个字节，是0xD0，从机地址+读写位
	MyI2C_ReceiveAck();                 //接收应答位
	MyI2C_SendByte(RegAddress);	        //第二个字节，指定寄存器地址
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);               //第三个字节，指定要写入指定寄存器地址下的数据
	MyI2C_ReceiveAck();
	MyI2C_Stop();                       //终止时序
}

//指定地址读一个字节
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;

	MyI2C_Start();                      //起始
	MyI2C_SendByte(MPU6050_ADDRESS);    //发送一个字节，是0xD0，从机地址+读写位
	MyI2C_ReceiveAck();                 //接收应答位
	MyI2C_SendByte(RegAddress);	        //第二个字节，指定寄存器地址
	MyI2C_ReceiveAck();

	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDRESS | 0x01);
	MyI2C_ReceiveAck();
	Data = MyI2C_ReceiveByte();
	MyI2C_SendAck(1);
	MyI2C_Stop();

	return Data;

}

// void MPU6050_Init(void)
// {
// 	MyI2C_Init();
// 	MPU6050_WriteReg(MPU6050_PWR_MGMT_1,0x01);  //解除睡眠，选择陀螺仪时钟
// 	MPU6050_WriteReg(MPU6050_PWR_MGMT_2,0x00);  //6个轴均不待机
// 	MPU6050_WriteReg(MPU6050_SMPLRT_DIV,0x09);  //采样分频为10
// 	MPU6050_WriteReg(MPU6050_CONFIG,0x06);      //滤波参数给最大
// 	MPU6050_WriteReg(MPU6050_GYRO_CONFIG,0x18); //陀螺仪选择最大量程
// 	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG,0x18);//加速度计选择最大量程
// }

// uint8_t MPU6050_GetID(void)
// {
// 	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
// }

// void MPU6050_GetData(int16_t *AccX,int16_t *AccY,int16_t *AccZ,int16_t *GyroX,int16_t *GyroY,int16_t *GyroZ)
// {
// 	uint8_t DataH,DataL;

// 	//加速度计
// 	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
// 	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
// 	*AccX = (DataH << 8) | DataL;

// 	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
// 	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
// 	*AccY = (DataH << 8) | DataL;

// 	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
// 	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
// 	*AccZ = (DataH << 8) | DataL;

// 	//陀螺仪
// 	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
// 	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
// 	*GyroX = (DataH << 8) | DataL;

// 	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
// 	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
// 	*GyroY = (DataH << 8) | DataL;

// 	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
// 	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
// 	*GyroZ = (DataH << 8) | DataL;
// }

/**********************************************
函数名称：MPU_Init
函数功能：初始化MPU6050
函数参数：无
函数返回值：0,初始化成功  其他,初始化失败
**********************************************/
uint8_t MPU_Init(void)
{
	uint8_t res;

	MyI2C_Init();
	// 初始化IIC总线
	MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X80); // 复位MPU6050
	delay(100);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X00); // 唤醒MPU6050
	MPU_Set_Gyro_Fsr(3);					 // 陀螺仪传感器,±2000dps
	MPU_Set_Accel_Fsr(0);					 // 加速度传感器,±2g
	MPU_Set_Rate(50);						 // 设置采样率50Hz
	MPU_Write_Byte(MPU_INT_EN_REG, 0X00);	 // 关闭所有中断
	MPU_Write_Byte(MPU_USER_CTRL_REG, 0X00); // I2C主模式关闭
	MPU_Write_Byte(MPU_FIFO_EN_REG, 0X00);	 // 关闭FIFO
	MPU_Write_Byte(MPU_INTBP_CFG_REG, 0X80); // INT引脚低电平有效

	res = MPU_Read_Byte(MPU_DEVICE_ID_REG);
	if (res == MPU_ADDR) // 器件ID正确,即res = MPU_ADDR = 0x68
	{
		MPU_Write_Byte(MPU_PWR_MGMT1_REG, 0X01); // 设置CLKSEL,PLL X轴为参考
		MPU_Write_Byte(MPU_PWR_MGMT2_REG, 0X00); // 加速度与陀螺仪都工作
		MPU_Set_Rate(100);						 // 设置采样率为50Hz
	}
	else
		return 1; // 地址设置错误,返回1
	return 0;	  // 地址设置正确,返回0
}

/**********************************************
函数名称：MPU_Set_Gyro_Fsr
函数功能：设置MPU6050陀螺仪传感器满量程范围
函数参数：fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
函数返回值：0,设置成功  其他,设置失败
**********************************************/
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG, fsr << 3); // 设置陀螺仪满量程范围
}

/**********************************************
函数名称：MPU_Set_Accel_Fsr
函数功能：设置MPU6050加速度传感器满量程范围
函数参数：fsr:0,±2g;1,±4g;2,±8g;3,±16g
函数返回值：0,设置成功  其他,设置失败
**********************************************/
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG, fsr << 3); // 设置加速度传感器满量程范围
}

/**********************************************
函数名称：MPU_Set_LPF
函数功能：设置MPU6050的数字低通滤波器
函数参数：lpf:数字低通滤波频率(Hz)
函数返回值：0,设置成功  其他,设置失败
**********************************************/
uint8_t MPU_Set_LPF(uint16_t lpf)
{
	uint8_t data = 0;

	if (lpf >= 188)
		data = 1;
	else if (lpf >= 98)
		data = 2;
	else if (lpf >= 42)
		data = 3;
	else if (lpf >= 20)
		data = 4;
	else if (lpf >= 10)
		data = 5;
	else
		data = 6;
	return MPU_Write_Byte(MPU_CFG_REG, data); // 设置数字低通滤波器
}

/**********************************************
函数名称：MPU_Set_Rate
函数功能：设置MPU6050的采样率(假定Fs=1KHz)
函数参数：rate:4~1000(Hz)  初始化中rate取50
函数返回值：0,设置成功  其他,设置失败
**********************************************/
uint8_t MPU_Set_Rate(uint16_t rate)
{
	uint8_t data;
	if (rate > 1000)
		rate = 1000;
	if (rate < 4)
		rate = 4;
	data = 1000 / rate - 1;
	data = MPU_Write_Byte(MPU_SAMPLE_RATE_REG, data); // 设置数字低通滤波器
	return MPU_Set_LPF(rate / 2);					  // 自动设置LPF为采样率的一半
}

/**********************************************
函数名称：MPU_Get_Temperature
函数功能：得到温度传感器值
函数参数：无
函数返回值：温度值(扩大了100倍)
**********************************************/
short MPU_Get_Temperature(void)
{
	uint8_t buf[2];
	short raw;
	float temp;

	MPU_Read_Len(MPU_ADDR, MPU_TEMP_OUTH_REG, 2, buf);
	raw = ((uint16_t)buf[0] << 8) | buf[1];
	temp = 36.53 + ((double)raw) / 340;
	return temp * 100;
}

/**********************************************
函数名称：MPU_Get_Gyroscope
函数功能：得到陀螺仪值(原始值)
函数参数：gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
函数返回值：0,读取成功  其他,读取失败
**********************************************/
uint8_t MPU_Get_Gyroscope(short *gx, short *gy, short *gz)
{
	uint8_t buf[6], res;

	res = MPU_Read_Len(MPU_ADDR, MPU_GYRO_XOUTH_REG, 6, buf);
	if (res == 0)
	{
		*gx = ((uint16_t)buf[0] << 8) | buf[1];
		*gy = ((uint16_t)buf[2] << 8) | buf[3];
		*gz = ((uint16_t)buf[4] << 8) | buf[5];
	}
	return res;
}

/**********************************************
函数名称：MPU_Get_Accelerometer
函数功能：得到加速度值(原始值)
函数参数：ax,ay,az:加速度传感器x,y,z轴的原始读数(带符号)
函数返回值：0,读取成功  其他,读取失败
**********************************************/
uint8_t MPU_Get_Accelerometer(short *ax, short *ay, short *az)
{
	uint8_t buf[6], res;
	res = MPU_Read_Len(MPU_ADDR, MPU_ACCEL_XOUTH_REG, 6, buf);
	if (res == 0)
	{
		*ax = ((uint16_t)buf[0] << 8) | buf[1];
		*ay = ((uint16_t)buf[2] << 8) | buf[3];
		*az = ((uint16_t)buf[4] << 8) | buf[5];
	}
	return res;
}

/**********************************************
函数名称：MPU_Write_Len
函数功能：IIC连续写(写器件地址、寄存器地址、数据)
函数参数：addr:器件地址      reg:寄存器地址
				 len:写入数据的长度  buf:数据区
函数返回值：0,写入成功  其他,写入失败
**********************************************/
uint8_t MPU_Write_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
	uint8_t i;

	MyI2C_Start();
	MyI2C_SendByte((addr << 1) | 0); // 发送器件地址+写命令(0为写,1为读)
	MyI2C_ReceiveAck();
	MyI2C_SendByte(reg); // 写寄存器地址
	MyI2C_ReceiveAck();	 // 等待应答
	for (i = 0; i < len; i++)
	{
		MyI2C_SendByte(buf[i]); // 发送数据
		MyI2C_ReceiveAck();
	}
	MyI2C_Stop();
	return 0;
}

/**********************************************
函数名称：MPU_Read_Len
函数功能：IIC连续读(写入器件地址后,读寄存器地址、数据)
函数参数：addr:器件地址        reg:要读的寄存器地址
				 len:要读取的数据长度  buf:读取到的数据存储区
函数返回值：0,读取成功  其他,读取失败
**********************************************/
uint8_t MPU_Read_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
	MyI2C_Start();
	MyI2C_SendByte((addr << 1) | 0); // 发送器件地址+写命令
	MyI2C_ReceiveAck();
	MyI2C_SendByte(reg); // 写寄存器地址
	MyI2C_ReceiveAck();	 // 等待应答
	MyI2C_Start();
	MyI2C_SendByte((addr << 1) | 1); // 发送器件地址+读命令
	MyI2C_ReceiveAck();				 // 等待应答
	while (len)
	{
		if (len == 1)
		{
			*buf = MyI2C_ReceiveByte(); // 读数据,发送nACK
			MyI2C_SendAck(1);			// 发送nACK
		}
		else
		{
			*buf = MyI2C_ReceiveByte(); // 读数据,发送ACK
			MyI2C_SendAck(0);
		}
		len--;
		buf++;
	}
	MyI2C_Stop(); // 产生一个停止条件
	return 0;
}

/**********************************************
函数名称：MPU_Write_Byte
函数功能：IIC写一个字节
函数参数：data:写入的数据    reg:要写的寄存器地址
函数返回值：0,写入成功  其他,写入失败
**********************************************/
uint8_t MPU_Write_Byte(uint8_t reg, uint8_t data)
{
	MPU6050_WriteReg(reg,data);
	return 1;
	// MyI2C_Start();
	// MyI2C_SendByte((MPU_ADDR << 1) | 0); // 发送器件地址+写命令
	// if (MyI2C_ReceiveAck())				 // 等待应答
	// {
	// 	MyI2C_Stop();
	// 	return 1;
	// }
	// MyI2C_SendByte(reg);	// 写寄存器地址
	// MyI2C_ReceiveAck();		// 等待应答
	// MyI2C_SendByte(data);	// 发送数据
	// if (MyI2C_ReceiveAck()) // 等待ACK
	// {
	// 	MyI2C_Stop();
	// 	return 1;
	// }
	// MyI2C_Stop();
	// return 0;
}

/**********************************************
函数名称：MPU_Read_Byte
函数功能：IIC读一个字节
函数参数：reg:要读的寄存器地址
函数返回值：res:读取到的数据
**********************************************/
uint8_t MPU_Read_Byte(uint8_t reg)
{
	uint8_t res;
	res = MPU6050_ReadReg(reg);
	

	// MyI2C_Start();
	// MyI2C_SendByte((MPU_ADDR << 1) | 0); // 发送器件地址+写命令
	// MyI2C_ReceiveAck();					 // 等待应答
	// MyI2C_SendByte(reg);				 // 写寄存器地址
	// MyI2C_ReceiveAck();					 // 等待应答
	// MyI2C_Start();
	// MyI2C_SendByte((MPU_ADDR << 1) | 1); // 发送器件地址+读命令
	// MyI2C_ReceiveAck();					 // 等待应答
	// res = MyI2C_ReceiveByte();			 // 读取数据,发送nACK
	// MyI2C_SendAck(1);
	// MyI2C_Stop(); // 产生一个停止条件
	return res;
}
