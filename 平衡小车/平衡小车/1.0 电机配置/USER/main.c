#include "led.h"
#include "delay.h"
#include "OLED.h"
#include "key.h" 
#include "usart.h"
#include "Motor.h"
#include "LDC1314.h"
#include "LDC1314_cmd.h"
#include "STE_Motor.h"

#define LED_ON GPIO_ResetBits(GPIOD,GPIO_Pin_2) //����
#define LED_OFF GPIO_SetBits(GPIOD,GPIO_Pin_2)  //���
#define T 1000                                  //��ʱ ��λms

int main(void)
{		
		u8 i;								   //����������ѭ��
		u8 len;	               //��¼�ַ�������
	  delay_init();	    	   //��ʱ������ʼ��	
   	LED_Init();            //��ʼ��LED
		KEY_Init();            //��ʼ������	
	  TIM3_Init(9999,143);   //�����ʼ�� ���ڣ�20ms
		TIM1_Init(7199,0);     //Ƶ��10KHz����ռ�ձ�7200
		uart_init(115200);	   //���ڳ�ʼ��Ϊ115200
	                         
	  OLED_Init();           //��ʾ��ʼ��
	  OLED_Fill(0x00);       //����
	  UI();                  //��ʾUI����

		RMotor_Ctr(2000);
		LMotor_Ctr(3000);
   	while(1)
	  {
			
//					/*���������Χ��Ҫ΢��*/
//					TIM_SetCompare4(TIM3,666);��
//					delay_ms(5000);
//					TIM_SetCompare4(TIM3,540);��
//					delay_ms(5000);
//					TIM_SetCompare4(TIM3,400);��
//					delay_ms(5000);
			
					/*���ڲ��Գ���*/
					/*���ڽ����жϳ����������޸ģ�������OLED����ʾ*/
					if(USART_RX_STA&0x8000)                 //���ձ�־�����λΪ1�����ճɹ�����7λΪ������������
					{
								len=USART_RX_STA&0x3fff;          //�õ��˴ν��յ������ݳ���
								//printf("�����͵���ϢΪ:\r\n");
								for(i=0;i<len;i++)                //���ͽ��յ����ַ���
								{
									USART1->DR=USART_RX_BUF[i];
									while((USART1->SR&0X40)==0);    //�ȴ����ͽ���
								}
								OLED_ShowStr(79,6,"        ",1);  //���OLEDԭ����ʾ���ַ�
								OLED_ShowStr(79,6,USART_RX_BUF,1);//��ʾ���ڽ��յ����ַ��������8���ַ�
								printf("\r\n");                   //���뻻��
								USART_RX_STA=0;                   //����˴ν��յı�־
				}
	  }	 
}

