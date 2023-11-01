/*
�ó�����PID���ڵĺ�����
��Ƭ����stm32f103
���ߣ�����
ʱ�䣺2018.3.22
*/
#include "PID.h"

PIDTypdDef RSencer,LSencer;

/*��ʼ��RSencer�ṹ�����*/
void PID_RSencer_Init(void)
{
    RSencer.LastError  = 0;		
    RSencer.PrevError  = 0;			
		RSencer.Proportion = 0;			
    RSencer.Integral   = 0;		
    RSencer.Derivative = 0;			
    RSencer.SetPoint   = 0;    
		RSencer.SumError   = 0;     
}

/*��ʼ��LSencer�ṹ�����*/
void PID_LSencer_Init(void)
{
    LSencer.LastError  = 0;	
    LSencer.PrevError  = 0;			
		LSencer.Proportion = 0;			
    LSencer.Integral   = 0;			
    LSencer.Derivative = 0;	
    LSencer.SetPoint   = 0;    
		LSencer.SumError   = 0;     
}

/*����RSencer����ֵ*/
void PID_RSencer_SetPoint(float setpoint)
{	
		RSencer.SetPoint = setpoint;	
}

/*����LSencer����ֵ*/
void PID_LSencer_SetPoint(float setpoint)
{	
		LSencer.SetPoint = setpoint;	
}

/*����RSencer��PID����*/
void PID_RSencer_SetPID(float P,float I,float D)
{
		RSencer.Proportion = P;			
    RSencer.Integral   = I;		
    RSencer.Derivative = D;
}

/*����LSencer��PID����*/
void PID_LSencer_SetPID(float P,float I,float D)
{
		LSencer.Proportion = P;			
    LSencer.Integral   = I;			
    LSencer.Derivative = D;	
}

/*RSencerλ��ʽPID����*/
//�������Ϊ��ǰ��������õ�ֵ
//����ֵ��PID���������ֵ
int PID_RSencer_Calculate(float CurValue)
{
	float  iError,dError;                              //��ʱ����

	iError = RSencer.SetPoint - CurValue;              //ƫ��
	RSencer.SumError += iError;				                 //����
	if(RSencer.SumError > 1500.0)					             //�����޷�
			RSencer.SumError = 1500.0;
	else if(RSencer.SumError < -1500.0)
			RSencer.SumError = -1500.0;	
	dError = iError - RSencer.LastError; 			         //��ǰ΢��
	RSencer.LastError = iError;
	
	return(int)(RSencer.Proportion * iError            //������
          	+ RSencer.Integral   * RSencer.SumError  //������
          	+ RSencer.Derivative * dError);          //΢����
}

/*LSencerλ��ʽPID����*/
//�������Ϊ��ǰ��������õ�ֵ
//����ֵ��PID���������ֵ
int PID_LSencer_Calculate(float CurValue)
{
	float  iError,dError;                              //��ʱ����

	iError = LSencer.SetPoint - CurValue;              //ƫ��
	LSencer.SumError += iError;				                 //����
	if(LSencer.SumError > 1500.0)                      //�����޷�
			LSencer.SumError = 1500.0;
	else if(LSencer.SumError < -1500.0)
			LSencer.SumError = -1500.0;	
	dError = iError - LSencer.LastError; 			         //��ǰ΢��
	LSencer.LastError = iError;
	
	return(int)(LSencer.Proportion * iError            //������
          	+ LSencer.Integral   * LSencer.SumError  //������
            + LSencer.Derivative * dError);          //΢����
}

/*��ƽ��С��PI��*/
int velocity(int encoder_left,int encoder_right)
{
	static float Velocity,Encoder_iError,Encoder;
	static float Encoder_Integral;
	float kp=16,ki=0.08;
	Encoder_iError=0-(encoder_left+encoder_right);
	
	/*�˲�*/
	Encoder *=0.7;
	Encoder +=Encoder_iError*0.3;
	
	Encoder_Integral+=Encoder;
	
	if(Encoder_Integral>=10000)Encoder_Integral=10000;
	if(Encoder_Integral<=-10000)Encoder_Integral=-10000;
	
	Velocity=Encoder*kp+Encoder_Integral*ki;
	return Velocity;
}
