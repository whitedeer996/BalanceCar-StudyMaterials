#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
#define PI 3.14159265
int TIM1_UP_IRQHandler(void);  
void Set_Pwm(int moto1);
void Xianfu_Pwm(void);
int myabs(int a);
int Incremental_PI (int Encoder,int Target);
#endif
