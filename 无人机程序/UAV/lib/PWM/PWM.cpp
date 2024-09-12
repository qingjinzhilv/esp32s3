#include "PWM.h"
#include <stdio.h>

// 定义PWM输出GPIO
#define PWM1_GPIO          (42)
#define PWM2_GPIO          (6)
#define PWM3_GPIO          (15)
#define PWM4_GPIO          (38)
// 定义PWM通道
#define PWM1_CHANNEL            0
#define PWM2_CHANNEL            1
#define PWM3_CHANNEL            2
#define PWM4_CHANNEL            3

void PWM_init(void)
{
    ledcSetup(PWM1_CHANNEL, 1000, 10);
    ledcAttachPin(PWM1_GPIO, PWM1_CHANNEL);

    ledcSetup(PWM2_CHANNEL, 1000, 10);
    ledcAttachPin(PWM2_GPIO, PWM2_CHANNEL);

    ledcSetup(PWM3_CHANNEL, 1000, 10);
    ledcAttachPin(PWM3_GPIO, PWM3_CHANNEL);

    ledcSetup(PWM4_CHANNEL, 1000, 10);
    ledcAttachPin(PWM4_GPIO, PWM4_CHANNEL);
}

void Moto_Pwm(float MOTO1_PWM,float MOTO2_PWM,float MOTO3_PWM,float MOTO4_PWM){

	//限制数值在0-1000
	if(MOTO1_PWM>1000.0f)	    MOTO1_PWM = 1000.0f;
	if(MOTO2_PWM>1000.0f)	    MOTO2_PWM = 1000.0f;
	if(MOTO3_PWM>1000.0f)	    MOTO3_PWM = 1000.0f;
	if(MOTO4_PWM>1000.0f)	    MOTO4_PWM = 1000.0f;
	if(MOTO1_PWM<0.0f)			MOTO1_PWM = 0.0f;
	if(MOTO2_PWM<0.0f)			MOTO2_PWM = 0.0f;
	if(MOTO3_PWM<0.0f)			MOTO3_PWM = 0.0f;
	if(MOTO4_PWM<0.0f)			MOTO4_PWM = 0.0f;

    ledcWrite(PWM1_CHANNEL, MOTO1_PWM);
    ledcWrite(PWM2_CHANNEL, MOTO2_PWM);
    ledcWrite(PWM3_CHANNEL, MOTO3_PWM);
    ledcWrite(PWM4_CHANNEL, MOTO4_PWM);
}
