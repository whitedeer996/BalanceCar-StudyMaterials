#ifndef __MPUIIC_H
#define __MPUIIC_H
#include "sys.h"

/*PB1 CLK
  PB3  DATA*/
 	   		   
//IO��������
//#define MPU_SDA_IN()  {GPIOB->CRL&=0XFFFF0FFF;GPIOB->CRL|=8<<12;}
//#define MPU_SDA_OUT() {GPIOB->CRL&=0XFFFF0FFF;GPIOB->CRL|=3<<12;}

////IO��������	 
//#define MPU_IIC_SCL    PBout(1) 		//SCL
//#define MPU_IIC_SDA    PBout(3) 		//SDA	 
//#define MPU_READ_SDA   PBin(3) 		//����SDA 

//IO��������
//#define MPU_SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=8<<4;}
//#define MPU_SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOC->CRL|=3<<4;}
#define MPU_SDA_IN()  {GPIOC->CRL&=0XFF0FFFFF;GPIOC->CRL|=8<<(4*5);}//��/��������ģʽ
#define MPU_SDA_OUT() {GPIOC->CRL&=0XFF0FFFFF;GPIOC->CRL|=3<<(4*5);}//�������ģʽ
//IO��������	 
//#define MPU_IIC_SCL    PCout(0) 	//SCL-PC0
//#define MPU_IIC_SDA    PCout(1)   //���SDA-PC1	 
//#define MPU_READ_SDA   PCin(1) 	
#define MPU_IIC_SCL   PCout(4) 	//SCL���
#define MPU_IIC_SDA   PCout(5)     //SDA���	 
#define MPU_READ_SDA  PCin(5) 		//SDA����
//IIC���в�������
void MPU_IIC_Delay(void);				//MPU IIC��ʱ����
void MPU_IIC_Init(void);                //��ʼ��IIC��IO��				 
void MPU_IIC_Start(void);				//����IIC��ʼ�ź�
void MPU_IIC_Stop(void);	  			//����IICֹͣ�ź�
void MPU_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 MPU_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 MPU_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void MPU_IIC_Ack(void);					//IIC����ACK�ź�
void MPU_IIC_NAck(void);				//IIC������ACK�ź�

void IMPU_IC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 MPU_IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















