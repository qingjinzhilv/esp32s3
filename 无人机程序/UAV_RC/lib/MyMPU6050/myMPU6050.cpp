#include "myMPU6050.h"

#define MPU6050_ADDRESS    0xD0   //MPU6050的从机地址

int16_t AX, AY, AZ, GX, GY, GZ;			//定义用于存放各个数据的变量
MPU6050Params mpu6050 = 
{
    .MPU6050dt = 10,
    .preMillis = 0,
    .MPU6050ERROE = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
    .previousprintMillis = 0
};

MySensorMsg msg = {
	.A = {0.0f, 0.0f, 0.0f},
	.G = {0.0f, 0.0f, 0.0f}
};

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

void MPU6050_Init(void)
{
	MyI2C_Init();
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1,0x01);  //解除睡眠，选择陀螺仪时钟
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2,0x00);  //6个轴均不待机
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV,0x09);  //采样分频为10
	MPU6050_WriteReg(MPU6050_CONFIG,0x06);      //滤波参数给最大
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG,0x18); //陀螺仪选择最大量程
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG,0x18);//加速度计选择最大量程

	begin(1000.0f / (float)mpu6050.MPU6050dt);
	
	dataGetERROR();
	
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}


void MPU6050_GetData(int16_t *AccX,int16_t *AccY,int16_t *AccZ,int16_t *GyroX,int16_t *GyroY,int16_t *GyroZ)
{
	uint8_t DataH,DataL;
	
	//加速度计
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	*AccX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	*AccY = (DataH << 8) | DataL;

	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	*AccZ = (DataH << 8) | DataL;
	
	//陀螺仪
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	*GyroX = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	*GyroY = (DataH << 8) | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	*GyroZ = (DataH << 8) | DataL;
}

void dataGetERROR(void) 
{
	for(uint8_t i = 0; i < 100; ++i) {
		getMPU6050Data();
		mpu6050.MPU6050ERROE[0] += msg.A[0];
		mpu6050.MPU6050ERROE[1] += msg.A[1];
		mpu6050.MPU6050ERROE[2] += msg.A[2] - 9.8;
		mpu6050.MPU6050ERROE[3] += msg.G[0];
		mpu6050.MPU6050ERROE[4] += msg.G[1];
		mpu6050.MPU6050ERROE[5] += msg.G[2];
		vTaskDelay(pdMS_TO_TICKS(10));//延时
	}
	for(uint8_t i = 0; i < 6; ++i) 
	{
		mpu6050.MPU6050ERROE[i] /= 100.0f;
	}
}


void getMPU6050Data(void) 
{
	MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);		//获取MPU6050的数据
	msg.A[0] = (float)((float)AX / (float)32768) * 16 * 9.8;
	msg.A[1] = (float)((float)AY / (float)32768) * 16 * 9.8;
	msg.A[2] = (float)((float)AZ / (float)32768) * 16 * 9.8;
	msg.G[0] = (float)((float)GX / (float)32768) * 2000 * 3.5;
	msg.G[1] = (float)((float)GY / (float)32768) * 2000 * 3.5;
	msg.G[2] = (float)((float)GZ / (float)32768) * 2000 * 3.5;
	
}

void MPU6050Print(void) {
	/*OLED_ShowSignedNum(2, 1, msg.A[0], 3);					  //OLED显示数据
	OLED_ShowNum(2, 6, (uint32_t)(msg.A[0] * 100) % 100, 1);			
	OLED_ShowSignedNum(3, 1, msg.A[1], 3);
	OLED_ShowNum(3, 6, (uint32_t)(msg.A[1] * 100) % 100, 1);
	OLED_ShowSignedNum(4, 1, msg.A[2], 3);
	OLED_ShowNum(4, 6, (uint32_t)(msg.A[2] * 100) % 100, 1);
	OLED_ShowSignedNum(2, 8, msg.G[0], 3);
	OLED_ShowNum(2, 13, (uint32_t)(msg.G[0] * 100) % 100, 1);
	OLED_ShowSignedNum(3, 8, msg.G[1], 3);
	OLED_ShowNum(3, 13, (uint32_t)(msg.G[1] * 100) % 100, 1);
	OLED_ShowSignedNum(4, 8, msg.G[2], 3);
	OLED_ShowNum(4, 13, (uint32_t)(msg.G[2] * 100) % 100, 1);*/
}

void EularPrint(void) 
{
	My_MPU6050_Data.Yaw = getYaw();
	My_MPU6050_Data.Roll = getRoll();
	My_MPU6050_Data.Pitch = getPitch();
	
	printf("Yaw:%.2lf\r\n",My_MPU6050_Data.Yaw);
	
	printf("Roll:%.2lf\r\n",My_MPU6050_Data.Roll);
	
	printf("Pitch:%.2lf\r\n",My_MPU6050_Data.Pitch);		
}

void dataGetAndFilter(void) 
{
	getMPU6050Data();
	msg.A[0] -= mpu6050.MPU6050ERROE[0];
	msg.A[1] -= mpu6050.MPU6050ERROE[1];
	msg.A[2] -= mpu6050.MPU6050ERROE[2];
	msg.G[0] -= mpu6050.MPU6050ERROE[3];
	msg.G[1] -= mpu6050.MPU6050ERROE[4];
	msg.G[2] -= mpu6050.MPU6050ERROE[5];
}

void MPU6050_Test(void)
{
	if(millis() - mpu6050.preMillis >= mpu6050.MPU6050dt) 
	{
		mpu6050.preMillis = millis();
		dataGetAndFilter();		                            //获取MPU6050的数据
		updateIMU(msg.G[0], msg.G[1], msg.G[2], msg.A[0], msg.A[1], msg.A[2]);
	}

	EularPrint();
}
