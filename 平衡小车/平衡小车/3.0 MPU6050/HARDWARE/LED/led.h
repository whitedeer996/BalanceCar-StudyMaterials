#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED PDout(2)
#define LED_ON GPIO_ResetBits(GPIOD,GPIO_Pin_2) //ÁÁµÆ
#define LED_OFF GPIO_SetBits(GPIOD,GPIO_Pin_2)  //ÃðµÆ

void LED_Init(void);//³õÊ¼»¯
	 				    
#endif
