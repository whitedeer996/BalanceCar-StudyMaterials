/*
该函数是电机的函数库
单片机：stm32f103
作者：鹅鹅鹅
时间：2018.3.21
*/
#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

/*
定时器1PWM输出配置
PA11     TIM1_4
PA8      TIM1_1
*/
#define MOTOR_EN PAout(3)//TB6612使能

/*定时器1初始化*/
//定时器1用于输出PWM以控制电机
//此处输出的是定时器1通道1和通道4
void TIM1_Init(u16 arr,u16 psc);
/*TB6612使能*/
void Motor_En(void);
/*TB6612失能*/
void Motor_Stop(void);
/*右电机正转*/
void RMotor_TP(u16 duty);
/*右电机反转*/
void RMotor_TN(u16 duty);
/*左电机正转*/
void LMotor_TP(u16 duty);
/*左电机正转*/
void LMotor_TN(u16 duty);
/*右电机控制*/
void RMotor_Ctr(s16 duty);
/*左电机控制*/
void LMotor_Ctr(s16 duty);

#endif

