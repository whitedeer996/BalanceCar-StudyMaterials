#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED PDout(2)
#define LED_ON GPIO_ResetBits(GPIOD,GPIO_Pin_2) //����
#define LED_OFF GPIO_SetBits(GPIOD,GPIO_Pin_2)  //���

void LED_Init(void);//��ʼ��
	 				    
#endif
