/*
A1  KEYM  A4 KEYR 
A5  KEYD  A6 STARTSTOP 
C12 KEYU  C13 KEYL  
*/
#include "key.h" 
#include "delay.h"
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
		GPIO_InitTypeDef GPIO_InitStructure;
		
		/*A1 A4 A5 A6上拉输入，无输入时高电平*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//使能PORTA和ORTC时钟

		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5;//PA1 PA4 PA5 PA6
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA

		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12|GPIO_Pin_13;//PC12 PC13
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
		GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC

}

//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY按下
//2，KEYU按下
//3，KEYD按下 
//4，KEYL按下
//3，KEYR按下 
//4，KEYM按下
//注意此函数有响应优先级,KEY>KEYU>KEYD>KEYL>KEYR>KEYM!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY==0||KEYU==0||KEYD==0||KEYL==0||KEYR==0||KEYM==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY==0)return 1;
		else if(KEYU==0)return 2;
		else if(KEYD==0)return 3;		
		else if(KEYL==0)return 4;
		else if(KEYR==0)return 5;
		else if(KEYM==0)return 6;
	}
	else if(KEY==1||KEYU==1||KEYD==1||KEYL==1||KEYR==1||KEYM==1)
		key_up=1; 	    
 	return 0;// 无按键按下
}
