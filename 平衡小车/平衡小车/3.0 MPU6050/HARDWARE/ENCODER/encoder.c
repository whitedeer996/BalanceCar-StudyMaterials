/******************************************
定时器3的编码器模式配置，可以直接对两个通道口的输入进行计数，得到编码值
为TIM3->CNT
编码器数值：磁环为26级充磁，电机减速比为1:30
            一圈数值为2*26*30=1560
*******************************************/
#include "encoder.h"

//stm32中的编码器模式，需要用到两个定时器通道，此处用的定时器3
void ENC_Init(void)
{
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_ICInitTypeDef TIM_ICInitStructure;
	
/* Encoder unit connected to TIM3, 4X mode */    
GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);

/* TIM3 clock source enable */
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
/* Enable GPIOA, clock */
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB, ENABLE);

//GPIO_StructInit(&GPIO_InitStructure);
/*Configure PC.06,07 as encoder input*/
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
GPIO_Init(GPIOC, &GPIO_InitStructure);
	
/*Configure PB.06,07 as encoder input*/
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
GPIO_Init(GPIOB, &GPIO_InitStructure);

/* Enable the TIM3 Update Interrupt */
NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);

/* Enable the TIM4 Update Interrupt */
NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);

/* Timer configuration in Encoder mode */
TIM_DeInit(TIM3);TIM_DeInit(TIM4);
//TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

TIM_TimeBaseStructure.TIM_Prescaler = 0; // No prescaling 
TIM_TimeBaseStructure.TIM_Period = 0XFFFF; 
TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;   
TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

TIM_TimeBaseStructure.TIM_Prescaler = 0; // No prescaling 
TIM_TimeBaseStructure.TIM_Period = 0XFFFF; 
TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;   
TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, 
                           TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
TIM_ICStructInit(&TIM_ICInitStructure);
TIM_ICInitStructure.TIM_ICFilter = 6;
TIM_ICInit(TIM3, &TIM_ICInitStructure);

TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, 
                           TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
TIM_ICStructInit(&TIM_ICInitStructure);
TIM_ICInitStructure.TIM_ICFilter = 6;
TIM_ICInit(TIM4, &TIM_ICInitStructure);

// Clear all pending interrupts
TIM_ClearFlag(TIM3, TIM_FLAG_Update);
TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

TIM_ClearFlag(TIM4, TIM_FLAG_Update);
TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
//Reset counter
TIM3->CNT =10000;
TIM4->CNT =10000;

// ENC_Clear_Speed_Buffer();
TIM_Cmd(TIM3, ENABLE); 
TIM_Cmd(TIM4, ENABLE); 


}
