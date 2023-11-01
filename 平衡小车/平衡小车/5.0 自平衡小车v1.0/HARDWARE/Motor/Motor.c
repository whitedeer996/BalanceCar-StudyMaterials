/*
该函数是电机的函数库
单片机：stm32f103
作者：鹅鹅鹅
时间：2018.3.21
*/
#include "Motor.h"

/*定时器1初始化*/
//定时器1用于输出PWM以控制电机
//此处输出的是定时器1通道1和通道4
void TIM1_Init(u16 arr,u16 psc)
{  
		/*定义结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		
	  /*使能TIM1时钟和GPIOA端口时钟*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);	//使能定时器1时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE); //使能GPIOA/C时钟
		
	  /*配置引脚*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_8; //TIM1_CH4&CH1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO口速度为50MHz
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
		GPIO_ResetBits(GPIOA,GPIO_Pin_11|GPIO_Pin_8);//清0
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				       //端口配置
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		   //推挽输出
		GPIO_Init(GPIOA, &GPIO_InitStructure);					         //根据设定参数初始化GPIOD
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);						           //PA3清0
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;	 //端口配置
		GPIO_Init(GPIOC, &GPIO_InitStructure);					         //根据设定参数初始化GPIOD
		GPIO_ResetBits(GPIOC,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);					 //PC10，PC11清0
	 
		/*初始化TIM1*/
		TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
		TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
		
	  /*输出端口配置*/
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
		TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC4
		TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 OC1
		
		/*输出使能*/
		TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR2上的预装载寄存器
		TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR2上的预装载寄存器
		TIM_Cmd(TIM1, ENABLE);  //使能TIM1
		TIM_CtrlPWMOutputs(TIM1, ENABLE);//主输出使能，高级定时器才有的，输出pwm必须打开
}

/*TB6612使能*/
void Motor_En(void)
{
		MOTOR_EN=1;
}

/*TB6612失能*/
void Motor_Stop(void)
{
		MOTOR_EN=0;
}

/*右电机正转*/
void RMotor_TP(u16 duty)
{
		PCout(10)=1;
		PCout(11)=0;
		TIM_SetCompare4(TIM1,duty);//定时器1通道4输出duty/1000占空比
}

/*右电机反转*/
void RMotor_TN(u16 duty)
{
		PCout(10)=0;
		PCout(11)=1;
		TIM_SetCompare4(TIM1,duty);//定时器1通道4输出duty/1000占空比
}

/*左电机正转*/
void LMotor_TP(u16 duty)
{
		PCout(8)=1;
		PCout(9)=0;
		TIM_SetCompare1(TIM1,duty);//定时器1通道1输出duty/1000占空比
}

/*左电机正转*/
void LMotor_TN(u16 duty)
{
		PCout(8)=0;
		PCout(9)=1;
		TIM_SetCompare1(TIM1,duty);//定时器1通道1输出duty/1000占空比
}

/*右电机控制*/
void RMotor_Ctr(s16 duty)
{
		MOTOR_EN=1;
		if(duty>=0)
			RMotor_TP(duty);
		else
			RMotor_TN(-duty);
}

/*左电机控制*/
void LMotor_Ctr(s16 duty)
{
		MOTOR_EN=1;
		if(duty>=0)
			LMotor_TP(duty);
		else
			LMotor_TN(-duty);
}
