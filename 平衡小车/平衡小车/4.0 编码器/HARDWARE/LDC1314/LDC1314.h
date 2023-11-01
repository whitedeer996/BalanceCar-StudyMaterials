/*
�ó�����LDC1314������
��Ƭ����stm32f103
���ߣ�����
ʱ�䣺2018.3.11
*/

#ifndef LDC1314_H
#define LDC1314_H

#include "sys.h"

/*IO��������*/
#define LDC_SDA_IN()  {GPIOC->CRL&=0XFF0FFFFF;GPIOC->CRL|=8<<(4*5);}//��/��������ģʽ
#define LDC_SDA_OUT() {GPIOC->CRL&=0XFF0FFFFF;GPIOC->CRL|=3<<(4*5);}//�������ģʽ

/*IO��������*/	 
#define LDC_IIC_SCL    PCout(4) 	//SCL���
#define LDC_IIC_SDA    PCout(5)     //SDA���	 
#define LDC_Read_SDA   PCin(5) 		//SDA����

/*addr���Žӵأ�LCD_IIC�ĵ�ַΪ0x2A*/
#define LDC1314_IICADDR     0x2A
/*addr���ŽӸߣ�LCD_IIC�ĵ�ַΪ0x2B*/
#define LDC1314_I2CADDR2     0x2B

extern unsigned short allData[8];

/*IIC��ʱ����*/
void LDC_IIC_Delay(void);
/*IIC�˿ڳ�ʼ��*/
void LDC_IIC_Init(void);
/*IIC��ʼ�ź�*/
void LDC_IIC_Start(void);
/*����IICֹͣ�ź�*/
void LDC_IIC_Stop(void);
/*����ACKӦ��*/
void LDC_IIC_Ack(void);
/*����NACK��Ӧ��*/      
void LDC_IIC_NAck(void);
/*�ȴ��ӻ�Ӧ���ź�*/
//����ֵ��1 ����Ӧ��ɹ�
//        0 ����Ӧ��ʧ��
unsigned char LDC_IIC_Wait_Ack(void);
/*IIC����һ���ֽ�*/        
void LDC_IIC_Send_Byte(unsigned char txd);
/*��1���ֽڣ�ack=1ʱ������Ack��ack=0������NAck*/
//����ֵ����ȡ�����ֽ�  
unsigned char LDC_IIC_Read_Byte(unsigned char ack);
/*ָ����ַд����*/
//����ֵ��1 д���ݳɹ�
//        0 д����ʧ��
unsigned char LDC_IIC_WriteWord(unsigned char SlaveAddr, unsigned char RegisterAddr, unsigned short data);
/*ָ����ַ������*/
//����ֵ��1 д���ݳɹ�
//        0 д����ʧ��
unsigned char LDC_IIC_ReadWord(unsigned char SlaveAddr, unsigned char RegisterAddr, unsigned short* data);
/*LDC1314��ʼ��*/
//����ֵ��1 ��ʼ���ɹ�
//        0 ��ʼ��ʧ��
unsigned char LDC1314_Init(void);
void LDC1314_ProcessDRDY(void);

#endif
