#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
/*
A1  KEYM  A4 KEYR 
A5  KEYD  A6 STARTSTOP 
C12 KEYU  C13 KEYL   
*/


#define KEY   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)//读取按键
#define KEYU  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)//读取按键
#define KEYD  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)//读取按键
#define KEYL  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)//读取按键
#define KEYR  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)//读取按键
#define KEYM  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)//读取按键

void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8);  	//按键扫描函数					    
#endif
