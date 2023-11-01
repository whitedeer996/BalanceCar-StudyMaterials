/*
�ó�����PID���ڵĺ�����
��Ƭ����stm32f103
���ߣ�����
ʱ�䣺2018.3.22
*/
#ifndef PID_H
#define PID_H	 
#include "sys.h"

/*PID�ṹ�����*/
typedef struct
{
	float  SetPoint; 	//�趨Ŀ��
	double  SumError;	//����ۼ� 
		
	float  Proportion;  //�������� 
	float  Integral;    //���ֳ���
	float  Derivative;  //΢�ֳ���

	float  LastError;   //��һ�����
	float  PrevError;   //ǰһ�����
}PIDTypdDef;

extern PIDTypdDef RSencer,LSencer;

/*��ʼ��RSencer�ṹ�����*/
void PID_RSencer_Init(void);
/*��ʼ��LSencer�ṹ�����*/
void PID_LSencer_Init(void);
/*����RSencer����ֵ*/
void PID_RSencer_SetPoint(float setpoint);
/*����LSencer����ֵ*/
void PID_LSencer_SetPoint(float setpoint);
/*����RSencer��PID����*/
void PID_RSencer_SetPID(float P,float I,float D);
	/*����LSencer��PID����*/
void PID_LSencer_SetPID(float P,float I,float D);
/*RSencerλ��ʽPID����*/
//�������Ϊ��ǰ��������õ�ֵ
//����ֵ��PID���������ֵ
int PID_RSencer_Calculate(float CurValue);
/*LSencerλ��ʽPID����*/
//�������Ϊ��ǰ��������õ�ֵ
//����ֵ��PID���������ֵ
int PID_LSencer_Calculate(float CurValue);
/*��ƽ��С��PI��*/
int velocity(int encoder_left,int encoder_right);
		 				    
#endif
