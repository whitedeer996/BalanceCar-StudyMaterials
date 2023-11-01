/*
�ó�����LDC1314������
��Ƭ����stm32f103
���ߣ�����
ʱ�䣺2018.3.11
*/
#include "LDC1314.h"
#include "LDC1314_cmd.h"
#include "delay.h"

unsigned short allData[8];

/*IIC��ʱ����*/
void LDC_IIC_Delay(void)
{
	delay_us(5);//ʵ�����С��5us
}

/*IIC�˿ڳ�ʼ��*/
void LDC_IIC_Init(void)
{					     
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);     //��ʹ������IO PORTCʱ�� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;     //�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIO 
	
	GPIO_SetBits(GPIOC,GPIO_Pin_4|GPIO_Pin_5);			     //PC.4��PC.5�����	
}

/*IIC��ʼ�ź�*/
void LDC_IIC_Start(void)
{
	LDC_SDA_OUT();     //sda�����
	LDC_IIC_SDA=1;	  	  
	LDC_IIC_SCL=1;
	LDC_IIC_Delay();   //��ʱ>4.7us
 	LDC_IIC_SDA=0;     //SCL��ʱ�������½���
	LDC_IIC_Delay();   //��ʱ>4us
	LDC_IIC_SCL=0;     //SCLΪ��ʱ�����������ݱ仯
}

/*����IICֹͣ�ź�*/
void LDC_IIC_Stop(void)
{
	LDC_SDA_OUT();     //sda�����
	LDC_IIC_SCL=0;
	LDC_IIC_SDA=0;
 	LDC_IIC_Delay();   //��ʱ
	LDC_IIC_SCL=1;    
	LDC_IIC_Delay();   //��ʱ>4us
	LDC_IIC_SDA=1;     //SCL��ʱ������������
	LDC_IIC_Delay();   //��ʱ>4.7us							   	
}

/*����ACKӦ��*/
void LDC_IIC_Ack(void)
{
    LDC_IIC_SCL=0;
    LDC_SDA_OUT();
    LDC_IIC_SDA=0;     //����SDA
    LDC_IIC_Delay();   //��ʱ
    LDC_IIC_SCL=1;     //SCLΪ��ʱ��SDAΪ�ͣ�����>4us��Ӧ��/0
    LDC_IIC_Delay();   //��ʱ>4us
    LDC_IIC_SCL=0;
}

/*����NACK��Ӧ��*/      
void LDC_IIC_NAck(void)
{
    LDC_IIC_SCL=0;
    LDC_SDA_OUT();
    LDC_IIC_SDA=1;     //����SDA
    LDC_IIC_Delay();   //��ʱ
    LDC_IIC_SCL=1;     //SCLΪ��ʱ��SDAΪ�ߣ�����>4us����Ӧ��/1
    LDC_IIC_Delay();   //��ʱ>4us
    LDC_IIC_SCL=0;
}

/*�ȴ��ӻ�Ӧ���ź�*/
//����ֵ��1 ����Ӧ��ɹ�
//        0 ����Ӧ��ʧ��
unsigned char LDC_IIC_Wait_Ack(void)
{
    unsigned char Time=0;
	
		LDC_SDA_IN();          //����Ϊ�������롣
    LDC_IIC_SDA=1;         //�����ͷ��������ߣ��ȴ��ӻ�����Ӧ���ź�
		
    LDC_IIC_Delay();
    LDC_IIC_SCL=1;         //SCLΪ��ʱ���ȴ��ӻ�Ӧ��
    LDC_IIC_Delay();   
  
    while(LDC_Read_SDA)    //SDA����ʱ����ȷӦ��
    {
        Time++;
        //�����ʱ
        if(Time>250)       //���ʱ�䳬ʱ��û��Ӧ���ֹͣ
				{
					LDC_IIC_Stop();
					return 0;      //û����Ӧ����0
				} 
	}

    LDC_IIC_SCL=0;         //������������
    return 1;              //����Ӧ����1
}

/*IIC����һ���ֽ�*/        
void LDC_IIC_Send_Byte(unsigned char txd)
{                        
    unsigned char t;   
    LDC_SDA_OUT();      
    LDC_IIC_SCL=0;        //����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)      //����һ���ֽڣ������λ��ʼ����
    {       
		    LDC_IIC_Delay(); 
        LDC_IIC_SDA=(txd&0x80)>>7;
        txd<<=1;      
        LDC_IIC_Delay();   
        LDC_IIC_SCL=1;
        LDC_IIC_Delay();
        LDC_IIC_SCL=0;  
    } 
	  LDC_IIC_Delay();
} 

/*��1���ֽڣ�ack=1ʱ������Ack��ack=0������NAck*/
//����ֵ����ȡ�����ֽ�  
unsigned char LDC_IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    LDC_SDA_IN();        //SDA����Ϊ����
    for(i=0;i<8;i++) //����һ���ֽڣ��Ӹ�λ��ʼ����
    {
        LDC_IIC_SCL=0; 
        LDC_IIC_Delay(); 
        LDC_IIC_SCL=1;
        receive<<=1;
        if(LDC_Read_SDA) receive++;   
        LDC_IIC_Delay(); 
    }                    
    if (!ack)
        LDC_IIC_NAck();  //����nACK
    else
        LDC_IIC_Ack();   //����ACK   
    return receive;
}

/*ָ����ַд����*/
//����ֵ��1 д���ݳɹ�
//        0 д����ʧ��
unsigned char LDC_IIC_WriteWord(unsigned char SlaveAddr, unsigned char RegisterAddr, unsigned short data)
{
	 LDC_IIC_Start();
	 LDC_IIC_Send_Byte((SlaveAddr<<1)|0);//0Ϊд����
	 if (!LDC_IIC_Wait_Ack())
	 {
		 LDC_IIC_Stop();
		 return 0;//ʧ��
	 }
	 //GPIO_ResetBits(GPIOD,GPIO_Pin_2);	
	 LDC_IIC_Send_Byte(RegisterAddr);
	 LDC_IIC_Wait_Ack();
	 LDC_IIC_Send_Byte((unsigned char)(data>>8));
	 if (!LDC_IIC_Wait_Ack())
	 {
		 LDC_IIC_Stop();
		 return 0;//ʧ��
	 }
	 
	 LDC_IIC_Send_Byte((unsigned char)(data&0xFF));
	 if (!LDC_IIC_Wait_Ack())
	 {
		 LDC_IIC_Stop();
		 return 0;//ʧ��
	 }
	 LDC_IIC_Stop();
	 return 1;//�ɹ�
}

/*ָ����ַ������*/
//����ֵ��1 д���ݳɹ�
//        0 д����ʧ��
unsigned char LDC_IIC_ReadWord(unsigned char SlaveAddr, unsigned char RegisterAddr, unsigned short* data)
{
	 unsigned short word[2] = {0};
	 LDC_IIC_Start();
	 LDC_IIC_Send_Byte(SlaveAddr << 1 | 0);//0Ϊд����
	 
	 if (!LDC_IIC_Wait_Ack())
	 {
		 LDC_IIC_Stop();
		 return 0;//ʧ��
	 }
	 
	 LDC_IIC_Send_Byte(RegisterAddr);
	 LDC_IIC_Wait_Ack();

	 LDC_IIC_Start();
	 LDC_IIC_Send_Byte((SlaveAddr <<1)|1);//1Ϊ������
	 LDC_IIC_Wait_Ack();
	 
	 word[0] = LDC_IIC_Read_Byte(1);//���ֽ�
	 word[1] = LDC_IIC_Read_Byte(0);//���ֽ�
	 LDC_IIC_Stop();
	 
	 *data = (word[0] << 8) | word[1];
	 
	 return 1;
}

/*LDC1314��ʼ��*/
//����ֵ��1 ��ʼ���ɹ�
//        0 ��ʼ��ʧ��
unsigned char LDC1314_Init(void)
{
		unsigned char val=1;
	
		LDC_IIC_Init();//��ʼ��IIC
	
		delay_ms(100);
	
		val &= LDC_IIC_WriteWord(LDC1314_IICADDR,LDC1314_RESET_DEV,0x8000);
	
		val &=LDC_IIC_WriteWord(LDC1314_IICADDR,LDC1314_RCOUNT_CH0,0x04D6);
		val &=LDC_IIC_WriteWord(LDC1314_IICADDR,LDC1314_RCOUNT_CH1,0x04D6);
		val &=LDC_IIC_WriteWord(LDC1314_IICADDR,LDC1314_RCOUNT_CH2,0x04D6);
		val &=LDC_IIC_WriteWord(LDC1314_IICADDR,LDC1314_RCOUNT_CH3,0x04D6);
		val &=LDC_IIC_WriteWord(LDC1314_IICADDR,LDC1314_SETTLECOUNT_CH0,0x000A);
		val &=LDC_IIC_WriteWord(LDC1314_IICADDR,LDC1314_SETTLECOUNT_CH1,0x000A);
		val &=LDC_IIC_WriteWord(LDC1314_IICADDR,LDC1314_SETTLECOUNT_CH2,0x000A);
		val &=LDC_IIC_WriteWord(LDC1314_IICADDR,LDC1314_SETTLECOUNT_CH3,0x000A);
		val &=LDC_IIC_WriteWord(LDC1314_IICADDR,LDC1314_CLOCK_DIVIDERS_CH0,0x1002);
	  val &=LDC_IIC_WriteWord(LDC1314_IICADDR,LDC1314_CLOCK_DIVIDERS_CH1,0x1002);
		val &=LDC_IIC_WriteWord(LDC1314_IICADDR,LDC1314_CLOCK_DIVIDERS_CH2,0x1002);
		val &=LDC_IIC_WriteWord(LDC1314_IICADDR,LDC1314_CLOCK_DIVIDERS_CH3,0x1002);
		val &=LDC_IIC_WriteWord(LDC1314_IICADDR,LDC1314_ERROR_CONFIG,0x0000);
		//val &=LDC_IIC_WriteWord(LDC1314_IICADDR,LDC1314_MUX_CONFIG,0x020C);
		val &=LDC_IIC_WriteWord(LDC1314_IICADDR,LDC1314_MUX_CONFIG,0xC20C);//�ĸ�ͨ���ֻ�
		val &=LDC_IIC_WriteWord(LDC1314_IICADDR,LDC1314_DRIVE_CURRENT_CH0,0x9000);
		val &=LDC_IIC_WriteWord(LDC1314_IICADDR,LDC1314_DRIVE_CURRENT_CH1,0x9000);
		val &=LDC_IIC_WriteWord(LDC1314_IICADDR,LDC1314_DRIVE_CURRENT_CH2,0x9000);
		val &=LDC_IIC_WriteWord(LDC1314_IICADDR,LDC1314_DRIVE_CURRENT_CH3,0x9000);
		val &=LDC_IIC_WriteWord(LDC1314_IICADDR,LDC1314_CONFIG,0x1401);//ѡ���ڲ�ʱ��Դ
		
		return val;
}

void LDC1314_ProcessDRDY()
{
	
	  LDC_IIC_ReadWord(LDC1314_IICADDR,LDC1314_DATA_CH0,&allData[0]);//��
//	  LDC_IIC_ReadWord(LDC1314_IICADDR,0x01,&allData[1]);
//	  LDC_IIC_ReadWord(LDC1314_IICADDR,LDC1314_DATA_CH1,&allData[2]);
//	  LDC_IIC_ReadWord(LDC1314_IICADDR,0x03,&allData[3]);
	  LDC_IIC_ReadWord(LDC1314_IICADDR,LDC1314_DATA_CH2,&allData[4]);//��
//	  LDC_IIC_ReadWord(LDC1314_IICADDR,0x05,&allData[5]);
	  LDC_IIC_ReadWord(LDC1314_IICADDR,LDC1314_DATA_CH3,&allData[6]);//��
//	  LDC_IIC_ReadWord(LDC1314_IICADDR,0x07,&allData[7]);
}
