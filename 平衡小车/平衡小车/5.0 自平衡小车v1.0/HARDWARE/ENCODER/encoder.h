#ifndef __ENCODER_H
#define __ENCODER_H
#include "sys.h"
/*
���ñ�����
ʹ�õ��Ƕ�ʱ��4��ͨ��1��ͨ��2
PB6   TIM4_CH1
PB7   TIM4_CH2
���ڴŻ�Ϊ26����ţ�������ٱ�Ϊ1:30��һȦ������Ϊ4*13*30=1560
*/
#define MotorA TIM4->CNT   //��¼������
#define MotorB TIM3->CNT   //��¼������


void ENC_Init(void);
#endif
