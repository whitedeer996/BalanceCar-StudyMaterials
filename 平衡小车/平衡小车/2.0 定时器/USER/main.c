#include "led.h"
#include "delay.h"
#include "OLED.h"
#include "key.h" 
#include "usart.h"
#include "Motor.h"
#include "LDC1314.h"
#include "LDC1314_cmd.h"
#include "STE_Motor.h"
#include "PID.h"
#include "time5.h"

int main(void)
{		
	  delay_init();	    	   //延时函数初始化	
   	LED_Init();            //初始化LED
		KEY_Init();            //初始化按键	
	  TIM3_Init(9999,143);   //舵机初始化 周期：20ms
		TIM1_Init(7199,0);     //电机初始化，频率10KHz，总占空比7200
		uart_init(115200);	   //串口初始化为115200
	
		/*定时器5初始化在此函数中是不存在问题，但如若要在定时器中读取LDC1314的参数，则会与主函数发生冲突*/
		TIM5_init(9999,71);    //定时器5初始化	10000/(72000000/72)s=10ms

		while(!LDC1314_Init());//LCD1314初始化
	                       
	  OLED_Init();           //显示初始化
	  OLED_Fill(0x00);       //清屏
	  UI();                  //显示UI界面

//		RMotor_Ctr(2000);
//		LMotor_Ctr(3000);
   	while(1)
	  {
//				/*舵机调整范围需要微调*/
//				TIM_SetCompare4(TIM3,666);//右
//				delay_ms(5000);
//				TIM_SetCompare4(TIM3,540);//中
//				delay_ms(5000);
//				TIM_SetCompare4(TIM3,400);//左
//				delay_ms(5000);
				//读取DATA寄存器
				LDC1314_ProcessDRDY();
			  delay_ms(1000);
				USART1->DR=0xAA;
				while((USART1->SR&0X40)==0);    //等待发送结束
				USART1->DR=0xAA;
				while((USART1->SR&0X40)==0);    //等待发送结束
				USART1->DR=0xAA;
				while((USART1->SR&0X40)==0);    //等待发送结束
				USART1->DR=0xAA;
				while((USART1->SR&0X40)==0);    //等待发送结束
//				USART1->DR=(unsigned char)(allData[0] >> 8);
//				while((USART1->SR&0X40)==0);    //等待发送结束
				USART1->DR=(unsigned char)(allData[0]);//左
				while((USART1->SR&0X40)==0);    //等待发送结束
//				USART1->DR=(unsigned char)(allData[4] >> 8);
//				while((USART1->SR&0X40)==0);    //等待发送结束
				USART1->DR=(unsigned char)(allData[4]);//中
				while((USART1->SR&0X40)==0);    //等待发送结束
//				USART1->DR=(unsigned char)(allData[6] >> 8);
//				while((USART1->SR&0X40)==0);    //等待发送结束
				USART1->DR=(unsigned char)(allData[6]);//右
				while((USART1->SR&0X40)==0);    //等待发送结束

	  }	 
}

