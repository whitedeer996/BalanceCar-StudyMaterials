/*
该函数是舵机函数库
单片机：stm32f103
作者：鹅鹅鹅
时间：2018.3.14
*/

#include "STE_Motor.h"
#include "delay.h"

/*舵机初始化*/
/*定时器3初始化*/
//定时器3用于输出PWM以控制舵机
//此处输出的是定时器3通道4
void TIM3_Init(u16 arr,u16 psc)
{  
		/*定义结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		
	  /*使能TIM3时钟和GPIOB端口时钟*/
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);	//使能定时器3时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //使能GPIOB时钟
		
	  /*配置引脚*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //TIM3_CH4
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO口速度为50MHz
		GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);//清0
	 
		/*初始化TIM1*/
		TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
		TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
		
	  /*输出端口配置*/
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
		TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC4
		
		/*输出使能*/
		TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR上的预装载寄存器
		TIM_Cmd(TIM3, ENABLE);  //使能TIM3

}

