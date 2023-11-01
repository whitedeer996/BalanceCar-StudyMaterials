#ifndef __ENCODER_H
#define __ENCODER_H
#include "sys.h"
/*
配置编码器
使用的是定时器4的通道1和通道2
PB6   TIM4_CH1
PB7   TIM4_CH2
由于磁环为26级充磁，电机减速比为1:30，一圈脉冲数为4*13*30=1560
*/
#define MotorA TIM4->CNT   //记录脉冲数
#define MotorB TIM3->CNT   //记录脉冲数


void ENC_Init(void);
#endif
