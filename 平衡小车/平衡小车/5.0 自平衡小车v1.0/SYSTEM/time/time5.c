/*
定时器5配置函数
单片机：stm32f103
作者：鹅鹅鹅
时间：2018.3.23
*/

#include "led.h"
#include "time5.h"
#include "inv_mpu.h"
#include "McuDataSend.h"
#include "PID.h"
#include "Motor.h"
#include "encoder.h"

float Curpitch = 0.0;
float Curroll = 0.0;
extern u8 Initflag;

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
	  static float PD_value=0,PI_value=0;
		static u8 tim_count=0;
		static s16 LPwmOut,RPwmOut;
		if(TIM5->SR&0X0001)                  //溢出中断
		{	
				if(Initflag)
				{
					if(mpu_dmp_get_data(&Curpitch,&Curroll)==0)
					{
						tim_count++;
						LED	= !LED;
						//Data_Send_Status(Curroll,Curpitch,0);//发送pitch roll 另一个yaw可用来发送设定值，以此调节pid
						if(abs(Curpitch) > 45)//快倒下时停止运行
						{
							LPwmOut	= RPwmOut = 0;
						}
						else
						{
							PD_value=PID_LSencer_Calculate(Curpitch);
							if(tim_count>=5)//5次中断调节一次PI环
							{
								tim_count=0;
								PI_value=velocity(MotorA-10000,MotorB-10000);
								MotorA=10000;
								MotorB=10000;
							}
							LPwmOut=PD_value+PI_value;
							RPwmOut=PD_value+PI_value;	
							if(LPwmOut>7100) LPwmOut=7100;
							if(LPwmOut<-7100) LPwmOut=-7100;
							if(RPwmOut>7100) RPwmOut=7100;
							if(RPwmOut<-7100) RPwmOut=-7100;
					  }
					}	
				
					LMotor_Ctr(LPwmOut);
					RMotor_Ctr(RPwmOut);
				}
				else Motor_Stop();
		}	
		TIM5->SR&=~(1<<0);
}


