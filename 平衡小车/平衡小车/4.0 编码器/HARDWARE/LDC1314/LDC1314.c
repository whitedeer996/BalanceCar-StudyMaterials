/*
该程序是LDC1314函数库
单片机：stm32f103
作者：鹅鹅鹅
时间：2018.3.11
*/
#include "LDC1314.h"
#include "LDC1314_cmd.h"
#include "delay.h"

unsigned short allData[8];

/*IIC延时函数*/
void LDC_IIC_Delay(void)
{
	delay_us(5);//实测可以小于5us
}

/*IIC端口初始化*/
void LDC_IIC_Init(void)
{					     
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);     //先使能外设IO PORTC时钟 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;     //端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIO 
	
	GPIO_SetBits(GPIOC,GPIO_Pin_4|GPIO_Pin_5);			     //PC.4，PC.5输出高	
}

/*IIC起始信号*/
void LDC_IIC_Start(void)
{
	LDC_SDA_OUT();     //sda线输出
	LDC_IIC_SDA=1;	  	  
	LDC_IIC_SCL=1;
	LDC_IIC_Delay();   //延时>4.7us
 	LDC_IIC_SDA=0;     //SCL高时，产生下降沿
	LDC_IIC_Delay();   //延时>4us
	LDC_IIC_SCL=0;     //SCL为低时，才允许数据变化
}

/*产生IIC停止信号*/
void LDC_IIC_Stop(void)
{
	LDC_SDA_OUT();     //sda线输出
	LDC_IIC_SCL=0;
	LDC_IIC_SDA=0;
 	LDC_IIC_Delay();   //延时
	LDC_IIC_SCL=1;    
	LDC_IIC_Delay();   //延时>4us
	LDC_IIC_SDA=1;     //SCL高时，产生上升沿
	LDC_IIC_Delay();   //延时>4.7us							   	
}

/*产生ACK应答*/
void LDC_IIC_Ack(void)
{
    LDC_IIC_SCL=0;
    LDC_SDA_OUT();
    LDC_IIC_SDA=0;     //拉低SDA
    LDC_IIC_Delay();   //延时
    LDC_IIC_SCL=1;     //SCL为高时，SDA为低，持续>4us，应答/0
    LDC_IIC_Delay();   //延时>4us
    LDC_IIC_SCL=0;
}

/*产生NACK非应答*/      
void LDC_IIC_NAck(void)
{
    LDC_IIC_SCL=0;
    LDC_SDA_OUT();
    LDC_IIC_SDA=1;     //拉高SDA
    LDC_IIC_Delay();   //延时
    LDC_IIC_SCL=1;     //SCL为高时，SDA为高，持续>4us，非应答/1
    LDC_IIC_Delay();   //延时>4us
    LDC_IIC_SCL=0;
}

/*等待从机应答信号*/
//返回值：1 接收应答成功
//        0 接收应答失败
unsigned char LDC_IIC_Wait_Ack(void)
{
    unsigned char Time=0;
	
		LDC_SDA_IN();          //配置为上拉输入。
    LDC_IIC_SDA=1;         //主机释放数据总线，等待从机产生应答信号
		
    LDC_IIC_Delay();
    LDC_IIC_SCL=1;         //SCL为高时，等待从机应答
    LDC_IIC_Delay();   
  
    while(LDC_Read_SDA)    //SDA拉低时，正确应答
    {
        Time++;
        //软件延时
        if(Time>250)       //如果时间超时，没有应答就停止
				{
					LDC_IIC_Stop();
					return 0;      //没有响应返回0
				} 
	}

    LDC_IIC_SCL=0;         //继续接收数据
    return 1;              //有响应返回1
}

/*IIC发送一个字节*/        
void LDC_IIC_Send_Byte(unsigned char txd)
{                        
    unsigned char t;   
    LDC_SDA_OUT();      
    LDC_IIC_SCL=0;        //拉低时钟开始数据传输
    for(t=0;t<8;t++)      //发送一个字节，从最高位开始发送
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

/*读1个字节，ack=1时，发送Ack，ack=0，发送NAck*/
//返回值：读取到的字节  
unsigned char LDC_IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    LDC_SDA_IN();        //SDA设置为输入
    for(i=0;i<8;i++) //接收一个字节，从高位开始接收
    {
        LDC_IIC_SCL=0; 
        LDC_IIC_Delay(); 
        LDC_IIC_SCL=1;
        receive<<=1;
        if(LDC_Read_SDA) receive++;   
        LDC_IIC_Delay(); 
    }                    
    if (!ack)
        LDC_IIC_NAck();  //发送nACK
    else
        LDC_IIC_Ack();   //发送ACK   
    return receive;
}

/*指定地址写数据*/
//返回值：1 写数据成功
//        0 写数据失败
unsigned char LDC_IIC_WriteWord(unsigned char SlaveAddr, unsigned char RegisterAddr, unsigned short data)
{
	 LDC_IIC_Start();
	 LDC_IIC_Send_Byte((SlaveAddr<<1)|0);//0为写数据
	 if (!LDC_IIC_Wait_Ack())
	 {
		 LDC_IIC_Stop();
		 return 0;//失败
	 }
	 //GPIO_ResetBits(GPIOD,GPIO_Pin_2);	
	 LDC_IIC_Send_Byte(RegisterAddr);
	 LDC_IIC_Wait_Ack();
	 LDC_IIC_Send_Byte((unsigned char)(data>>8));
	 if (!LDC_IIC_Wait_Ack())
	 {
		 LDC_IIC_Stop();
		 return 0;//失败
	 }
	 
	 LDC_IIC_Send_Byte((unsigned char)(data&0xFF));
	 if (!LDC_IIC_Wait_Ack())
	 {
		 LDC_IIC_Stop();
		 return 0;//失败
	 }
	 LDC_IIC_Stop();
	 return 1;//成功
}

/*指定地址读数据*/
//返回值：1 写数据成功
//        0 写数据失败
unsigned char LDC_IIC_ReadWord(unsigned char SlaveAddr, unsigned char RegisterAddr, unsigned short* data)
{
	 unsigned short word[2] = {0};
	 LDC_IIC_Start();
	 LDC_IIC_Send_Byte(SlaveAddr << 1 | 0);//0为写数据
	 
	 if (!LDC_IIC_Wait_Ack())
	 {
		 LDC_IIC_Stop();
		 return 0;//失败
	 }
	 
	 LDC_IIC_Send_Byte(RegisterAddr);
	 LDC_IIC_Wait_Ack();

	 LDC_IIC_Start();
	 LDC_IIC_Send_Byte((SlaveAddr <<1)|1);//1为读数据
	 LDC_IIC_Wait_Ack();
	 
	 word[0] = LDC_IIC_Read_Byte(1);//高字节
	 word[1] = LDC_IIC_Read_Byte(0);//低字节
	 LDC_IIC_Stop();
	 
	 *data = (word[0] << 8) | word[1];
	 
	 return 1;
}

/*LDC1314初始化*/
//返回值：1 初始化成功
//        0 初始化失败
unsigned char LDC1314_Init(void)
{
		unsigned char val=1;
	
		LDC_IIC_Init();//初始化IIC
	
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
		val &=LDC_IIC_WriteWord(LDC1314_IICADDR,LDC1314_MUX_CONFIG,0xC20C);//四个通道轮换
		val &=LDC_IIC_WriteWord(LDC1314_IICADDR,LDC1314_DRIVE_CURRENT_CH0,0x9000);
		val &=LDC_IIC_WriteWord(LDC1314_IICADDR,LDC1314_DRIVE_CURRENT_CH1,0x9000);
		val &=LDC_IIC_WriteWord(LDC1314_IICADDR,LDC1314_DRIVE_CURRENT_CH2,0x9000);
		val &=LDC_IIC_WriteWord(LDC1314_IICADDR,LDC1314_DRIVE_CURRENT_CH3,0x9000);
		val &=LDC_IIC_WriteWord(LDC1314_IICADDR,LDC1314_CONFIG,0x1401);//选择内部时钟源
		
		return val;
}

void LDC1314_ProcessDRDY()
{
	
	  LDC_IIC_ReadWord(LDC1314_IICADDR,LDC1314_DATA_CH0,&allData[0]);//左
//	  LDC_IIC_ReadWord(LDC1314_IICADDR,0x01,&allData[1]);
//	  LDC_IIC_ReadWord(LDC1314_IICADDR,LDC1314_DATA_CH1,&allData[2]);
//	  LDC_IIC_ReadWord(LDC1314_IICADDR,0x03,&allData[3]);
	  LDC_IIC_ReadWord(LDC1314_IICADDR,LDC1314_DATA_CH2,&allData[4]);//中
//	  LDC_IIC_ReadWord(LDC1314_IICADDR,0x05,&allData[5]);
	  LDC_IIC_ReadWord(LDC1314_IICADDR,LDC1314_DATA_CH3,&allData[6]);//右
//	  LDC_IIC_ReadWord(LDC1314_IICADDR,0x07,&allData[7]);
}
