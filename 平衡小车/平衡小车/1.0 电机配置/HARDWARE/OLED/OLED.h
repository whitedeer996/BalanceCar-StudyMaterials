/*
�ú�����OLED������
����SSD1306�����޸�
��Ƭ����stm32f103
���ߣ�����
ʱ�䣺2018.3.6
*/

#ifndef OLED_H
#define OLED_H

#include "sys.h"

/*IO��������*/
#define SDA_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=8<<4;}//��/��������ģʽ
#define SDA_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=3<<4;}//�������ģʽ

/*IO��������*/	 
#define IIC_SCL    PBout(8) 	//SCL���
#define IIC_SDA    PBout(9)     //SDA���	 
#define Read_SDA   PBin(9) 		//SDA����

/*IIC��ʱ����*/
void IIC_Delay(void);
/*IIC�˿ڳ�ʼ��*/
void IIC_Init(void);
/*IIC��ʼ�ź�*/
void IIC_Start(void);
/*����IICֹͣ�ź�*/
void IIC_Stop(void);
/*����ACKӦ��*/
void IIC_Ack(void);
/*����NACK��Ӧ��*/      
void IIC_NAck(void);
/*�ȴ��ӻ�Ӧ���ź�*/
//����ֵ��1 ����Ӧ��ʧ��
//        0 ����Ӧ��ɹ�
unsigned char IIC_Wait_Ack(void);
/*IIC����һ���ֽ�*/        
void IIC_Send_Byte(unsigned char txd);
/*��1���ֽڣ�ack=1ʱ������Ack��ack=0������NAck*/   
//����ֵ����ȡ�����ֽ� 
unsigned char IIC_Read_Byte(unsigned char ack);
/*д����*/
void WriteCmd(unsigned char IIC_Command);
/*д����*/
void WriteDat(unsigned char IIC_Data);
/*��ʼ��OLED*/
void OLED_Init(void);
/*������ʼ������*/
void OLED_SetPos(unsigned char x, unsigned char y);
/*ȫ�����*/
void OLED_Fill(unsigned char fill_Data);
/*����*/
void OLED_CLS(void);
/*����OLED*/
void OLED_ON(void);
/*OLED����*/
void OLED_OFF(void);
/*��ʾcodetab.h�е�ASCII�ַ�*/
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
/*��ʾcodetab.h�еĺ���*/
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
/*��ʾ����*/
void OLED_ShowOneNum(unsigned char x, unsigned char y, int num, unsigned char TextSize);
/*��ʾ���ִ�*/
void OLED_ShowNum(unsigned char x, unsigned char y, u16 num, unsigned char TextSize);
/*�Զ���UI����*/
void UI(void);

#endif
