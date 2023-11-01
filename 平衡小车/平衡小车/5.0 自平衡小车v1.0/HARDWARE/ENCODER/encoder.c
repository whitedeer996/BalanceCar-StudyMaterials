/*
定时器3和定时器4的编码器模式配置，对编码器脉冲进行计数
其中对定时器3使用了完全重映像
PC6   TIM3_CH1
PC7   TIM3_CH2
对定时器4则是直接使用其通道1和通道2
PB6   TIM4_CH1
PB7   TIM4_CH2
在该例程中我们只使用一个电机，故只使用定时器4
由于磁环为26级充磁，电机减速比为1:30，一圈脉冲数为4*13*30=1560
*/
#include "encoder.h"

/*编码器初始化*/
void ENC_Init(void)
{
		/*定义结构体*/
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_ICInitTypeDef TIM_ICInitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
			
		/*打开复用功能，定时器3引脚完全重映射，使能定时器3时钟和PC端口时钟*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		
		/*使能定时器4时钟和PB端口时钟*/
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

		/*端口配置*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_Init(GPIOC, &GPIO_InitStructure);
			
		/*定时器3、4中断优先级配置，此处不需要中断*/
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
		NVIC_Init(&NVIC_InitStructure);

		/*配置定时器3、4基本单元*/
		TIM_DeInit(TIM3);
		TIM_DeInit(TIM4);
		TIM_TimeBaseStructure.TIM_Period = 0XFFFF; 		
		TIM_TimeBaseStructure.TIM_Prescaler = 0; 
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

		/*输入捕获配置*/
		//此处存在小问题当只是TIM_ICPolarity_Falling下降沿检测和TIM_ICPolarity_BothEdge双边沿检测没有区别
		//通过查看硬件手册中stm32固件库使用手册，发现该手册中不存在TIM_ICPolarity_BothEdge双边沿触发
		//只存在TIM_ICPolarity_Rising上升沿和TIM_ICPolarity_Falling下降沿触发，估计是该固件存在这个bug
		TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
		TIM_ICStructInit(&TIM_ICInitStructure);
		TIM_ICInitStructure.TIM_ICFilter = 6;
		TIM_ICInit(TIM3, &TIM_ICInitStructure);
		TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
		TIM_ICStructInit(&TIM_ICInitStructure);
		TIM_ICInitStructure.TIM_ICFilter = 6;
		TIM_ICInit(TIM4, &TIM_ICInitStructure);

		/*清除相关中断，以及对应的计数器赋初值，并启动定时器*/
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
		TIM_ClearFlag(TIM4, TIM_FLAG_Update);
		TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
		TIM3->CNT =10000;
		TIM4->CNT =10000;
		TIM_Cmd(TIM3, ENABLE); 
		TIM_Cmd(TIM4, ENABLE); 
}
