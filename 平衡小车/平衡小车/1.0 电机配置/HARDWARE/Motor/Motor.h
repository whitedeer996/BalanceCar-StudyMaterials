/*
�ú����ǵ���ĺ�����
��Ƭ����stm32f103
���ߣ�����
ʱ�䣺2018.3.21
*/
#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

/*
��ʱ��1PWM�������
PA11     TIM1_4
PA8      TIM1_1
*/
#define MOTOR_EN PAout(3)//TB6612ʹ��

/*��ʱ��1��ʼ��*/
//��ʱ��1�������PWM�Կ��Ƶ��
//�˴�������Ƕ�ʱ��1ͨ��1��ͨ��4
void TIM1_Init(u16 arr,u16 psc);
/*TB6612ʹ��*/
void Motor_En(void);
/*TB6612ʧ��*/
void Motor_Stop(void);
/*�ҵ����ת*/
void RMotor_TP(u16 duty);
/*�ҵ����ת*/
void RMotor_TN(u16 duty);
/*������ת*/
void LMotor_TP(u16 duty);
/*������ת*/
void LMotor_TN(u16 duty);
/*�ҵ������*/
void RMotor_Ctr(s16 duty);
/*��������*/
void LMotor_Ctr(s16 duty);

#endif

