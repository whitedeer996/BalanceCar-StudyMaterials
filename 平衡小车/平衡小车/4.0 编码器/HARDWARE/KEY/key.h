#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
/*
A1  KEYM  A4 KEYR 
A5  KEYD  A6 STARTSTOP 
C12 KEYU  C13 KEYL   
*/


#define KEY   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)//��ȡ����
#define KEYU  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)//��ȡ����
#define KEYD  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)//��ȡ����
#define KEYL  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)//��ȡ����
#define KEYR  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)//��ȡ����
#define KEYM  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)//��ȡ����

void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8);  	//����ɨ�躯��					    
#endif
