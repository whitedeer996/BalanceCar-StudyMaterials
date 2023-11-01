/*
该程序是PID调节的函数库
单片机：stm32f103
作者：鹅鹅鹅
时间：2018.3.22
*/
#ifndef PID_H
#define PID_H	 
#include "sys.h"

/*PID结构体参数*/
typedef struct
{
	float  SetPoint; 	//设定目标
	double  SumError;	//误差累计 
		
	float  Proportion;  //比例常数 
	float  Integral;    //积分常数
	float  Derivative;  //微分常数

	float  LastError;   //上一次误差
	float  PrevError;   //前一次误差
}PIDTypdDef;

extern PIDTypdDef RSencer,LSencer;

/*初始化RSencer结构体参数*/
void PID_RSencer_Init(void);
/*初始化LSencer结构体参数*/
void PID_LSencer_Init(void);
/*设置RSencer期望值*/
void PID_RSencer_SetPoint(float setpoint);
/*设置LSencer期望值*/
void PID_LSencer_SetPoint(float setpoint);
/*设置RSencer的PID参数*/
void PID_RSencer_SetPID(float P,float I,float D);
	/*设置LSencer的PID参数*/
void PID_LSencer_SetPID(float P,float I,float D);
/*RSencer位置式PID计算*/
//调入参数为当前传感器测得的值
//返回值：PID计算后的输出值
int PID_RSencer_Calculate(float CurValue);
/*LSencer位置式PID计算*/
//调入参数为当前传感器测得的值
//返回值：PID计算后的输出值
int PID_LSencer_Calculate(float CurValue);
/*自平衡小车PI环*/
int velocity(int encoder_left,int encoder_right);
		 				    
#endif
