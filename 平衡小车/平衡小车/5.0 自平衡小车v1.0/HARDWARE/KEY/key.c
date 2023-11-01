/*
A1  KEYM  A4 KEYR 
A5  KEYD  A6 STARTSTOP 
C12 KEYU  C13 KEYL  
*/
#include "key.h" 
#include "delay.h"
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
		GPIO_InitTypeDef GPIO_InitStructure;
		
		/*A1 A4 A5 A6�������룬������ʱ�ߵ�ƽ*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTA��ORTCʱ��

		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5;//PA1 PA4 PA5 PA6
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
		GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA

		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12|GPIO_Pin_13;//PC12 PC13
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
		GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC

}

//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY����
//2��KEYU����
//3��KEYD���� 
//4��KEYL����
//3��KEYR���� 
//4��KEYM����
//ע��˺�������Ӧ���ȼ�,KEY>KEYU>KEYD>KEYL>KEYR>KEYM!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY==0||KEYU==0||KEYD==0||KEYL==0||KEYR==0||KEYM==0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY==0)return 1;
		else if(KEYU==0)return 2;
		else if(KEYD==0)return 3;		
		else if(KEYL==0)return 4;
		else if(KEYR==0)return 5;
		else if(KEYM==0)return 6;
	}
	else if(KEY==1||KEYU==1||KEYD==1||KEYL==1||KEYR==1||KEYM==1)
		key_up=1; 	    
 	return 0;// �ް�������
}
