#ifndef __ENCODER_H
#define __ENCODER_H
#include "sys.h"
#define MotorA TIM3->CNT   //��ȡ����ֵ���ȼ���λ��
#define MotorB TIM4->CNT   //��ȡ����ֵ���ȼ���λ��

void ENC_Init(void);
#endif
