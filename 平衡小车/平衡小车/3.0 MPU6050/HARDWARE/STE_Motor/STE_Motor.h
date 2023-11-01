/*
该函数是舵机函数库
单片机：stm32f103
作者：鹅鹅鹅
时间：2018.3.14
*/

#ifndef STE_Motor_H
#define STE_Motor_H

#include "sys.h"

/*舵机初始化*/
void TIM3_Init(u16 arr,u16 psc);

#endif
