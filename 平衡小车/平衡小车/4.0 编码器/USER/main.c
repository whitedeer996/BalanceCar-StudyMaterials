/*
��ƽ��С��PD��
���� ����
ʱ�� 2018.5.7
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
	  delay_init();	    	   //��ʱ������ʼ��	
   	LED_Init();            //��ʼ��LED
		KEY_Init();            //��ʼ������	
	  TIM3_Init(9999,143);   //�����ʼ�� ���ڣ�20ms
		TIM1_Init(7199,0);     //�����ʼ����Ƶ��10KHz����ռ�ձ�7200
		uart_init(115200);	   //���ڳ�ʼ��Ϊ115200
	
	  PID_LSencer_Init();
		
		TIM5_init(9999,71);    //��ʱ��5��ʼ��	10000/(72000000/72)s=10ms
		
		ENC_Init();
	  OLED_Init();           //��ʾ��ʼ��
	  OLED_Fill(0x00);       //����
	  UI();                  //��ʾUI����
		while(1)
		{
			OLED_ShowNum(24,3,MotorA,1);//��ʾ��ǰ�Ƕ�
			OLED_ShowNum(81,3,MotorB,1);//��ʾ��ǰ�Ƕ�
		}
	
		
}

