/*
自平衡小车PD环
作者 鹅鹅鹅
时间 2018.5.7
*/

#include "led.h"
#include "delay.h"
#include "OLED.h"
#include "key.h" 
#include "usart.h"
#include "Motor.h"
#include "STE_Motor.h"
#include "PID.h"
#include "time5.h"
#include "mpu6050.h"
#include "McuDataSend.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "stdio.h"
#include "encoder.h"

extern float Curpitch;
extern float Curroll;
float Tarpitch=0.0;
float Tarroll=0.0;
u8 Initflag=0;

int main(void)
{		
	  delay_init();	    	   //延时函数初始化	
   	LED_Init();            //初始化LED
		KEY_Init();            //初始化按键	
	  TIM3_Init(9999,143);   //舵机初始化 周期：20ms
		TIM1_Init(7199,0);     //电机初始化，频率10KHz，总占空比7200
		uart_init(115200);	   //串口初始化为115200
	
	  PID_LSencer_Init();
		
		TIM5_init(9999,71);    //定时器5初始化	10000/(72000000/72)s=10ms
		
		ENC_Init();
	  OLED_Init();           //显示初始化
	  OLED_Fill(0x00);       //清屏
	  UI();                  //显示UI界面
		while(1)
		{
			OLED_ShowNum(24,3,MotorA,1);//显示当前角度
			OLED_ShowNum(81,3,MotorB,1);//显示当前角度
		}
	
		
}

