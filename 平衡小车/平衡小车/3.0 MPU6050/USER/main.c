/*
��ƽ��С��PI��
���� ����
ʱ�� 2018.5.8
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
	  delay_init();	    	   //��ʱ������ʼ��	
   	LED_Init();            //��ʼ��LED
		KEY_Init();            //��ʼ������	
	  TIM3_Init(9999,143);   //�����ʼ�� ���ڣ�20ms
		TIM1_Init(7199,0);     //�����ʼ����Ƶ��10KHz����ռ�ձ�7200
		uart_init(115200);	   //���ڳ�ʼ��Ϊ115200
	
	  PID_LSencer_Init();
	
	  /*MPU6050ģ��*/	
		MPU_Init();							//��ʼ��MPU6050
		while(mpu_dmp_init())
		{
			delay_ms(200);
			printf("mpu6050 Error\r\n");
		}  
		printf("mpu6050 OK\r\n"); 
		
		TIM5_init(9999,71);    //��ʱ��5��ʼ��	10000/(72000000/72)s=10ms
		
	  OLED_Init();           //��ʾ��ʼ��
	  OLED_Fill(0x00);       //����
	  UI();                  //��ʾUI����

		while(KEY_Scan(1)!=6)//6Ϊ�м�
		{
			mpu_dmp_get_data(&Tarpitch,&Tarroll);
		}
		
		/*�趨PID����*/
		//�˴���ֻ��Ҫһ��PID
		PID_LSencer_SetPID(240,0,1200);
		PID_LSencer_SetPoint(Tarpitch);
		Initflag=1;//ǰ��׼�����
		OLED_ShowNum(78,6,abs((int)Tarpitch),1);//��ʾ������Ŀ��ֵ
		
   	while(1)
	  {
			//��ʾPID����
			OLED_ShowNum(24,4,LSencer.Proportion,1);//��ʾp����
			OLED_ShowNum(24,5,LSencer.Integral*10,1);//��ʾi*10������*10�������Ӿ��ȣ�
			OLED_ShowNum(24,6,LSencer.Derivative,1);//��ʾd����
			//��ʾ��ǰ����������
			
			OLED_ShowNum(78,5,abs((int)Curpitch),1);//��ʾ��������ǰ����

			if(KEY_Scan(1)==1)
			{
					Initflag=0;//ͣ��
			}
	  }	 
}

