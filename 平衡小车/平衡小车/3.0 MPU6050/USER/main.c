/*
自平衡小车PI环
作者 鹅鹅鹅
时间 2018.5.8
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
	
	  /*MPU6050模块*/	
		MPU_Init();							//初始化MPU6050
		while(mpu_dmp_init())
		{
			delay_ms(200);
			printf("mpu6050 Error\r\n");
		}  
		printf("mpu6050 OK\r\n"); 
		
		TIM5_init(9999,71);    //定时器5初始化	10000/(72000000/72)s=10ms
		
	  OLED_Init();           //显示初始化
	  OLED_Fill(0x00);       //清屏
	  UI();                  //显示UI界面

		while(KEY_Scan(1)!=6)//6为中键
		{
			mpu_dmp_get_data(&Tarpitch,&Tarroll);
		}
		
		/*设定PID参数*/
		//此处我只需要一个PID
		PID_LSencer_SetPID(240,0,1200);
		PID_LSencer_SetPoint(Tarpitch);
		Initflag=1;//前期准备完成
		OLED_ShowNum(78,6,abs((int)Tarpitch),1);//显示传感器目标值
		
   	while(1)
	  {
			//显示PID参数
			OLED_ShowNum(24,4,LSencer.Proportion,1);//显示p参数
			OLED_ShowNum(24,5,LSencer.Integral*10,1);//显示i*10参数（*10用以增加精度）
			OLED_ShowNum(24,6,LSencer.Derivative,1);//显示d参数
			//显示当前传感器参数
			
			OLED_ShowNum(78,5,abs((int)Curpitch),1);//显示传感器当前参数

			if(KEY_Scan(1)==1)
			{
					Initflag=0;//停车
			}
	  }	 
}

