#include "led.h"

/*端口初始化*/
void LED_Init(void)
{
 
		 GPIO_InitTypeDef  GPIO_InitStructure;
			
		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //使能PD端口时钟
			
		 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 //LED0-->PD.2 端口配置
		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
		 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
		 GPIO_Init(GPIOD, &GPIO_InitStructure);					 //根据设定参数初始化GPIOD
		 GPIO_SetBits(GPIOD,GPIO_Pin_2);						 //PD2 输出
}
 
