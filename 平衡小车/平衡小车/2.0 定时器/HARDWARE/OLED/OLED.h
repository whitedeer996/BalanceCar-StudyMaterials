/*
该函数是OLED函数库
根据SSD1306略有修改
单片机：stm32f103
作者：鹅鹅鹅
时间：2018.3.6
*/

#ifndef OLED_H
#define OLED_H

#include "sys.h"

/*IO方向设置*/
#define SDA_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=8<<4;}//上/下拉输入模式
#define SDA_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=3<<4;}//推挽输出模式

/*IO操作函数*/	 
#define IIC_SCL    PBout(8) 	//SCL输出
#define IIC_SDA    PBout(9)     //SDA输出	 
#define Read_SDA   PBin(9) 		//SDA输入

/*IIC延时函数*/
void IIC_Delay(void);
/*IIC端口初始化*/
void IIC_Init(void);
/*IIC起始信号*/
void IIC_Start(void);
/*产生IIC停止信号*/
void IIC_Stop(void);
/*产生ACK应答*/
void IIC_Ack(void);
/*产生NACK非应答*/      
void IIC_NAck(void);
/*等待从机应答信号*/
//返回值：1 接收应答失败
//        0 接收应答成功
unsigned char IIC_Wait_Ack(void);
/*IIC发送一个字节*/        
void IIC_Send_Byte(unsigned char txd);
/*读1个字节，ack=1时，发送Ack，ack=0，发送NAck*/   
//返回值：读取到的字节 
unsigned char IIC_Read_Byte(unsigned char ack);
/*写命令*/
void WriteCmd(unsigned char IIC_Command);
/*写数据*/
void WriteDat(unsigned char IIC_Data);
/*初始化OLED*/
void OLED_Init(void);
/*设置起始点坐标*/
void OLED_SetPos(unsigned char x, unsigned char y);
/*全屏填充*/
void OLED_Fill(unsigned char fill_Data);
/*清屏*/
void OLED_CLS(void);
/*唤醒OLED*/
void OLED_ON(void);
/*OLED休眠*/
void OLED_OFF(void);
/*显示codetab.h中的ASCII字符*/
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
/*显示codetab.h中的汉字*/
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
/*显示数字*/
void OLED_ShowOneNum(unsigned char x, unsigned char y, int num, unsigned char TextSize);
/*显示数字串*/
void OLED_ShowNum(unsigned char x, unsigned char y, u16 num, unsigned char TextSize);
/*自定义UI界面*/
void UI(void);

#endif
