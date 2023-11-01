/*
�ú����Ƕ��������
��Ƭ����stm32f103
���ߣ�����
ʱ�䣺2018.3.14
*/

#include "STE_Motor.h"
#include "delay.h"

/*�����ʼ��*/
/*��ʱ��3��ʼ��*/
//��ʱ��3�������PWM�Կ��ƶ��
//�˴�������Ƕ�ʱ��3ͨ��4
void TIM3_Init(u16 arr,u16 psc)
{  
		/*����ṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		
	  /*ʹ��TIM3ʱ�Ӻ�GPIOB�˿�ʱ��*/
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //ʹ��GPIOBʱ��
		
	  /*��������*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //TIM3_CH4
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO���ٶ�Ϊ50MHz
		GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);//��0
	 
		/*��ʼ��TIM1*/
		TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
		TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
		
	  /*����˿�����*/
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
		TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC4
		
		/*���ʹ��*/
		TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR�ϵ�Ԥװ�ؼĴ���
		TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3

}

