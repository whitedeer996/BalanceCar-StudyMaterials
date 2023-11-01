/*
该程序是PID调节的函数库
单片机：stm32f103
作者：鹅鹅鹅
时间：2018.3.22
*/
#include "PID.h"

PIDTypdDef RSencer,LSencer;

/*初始化RSencer结构体参数*/
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

/*初始化LSencer结构体参数*/
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

/*设置RSencer期望值*/
void PID_RSencer_SetPoint(float setpoint)
{	
		RSencer.SetPoint = setpoint;	
}

/*设置LSencer期望值*/
void PID_LSencer_SetPoint(float setpoint)
{	
		LSencer.SetPoint = setpoint;	
}

/*设置RSencer的PID参数*/
void PID_RSencer_SetPID(float P,float I,float D)
{
		RSencer.Proportion = P;			
    RSencer.Integral   = I;		
    RSencer.Derivative = D;
}

/*设置LSencer的PID参数*/
void PID_LSencer_SetPID(float P,float I,float D)
{
		LSencer.Proportion = P;			
    LSencer.Integral   = I;			
    LSencer.Derivative = D;	
}

/*RSencer位置式PID计算*/
//调入参数为当前传感器测得的值
//返回值：PID计算后的输出值
int PID_RSencer_Calculate(float CurValue)
{
	float  iError,dError;                              //临时变量

	iError = RSencer.SetPoint - CurValue;              //偏差
	RSencer.SumError += iError;				                 //积分
	if(RSencer.SumError > 1500.0)					             //积分限幅
			RSencer.SumError = 1500.0;
	else if(RSencer.SumError < -1500.0)
			RSencer.SumError = -1500.0;	
	dError = iError - RSencer.LastError; 			         //当前微分
	RSencer.LastError = iError;
	
	return(int)(RSencer.Proportion * iError            //比例项
          	+ RSencer.Integral   * RSencer.SumError  //积分项
          	+ RSencer.Derivative * dError);          //微分项
}

/*LSencer位置式PID计算*/
//调入参数为当前传感器测得的值
//返回值：PID计算后的输出值
int PID_LSencer_Calculate(float CurValue)
{
	float  iError,dError;                              //临时变量

	iError = LSencer.SetPoint - CurValue;              //偏差
	LSencer.SumError += iError;				                 //积分
	if(LSencer.SumError > 1500.0)                      //积分限幅
			LSencer.SumError = 1500.0;
	else if(LSencer.SumError < -1500.0)
			LSencer.SumError = -1500.0;	
	dError = iError - LSencer.LastError; 			         //当前微分
	LSencer.LastError = iError;
	
	return(int)(LSencer.Proportion * iError            //比例项
          	+ LSencer.Integral   * LSencer.SumError  //积分项
            + LSencer.Derivative * dError);          //微分项
}

/*自平衡小车PI环*/
int velocity(int encoder_left,int encoder_right)
{
	static float Velocity,Encoder_iError,Encoder;
	static float Encoder_Integral;
	float kp=16,ki=0.08;
	Encoder_iError=0-(encoder_left+encoder_right);
	
	/*滤波*/
	Encoder *=0.7;
	Encoder +=Encoder_iError*0.3;
	
	Encoder_Integral+=Encoder;
	
	if(Encoder_Integral>=10000)Encoder_Integral=10000;
	if(Encoder_Integral<=-10000)Encoder_Integral=-10000;
	
	Velocity=Encoder*kp+Encoder_Integral*ki;
	return Velocity;
}
