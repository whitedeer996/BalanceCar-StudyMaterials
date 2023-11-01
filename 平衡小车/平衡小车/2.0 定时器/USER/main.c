#include "led.h"
#include "delay.h"
#include "OLED.h"
#include "key.h" 
#include "usart.h"
#include "Motor.h"
#include "LDC1314.h"
#include "LDC1314_cmd.h"
#include "STE_Motor.h"
#include "PID.h"
#include "time5.h"

int main(void)
{		
	  delay_init();	    	   //��ʱ������ʼ��	
   	LED_Init();            //��ʼ��LED
		KEY_Init();            //��ʼ������	
	  TIM3_Init(9999,143);   //�����ʼ�� ���ڣ�20ms
		TIM1_Init(7199,0);     //�����ʼ����Ƶ��10KHz����ռ�ձ�7200
		uart_init(115200);	   //���ڳ�ʼ��Ϊ115200
	
		/*��ʱ��5��ʼ���ڴ˺������ǲ��������⣬������Ҫ�ڶ�ʱ���ж�ȡLDC1314�Ĳ����������������������ͻ*/
		TIM5_init(9999,71);    //��ʱ��5��ʼ��	10000/(72000000/72)s=10ms

		while(!LDC1314_Init());//LCD1314��ʼ��
	                       
	  OLED_Init();           //��ʾ��ʼ��
	  OLED_Fill(0x00);       //����
	  UI();                  //��ʾUI����

//		RMotor_Ctr(2000);
//		LMotor_Ctr(3000);
   	while(1)
	  {
//				/*���������Χ��Ҫ΢��*/
//				TIM_SetCompare4(TIM3,666);//��
//				delay_ms(5000);
//				TIM_SetCompare4(TIM3,540);//��
//				delay_ms(5000);
//				TIM_SetCompare4(TIM3,400);//��
//				delay_ms(5000);
				//��ȡDATA�Ĵ���
				LDC1314_ProcessDRDY();
			  delay_ms(1000);
				USART1->DR=0xAA;
				while((USART1->SR&0X40)==0);    //�ȴ����ͽ���
				USART1->DR=0xAA;
				while((USART1->SR&0X40)==0);    //�ȴ����ͽ���
				USART1->DR=0xAA;
				while((USART1->SR&0X40)==0);    //�ȴ����ͽ���
				USART1->DR=0xAA;
				while((USART1->SR&0X40)==0);    //�ȴ����ͽ���
//				USART1->DR=(unsigned char)(allData[0] >> 8);
//				while((USART1->SR&0X40)==0);    //�ȴ����ͽ���
				USART1->DR=(unsigned char)(allData[0]);//��
				while((USART1->SR&0X40)==0);    //�ȴ����ͽ���
//				USART1->DR=(unsigned char)(allData[4] >> 8);
//				while((USART1->SR&0X40)==0);    //�ȴ����ͽ���
				USART1->DR=(unsigned char)(allData[4]);//��
				while((USART1->SR&0X40)==0);    //�ȴ����ͽ���
//				USART1->DR=(unsigned char)(allData[6] >> 8);
//				while((USART1->SR&0X40)==0);    //�ȴ����ͽ���
				USART1->DR=(unsigned char)(allData[6]);//��
				while((USART1->SR&0X40)==0);    //�ȴ����ͽ���

	  }	 
}

