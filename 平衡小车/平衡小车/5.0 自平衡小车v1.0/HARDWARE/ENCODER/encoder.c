/*
��ʱ��3�Ͷ�ʱ��4�ı�����ģʽ���ã��Ա�����������м���
���жԶ�ʱ��3ʹ������ȫ��ӳ��
PC6   TIM3_CH1
PC7   TIM3_CH2
�Զ�ʱ��4����ֱ��ʹ����ͨ��1��ͨ��2
PB6   TIM4_CH1
PB7   TIM4_CH2
�ڸ�����������ֻʹ��һ���������ֻʹ�ö�ʱ��4
���ڴŻ�Ϊ26����ţ�������ٱ�Ϊ1:30��һȦ������Ϊ4*13*30=1560
*/
#include "encoder.h"

/*��������ʼ��*/
void ENC_Init(void)
{
		/*����ṹ��*/
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_ICInitTypeDef TIM_ICInitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
			
		/*�򿪸��ù��ܣ���ʱ��3������ȫ��ӳ�䣬ʹ�ܶ�ʱ��3ʱ�Ӻ�PC�˿�ʱ��*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		
		/*ʹ�ܶ�ʱ��4ʱ�Ӻ�PB�˿�ʱ��*/
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

		/*�˿�����*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_Init(GPIOC, &GPIO_InitStructure);
			
		/*��ʱ��3��4�ж����ȼ����ã��˴�����Ҫ�ж�*/
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
		NVIC_Init(&NVIC_InitStructure);

		/*���ö�ʱ��3��4������Ԫ*/
		TIM_DeInit(TIM3);
		TIM_DeInit(TIM4);
		TIM_TimeBaseStructure.TIM_Period = 0XFFFF; 		
		TIM_TimeBaseStructure.TIM_Prescaler = 0; 
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

		/*���벶������*/
		//�˴�����С���⵱ֻ��TIM_ICPolarity_Falling�½��ؼ���TIM_ICPolarity_BothEdge˫���ؼ��û������
		//ͨ���鿴Ӳ���ֲ���stm32�̼���ʹ���ֲᣬ���ָ��ֲ��в�����TIM_ICPolarity_BothEdge˫���ش���
		//ֻ����TIM_ICPolarity_Rising�����غ�TIM_ICPolarity_Falling�½��ش����������Ǹù̼��������bug
		TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
		TIM_ICStructInit(&TIM_ICInitStructure);
		TIM_ICInitStructure.TIM_ICFilter = 6;
		TIM_ICInit(TIM3, &TIM_ICInitStructure);
		TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
		TIM_ICStructInit(&TIM_ICInitStructure);
		TIM_ICInitStructure.TIM_ICFilter = 6;
		TIM_ICInit(TIM4, &TIM_ICInitStructure);

		/*�������жϣ��Լ���Ӧ�ļ���������ֵ����������ʱ��*/
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
		TIM_ClearFlag(TIM4, TIM_FLAG_Update);
		TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
		TIM3->CNT =10000;
		TIM4->CNT =10000;
		TIM_Cmd(TIM3, ENABLE); 
		TIM_Cmd(TIM4, ENABLE); 
}
