/*
定时器5配置函数
单片机：stm32f103
作者：鹅鹅鹅
时间：2018.3.23
*/

#include "led.h"
#include "time5.h"

/*定时器5初始化*/
void TIM5_init(u16 arr,u16 psc)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);//时钟使能

		TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
		TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置时钟分频
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//设置时钟分割 
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数模式 
		TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
		TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能指定的TIM中断 
		//优先级设定
		NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;//TIM5中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//先占优先级1级 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//从优先级1级 
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQ通道被使能 
		NVIC_Init(&NVIC_InitStructure);//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
		TIM_Cmd(TIM5, ENABLE);//使能TIMx外设 
}


/*定时器5中断函数*/
void TIM5_IRQHandler(void)              
{
		static int count=0;
		if(TIM5->SR&0X0001)                  //溢出中断
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


