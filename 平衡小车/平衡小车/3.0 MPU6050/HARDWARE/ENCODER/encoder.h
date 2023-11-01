#ifndef __ENCODER_H
#define __ENCODER_H
#include "sys.h"
#define MotorA TIM3->CNT   //读取计数值，等价于位置
#define MotorB TIM4->CNT   //读取计数值，等价于位置

void ENC_Init(void);
#endif
