/************************************
定义了MPU6050数据与匿名四轴上位机的相关通讯协议
以及数据发送函数
**************************************/
#ifndef __MCUDATASEND_H
#define __MCUDATASEND_H
#include "sys.h"
#include "stm32f10x.h"

void Data_Send_Status(short roll,short pitch,short yaw);
void Data_Send_Senser(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz);

#endif
