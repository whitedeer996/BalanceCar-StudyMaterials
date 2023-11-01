/*
�ú�����OLED������
����SSD1306�����޸�
��Ƭ����stm32f103
���ߣ�����
ʱ�䣺2018.3.6
*/
#include "OLED.h"
#include "delay.h"
#include "codetab.h"

/*IIC��ʱ����*/
void IIC_Delay(void)
{
	delay_us(1);//ʵ�����С��5us
}

/*IIC�˿ڳ�ʼ��*/
void IIC_Init(void)
{					     
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);     //��ʹ������IO PORTBʱ�� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;     //�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIO 
	
	GPIO_SetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9);			     //PB.8,PB.9�����	
}

/*IIC��ʼ�ź�*/
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	IIC_Delay();   //��ʱ>4.7us
 	IIC_SDA=0;     //SCL��ʱ�������½���
	IIC_Delay();   //��ʱ>4us
	IIC_SCL=0;     //SCLΪ��ʱ�����������ݱ仯
}

/*����IICֹͣ�ź�*/
void IIC_Stop(void)
{
	SDA_OUT();     //sda�����
	IIC_SCL=0;
	IIC_SDA=0;
 	IIC_Delay();   //��ʱ
	IIC_SCL=1;    
	IIC_Delay();   //��ʱ>4us
	IIC_SDA=1;     //SCL��ʱ������������
	IIC_Delay();   //��ʱ>4.7us							   	
}

/*����ACKӦ��*/
void IIC_Ack(void)
{
    IIC_SCL=0;
    SDA_OUT();
    IIC_SDA=0;     //����SDA
    IIC_Delay();   //��ʱ
    IIC_SCL=1;     //SCLΪ��ʱ��SDAΪ�ͣ�����>4us��Ӧ��/0
    IIC_Delay();   //��ʱ>4us
    IIC_SCL=0;
}

/*����NACK��Ӧ��*/      
void IIC_NAck(void)
{
    IIC_SCL=0;
    SDA_OUT();
    IIC_SDA=1;     //����SDA
    IIC_Delay();   //��ʱ
    IIC_SCL=1;     //SCLΪ��ʱ��SDAΪ�ߣ�����>4us����Ӧ��/1
    IIC_Delay();   //��ʱ>4us
    IIC_SCL=0;
}

/*�ȴ��ӻ�Ӧ���ź�*/
//����ֵ��1 ����Ӧ��ʧ��
//        0 ����Ӧ��ɹ�
unsigned char IIC_Wait_Ack(void)
{
    unsigned char Time=0;
 
    SDA_IN();          //����Ϊ�������롣
    IIC_SDA=1;         //�����ͷ��������ߣ��ȴ��ӻ�����Ӧ���ź�
    IIC_Delay();
    IIC_SCL=1;         //SCLΪ��ʱ���ȴ��ӻ�Ӧ��
    IIC_Delay();       
    while(Read_SDA)    //SDA����ʱ����ȷӦ��
    {
        Time++;
        //�����ʱ
        if(Time>250)   //���ʱ�䳬ʱ��û��Ӧ���ֹͣ
		{
			IIC_Stop();
			return 1;  //û����Ӧ����1
		} 
	}
    IIC_SCL=0;        //������������
    return 0;         //����Ӧ����0
}

/*IIC����һ���ֽ�*/        
void IIC_Send_Byte(unsigned char txd)
{                        
    unsigned char t;   
    SDA_OUT();      
    IIC_SCL=0;        //����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)  //����һ���ֽڣ������λ��ʼ����
    {       
		IIC_Delay(); 
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1;      
        IIC_Delay();   
        IIC_SCL=1;
        IIC_Delay();
        IIC_SCL=0;  
    } 
	IIC_Delay();
} 

/*��1���ֽڣ�ack=1ʱ������Ack��ack=0������NAck*/
//����ֵ����ȡ�����ֽ�  
unsigned char IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA_IN();        //SDA����Ϊ����
    for(i=0;i<8;i++) //����һ���ֽڣ��Ӹ�λ��ʼ����
    {
        IIC_SCL=0; 
        IIC_Delay(); 
        IIC_SCL=1;
        receive<<=1;
        if(Read_SDA) receive++;   
        IIC_Delay(); 
    }                    
    if (!ack)
        IIC_NAck();  //����nACK
    else
        IIC_Ack();   //����ACK   
    return receive;
}

/*д����*/
void WriteCmd(unsigned char IIC_Command)
{
	IIC_Start();
	IIC_Send_Byte(0x78);                   //D/C�Žӵ�ʱ��ssd1306�Ĵ�����ַΪ0x78
	IIC_Wait_Ack();                        //�ȴ��ӻ�Ӧ��
	IIC_Send_Byte(0x00);                   //���½��������ݣ����������Ϊ����
	IIC_Wait_Ack();
	IIC_Send_Byte(IIC_Command);
	IIC_Wait_Ack();
	IIC_Stop();
}

/*д����*/
void WriteDat(unsigned char IIC_Data)
{
	IIC_Start();
	IIC_Send_Byte(0x78);                   //D/C�Žӵ�ʱ��ssd1306�Ĵ�����ַΪ0x78
	IIC_Wait_Ack();                        //�ȴ��ӻ�Ӧ��
	IIC_Send_Byte(0x40);                   //���½��������ݣ���������ݽ������浽GDDRAM
	IIC_Wait_Ack();
	IIC_Send_Byte(IIC_Data);
	IIC_Wait_Ack();
	IIC_Stop();
}

/*��ʼ��OLED*/
void OLED_Init(void)
{
	IIC_Init();   //I2C��ʼ�� 
	delay_ms(100);//����һ������ʱ

	WriteCmd(0xA8);//��������·��
	WriteCmd(0x3F);//Ĭ��0X3F(1/64)

	WriteCmd(0xD3);//������ʾƫ��
	WriteCmd(0x00);//Ĭ��Ϊ0

	WriteCmd(0x40);//������ʾ��ʼ�� [5:0],����.

	WriteCmd(0xA1);//���ض�������,bit0:0,0->0;1,0->127;

	WriteCmd(0xC8);//����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��

	WriteCmd(0xDA);//����COMӲ����������
	WriteCmd(0x12);//[5:4]����

	WriteCmd(0x81);//�Աȶ�����
	WriteCmd(0x7F);//1~255;Ĭ��0X7F (��������,Խ��Խ��)

	WriteCmd(0xA4);//ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)

	WriteCmd(0xA6);//������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ

	WriteCmd(0xD5);//����ʱ�ӷ�Ƶ����,��Ƶ��
	WriteCmd(0x80);//[3:0],��Ƶ����;[7:4],��Ƶ��

	WriteCmd(0x8D);//��ɱ�����
	WriteCmd(0x14); //bit2������/�ر�

	WriteCmd(0xAF); //������ʾ	 
}

/*������ʼ������*/
void OLED_SetPos(unsigned char x, unsigned char y)
{ 
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)|0x01);
}

/*ȫ�����*/
void OLED_Fill(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
			{
				WriteDat(fill_Data);
			}
	}
}

/*����*/
void OLED_CLS(void)
{
	OLED_Fill(0x00);
}

/*OLED����*/
void OLED_ON(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X14);  //������ɱ�
	WriteCmd(0XAF);  //OLED����
}

/*OLED����*/
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X10);  //�رյ�ɱ�
	WriteCmd(0XAE);  
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
// Calls          : 
// Parameters     : x,y -- ��ʼ������(x:0~127, y:0~7); ch[] -- Ҫ��ʾ���ַ���; TextSize -- �ַ���С(1:6*8 ; 2:8*16)
// Description    : ��ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ��
//--------------------------------------------------------------
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					WriteDat(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		case 2:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
// Calls          : 
// Parameters     : x,y -- ��ʼ������(x:0~127, y:0~7); N:������codetab.h�е�����
// Description    : ��ʾcodetab.h�еĺ���,16*16����
//--------------------------------------------------------------
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0;
	unsigned int  adder=32*N;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
}

/*��ʾ����*/
void OLED_ShowOneNum(unsigned char x, unsigned char y, int num, unsigned char TextSize)
	{
	unsigned char c = 0,i = 0;
	switch(TextSize)
	{
		case 1:
		{
			  c=num+16;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					WriteDat(F6x8[c][i]);
		}break;
		case 2:
		{			
				c = 16+num;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i+8]);
				x += 8;
		}break;
	}
 }
	
/*��ʾ���ִ�*/
void OLED_ShowNum(unsigned char x, unsigned char y, u16 num, unsigned char TextSize)
 {
	int i=0;
	int  n;
	int a[4];   //�Ϊ��λ��
  if (num==0)
	OLED_ShowOneNum(x,y,num,TextSize);
	else
	{			
    while(num>=10) 
		{a[i]=num%10; num/=10;i++;}
		 a[i]=num;
		 n=i;
		for(i=0;i<5;i++)  
		{
			if(n-i<0)  OLED_ShowOneNum(x+6*i,y,-16,TextSize);
			else  OLED_ShowOneNum(x+6*i,y,a[n-i],TextSize);
		}
	}
}
/*�Զ���UI����*/
void UI(void)
{
	
		OLED_ShowOneNum(0,0,79,2);   //��601��
		OLED_ShowOneNum(8,0,80,2);
		OLED_ShowOneNum(16,0,81,2);
	  
	  OLED_ShowCN(24,0,1);       //�����µ��ӡ�
		OLED_ShowCN(40,0,2);
		OLED_ShowCN(56,0,3);
		OLED_ShowCN(72,0,4);
	 
		OLED_ShowCN(111,0,0);      //16x16 logo
	
	  OLED_ShowStr(6,4,"kp:",1);//pid����
		OLED_ShowStr(6,5,"ki:",1);
		OLED_ShowStr(6,6,"kd:",1);

		OLED_ShowStr(63,4,"TA:",1);//Ŀ��ֵAΪ�Ƕȣ�VΪ�ٶ�
	  OLED_ShowStr(63,5,"TV:",1);	
	
		OLED_ShowStr(63,6,"Da:",1);//���ڽ��յ�����
	
 //   OLED_ShowStr(36,7,"QQ:1103298792",1);	

}

