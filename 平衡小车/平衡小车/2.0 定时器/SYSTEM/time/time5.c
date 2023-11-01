/*
��ʱ��5���ú���
��Ƭ����stm32f103
���ߣ�����
ʱ�䣺2018.3.23
*/

#include "led.h"
#include "time5.h"

/*��ʱ��5��ʼ��*/
void TIM5_init(u16 arr,u16 psc)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);//ʱ��ʹ��

		TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
		TIM_TimeBaseStructure.TIM_Prescaler =psc; //����ʱ�ӷ�Ƶ
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//����ʱ�ӷָ� 
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM���ϼ���ģʽ 
		TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
		TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM�ж� 
		//���ȼ��趨
		NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;//TIM5�ж�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//��ռ���ȼ�1�� 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//�����ȼ�1�� 
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQͨ����ʹ�� 
		NVIC_Init(&NVIC_InitStructure);//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
		TIM_Cmd(TIM5, ENABLE);//ʹ��TIMx���� 
}


/*��ʱ��5�жϺ���*/
void TIM5_IRQHandler(void)              
{
		static int count=0;
		if(TIM5->SR&0X0001)                  //����ж�
		{	
				count++;
				if(count>=100)
				{
						LED=!LED;
						count=0;
				}
     
		}	
		TIM5->SR&=~(1<<0);
}


