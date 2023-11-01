#ifndef __MPUIIC_H
#define __MPUIIC_H
#include "sys.h"

/*PB1 CLK
  PB3  DATA*/
 	   		   
//IO方向设置
//#define MPU_SDA_IN()  {GPIOB->CRL&=0XFFFF0FFF;GPIOB->CRL|=8<<12;}
//#define MPU_SDA_OUT() {GPIOB->CRL&=0XFFFF0FFF;GPIOB->CRL|=3<<12;}

////IO操作函数	 
//#define MPU_IIC_SCL    PBout(1) 		//SCL
//#define MPU_IIC_SDA    PBout(3) 		//SDA	 
//#define MPU_READ_SDA   PBin(3) 		//输入SDA 

//IO方向设置
//#define MPU_SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=8<<4;}
//#define MPU_SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOC->CRL|=3<<4;}
#define MPU_SDA_IN()  {GPIOC->CRL&=0XFF0FFFFF;GPIOC->CRL|=8<<(4*5);}//上/下拉输入模式
#define MPU_SDA_OUT() {GPIOC->CRL&=0XFF0FFFFF;GPIOC->CRL|=3<<(4*5);}//推挽输出模式
//IO操作函数	 
//#define MPU_IIC_SCL    PCout(0) 	//SCL-PC0
//#define MPU_IIC_SDA    PCout(1)   //输出SDA-PC1	 
//#define MPU_READ_SDA   PCin(1) 	
#define MPU_IIC_SCL   PCout(4) 	//SCL输出
#define MPU_IIC_SDA   PCout(5)     //SDA输出	 
#define MPU_READ_SDA  PCin(5) 		//SDA输入
//IIC所有操作函数
void MPU_IIC_Delay(void);				//MPU IIC延时函数
void MPU_IIC_Init(void);                //初始化IIC的IO口				 
void MPU_IIC_Start(void);				//发送IIC开始信号
void MPU_IIC_Stop(void);	  			//发送IIC停止信号
void MPU_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 MPU_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 MPU_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void MPU_IIC_Ack(void);					//IIC发送ACK信号
void MPU_IIC_NAck(void);				//IIC不发送ACK信号

void IMPU_IC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 MPU_IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















