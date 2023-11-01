#include "led.h"
#include "delay.h"
#include "OLED.h"
#include "key.h" 
#include "usart.h"
#include "Motor.h"
#include "LDC1314.h"
#include "LDC1314_cmd.h"
#include "STE_Motor.h"

#define LED_ON GPIO_ResetBits(GPIOD,GPIO_Pin_2) //亮灯
#define LED_OFF GPIO_SetBits(GPIOD,GPIO_Pin_2)  //灭灯
#define T 1000                                  //定时 单位ms

int main(void)
{		
		u8 i;								   //计数，用于循环
		u8 len;	               //记录字符串长度
	  delay_init();	    	   //延时函数初始化	
   	LED_Init();            //初始化LED
		KEY_Init();            //初始化按键	
	  TIM3_Init(9999,143);   //舵机初始化 周期：20ms
		TIM1_Init(7199,0);     //频率10KHz，总占空比7200
		uart_init(115200);	   //串口初始化为115200
	                         
	  OLED_Init();           //显示初始化
	  OLED_Fill(0x00);       //清屏
	  UI();                  //显示UI界面

		RMotor_Ctr(2000);
		LMotor_Ctr(3000);
   	while(1)
	  {
			
//					/*舵机调整范围需要微调*/
//					TIM_SetCompare4(TIM3,666);右
//					delay_ms(5000);
//					TIM_SetCompare4(TIM3,540);中
//					delay_ms(5000);
//					TIM_SetCompare4(TIM3,400);左
//					delay_ms(5000);
			
					/*串口测试程序*/
					/*串口接收中断程序已略作修改，用于在OLED上显示*/
					if(USART_RX_STA&0x8000)                 //接收标志（最高位为1，接收成功，低7位为接收数据量）
					{
								len=USART_RX_STA&0x3fff;          //得到此次接收到的数据长度
								//printf("您发送的消息为:\r\n");
								for(i=0;i<len;i++)                //发送接收到的字符串
								{
									USART1->DR=USART_RX_BUF[i];
									while((USART1->SR&0X40)==0);    //等待发送结束
								}
								OLED_ShowStr(79,6,"        ",1);  //清除OLED原先显示的字符
								OLED_ShowStr(79,6,USART_RX_BUF,1);//显示串口接收到的字符串，最多8个字符
								printf("\r\n");                   //插入换行
								USART_RX_STA=0;                   //清除此次接收的标志
				}
	  }	 
}

