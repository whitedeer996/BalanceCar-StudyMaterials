#include "sys.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
u8 Flag_Stop=1;                  //停止标志位
int Encoder,Position=10000;      //编码器的脉冲计数
long SHOW;
int Moto1;                 //电机PWM变量 应是Motor的 向Moto致敬	

int main(void)
{ 
	Stm32_Clock_Init(9);            //系统时钟设置
	delay_init(72);                 //延时初始化
	LED_Init();                     //初始化与 LED 连接的硬件接口
	uart_init(72,115200);           //初始化串口1
	MiniBalance_PWM_Init(7199,0);   //=====初始化PWM 10KHZ 高频可以防止电机低频时的尖叫声
	Encoder_Init_TIM2();            //初始化编码器
  Timer3_Init(99,7199);           //=====10MS进一次中断服务函数，中断服务函数在control.c
	while(1)
		{
			printf("Encoder:%d Position:%d \r\n",Encoder,Position);	
		} 
}
