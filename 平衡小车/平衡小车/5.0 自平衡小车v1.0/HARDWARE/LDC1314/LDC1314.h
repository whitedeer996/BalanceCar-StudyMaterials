/*
该程序是LDC1314函数库
单片机：stm32f103
作者：鹅鹅鹅
时间：2018.3.11
*/

#ifndef LDC1314_H
#define LDC1314_H

#include "sys.h"

/*IO方向设置*/
#define LDC_SDA_IN()  {GPIOC->CRL&=0XFF0FFFFF;GPIOC->CRL|=8<<(4*5);}//上/下拉输入模式
#define LDC_SDA_OUT() {GPIOC->CRL&=0XFF0FFFFF;GPIOC->CRL|=3<<(4*5);}//推挽输出模式

/*IO操作函数*/	 
#define LDC_IIC_SCL    PCout(4) 	//SCL输出
#define LDC_IIC_SDA    PCout(5)     //SDA输出	 
#define LDC_Read_SDA   PCin(5) 		//SDA输入

/*addr引脚接地，LCD_IIC的地址为0x2A*/
#define LDC1314_IICADDR     0x2A
/*addr引脚接高，LCD_IIC的地址为0x2B*/
#define LDC1314_I2CADDR2     0x2B

extern unsigned short allData[8];

/*IIC延时函数*/
void LDC_IIC_Delay(void);
/*IIC端口初始化*/
void LDC_IIC_Init(void);
/*IIC起始信号*/
void LDC_IIC_Start(void);
/*产生IIC停止信号*/
void LDC_IIC_Stop(void);
/*产生ACK应答*/
void LDC_IIC_Ack(void);
/*产生NACK非应答*/      
void LDC_IIC_NAck(void);
/*等待从机应答信号*/
//返回值：1 接收应答成功
//        0 接收应答失败
unsigned char LDC_IIC_Wait_Ack(void);
/*IIC发送一个字节*/        
void LDC_IIC_Send_Byte(unsigned char txd);
/*读1个字节，ack=1时，发送Ack，ack=0，发送NAck*/
//返回值：读取到的字节  
unsigned char LDC_IIC_Read_Byte(unsigned char ack);
/*指定地址写数据*/
//返回值：1 写数据成功
//        0 写数据失败
unsigned char LDC_IIC_WriteWord(unsigned char SlaveAddr, unsigned char RegisterAddr, unsigned short data);
/*指定地址读数据*/
//返回值：1 写数据成功
//        0 写数据失败
unsigned char LDC_IIC_ReadWord(unsigned char SlaveAddr, unsigned char RegisterAddr, unsigned short* data);
/*LDC1314初始化*/
//返回值：1 初始化成功
//        0 初始化失败
unsigned char LDC1314_Init(void);
void LDC1314_ProcessDRDY(void);

#endif
