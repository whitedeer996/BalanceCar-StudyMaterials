#ifndef __ENCODER_H
#define __ENCODER_H
#include <sys.h>	 
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
#define ENCODER_TIM_PERIOD (u16)(65535)   //不可大于65535 因为F103的定时器是16位的。
#define COUNTER_RESET 10000   //在位置控制里面，为方便代码书写，我们设定CNT的初始值为10000，在速度控制里面，改成0即可
void Encoder_Init_TIM2(void);
void Encoder_Init_TIM4(void);
int Read_Encoder(u8 TIMX);
#endif
