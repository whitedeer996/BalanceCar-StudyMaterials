#include "led.h"

void LED_Init(void)
{
RCC->APB2ENR|=1<<2; //使能 PORTA 时钟  
GPIOA->CRH&=0XFFF0FFFF;
GPIOA->CRH|=0X00030000;//A12 推挽输出
GPIOA->ODR|=1<<8; //A12 输出高
}

/**************************************************************************
函数功能：LED闪烁
入口参数：闪烁频率 
返回  值：无
**************************************************************************/
void Led_Flash(u16 time)
{
	  static int temp;
		if(++temp==time)	LED=~LED,temp=0;
}
