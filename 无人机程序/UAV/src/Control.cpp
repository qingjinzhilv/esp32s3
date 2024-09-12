#include <stdint.h>
#include <stdio.h>
#include "Control.h"
#include <stdbool.h>
#include "math.h"
#include "freertos/FreeRTOS.h"//RTOS头文件
#include "freertos/task.h"    //RTOS头文件
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "LED.h"
#include "PWM.h"
// #include "MyMPU6050.h" 
// #include "SPL06.h"
#include "Key.h"
#include "VBAT.h"
#include "Data_declaration.h"
#include "My_WIFI.h"
#include "My_ESPNOW.h"
#include "PID.h"

espnow_send_param_t espnow_send_param;

void Init(void)
{
	//创建互斥信号量时，会主动释放一次互斥信号量
	metux_semhore_handle = xSemaphoreCreateMutex();
	if(metux_semhore_handle != NULL)
	{
		printf("互斥信号量创建成功\r\n");
	}
	LED_Init();				//LED灯初始化
    Serial.begin(115200);	//串口初始化
	PWM_init();				//空心板电机初始化
	Key_Init();				//按键解锁初始化
	VBAT_Init();			//电压检测初始化
	// MyWiFi.WIFI_Connect("倾尽","00000001");		//连接WIFI
	My_espnow_Init();
	MPU6050_Init();			//MPU6050初始化
	Serial.printf("MPU6050 ID -> %X %d\r\n",MPU6050_GetID(),MPU6050_GetID());
	//PID初始化
	Pid_Init(&PID_Yaw);
	Pid_Init(&PID_Pitch);
	Pid_Init(&PID_Roll);
	//PID参数设置
	PidSet(&PID_Yaw,Yaw_kp,Yaw_ki,Yaw_kd,Yaw_imax);
	PidSet(&PID_Pitch,Pitch_kp,Pitch_ki,Pitch_kd,Pitch_imax);
	PidSet(&PID_Roll,Roll_kp,Roll_ki,Roll_kd,Roll_imax);
	//PID设置期望值
	pidSetTarget(&PID_Yaw,Yaw_Target);
	pidSetTarget(&PID_Pitch,Pitch_Target);
	pidSetTarget(&PID_Roll,Roll_Target);
	
	Moto_Pwm(0,0,0,0);
	/*********************SPL06 气压传感器初始化*********************/
	
	/*********************SPL06 气压传感器初始化*********************/
	vTaskDelay(pdMS_TO_TICKS(1000));
	
	init_ok = true;
}

void Task_Create_Init(void)
{
	//创建任务，获取陀螺仪数据
	xTaskCreate(Data_Task, "Data_Task", 1024 * 8, NULL, 5, NULL); 
}


void Data_Task(void *pvParameter)
{
	bool temp_1 = false;
	while(1)
	{
		//获取信号量
		xSemaphoreTake(metux_semhore_handle,0);
		MPU6050_Test();
		//解锁状态
		My_UAV_Data.unlock = My_UAV_RC_Data.unlock;
		digitalWrite(GPIO_NUM_16,!My_UAV_Data.unlock);
		//陀螺仪数据
		My_UAV_Data.UAV_MPU6050_Data.Yaw = My_MPU6050_Data.Yaw;
		My_UAV_Data.UAV_MPU6050_Data.Pitch = My_MPU6050_Data.Pitch;
		My_UAV_Data.UAV_MPU6050_Data.Roll = My_MPU6050_Data.Roll;
		//电压数据
		My_UAV_Data.UAV_Battery_Voltage = VBAT_Value;
		if(VBAT_Value <= 380)
		{
			//如果电池电压小于3v就闪烁警告
			temp_1 = !temp_1; //通过循环取反达到闪烁的效果
			digitalWrite(LED2, temp_1);
		}
		else digitalWrite(LED2, 0);//电池电量大于3v就常亮

		//油门值赋值
		My_UAV_Data.Throttle_value = My_UAV_RC_Data.throttle_value;
		
		// //初始化完成 && 遥控解锁
		if(init_ok && My_UAV_Data.unlock)
		{
			//PID控制输出 Yawn Pitch Roll
			Yaw_loop_out += PidIncCtrl(&PID_Yaw,My_MPU6050_Data.Yaw);
			Yaw_loop_out = constrain_float(Yaw_loop_out,PWM_MIN,PWM_MAX);
			
			Pitch_loop_out += PidIncCtrl(&PID_Pitch,My_MPU6050_Data.Pitch);
			Pitch_loop_out = constrain_float(Pitch_loop_out,PWM_MIN,PWM_MAX);
			
			Roll_loop_out += PidIncCtrl(&PID_Roll,My_MPU6050_Data.Roll);
			Roll_loop_out = constrain_float(Roll_loop_out,PWM_MIN,PWM_MAX);
			
			M1_PWM = Yaw_loop_out + Pitch_loop_out + Roll_loop_out + My_UAV_Data.Throttle_value;
			M2_PWM = Yaw_loop_out - Pitch_loop_out + Roll_loop_out + My_UAV_Data.Throttle_value;
			M3_PWM = -Yaw_loop_out - Pitch_loop_out - Roll_loop_out + My_UAV_Data.Throttle_value;
			M4_PWM = -Yaw_loop_out + Pitch_loop_out - Roll_loop_out + My_UAV_Data.Throttle_value;
			//将PWM值输入
			Moto_Pwm(M1_PWM,M2_PWM,M3_PWM,M4_PWM);
			// Serial.printf("M1_PWM->%d,M2_PWM->%d,M3_PWM->%d,M4_PWM->%d\r\n",M1_PWM,M2_PWM,M3_PWM,M4_PWM);
			// Serial.printf("油门值->%d\r\n",My_UAV_Data.Throttle_value);
		}
		else
		{
			Moto_Pwm(0,0,0,0);
		}
		//保护
		if(My_MPU6050_Data.Roll > 90 || My_MPU6050_Data.Roll < -90 || My_MPU6050_Data.Pitch > 90 || My_MPU6050_Data.Pitch < -90)
		{
			My_UAV_RC_Data.unlock = __unlock;
			Moto_Pwm(0,0,0,0);
		}
		//释放信号量
		xSemaphoreGive(metux_semhore_handle);
		delay(5);
	}
}

