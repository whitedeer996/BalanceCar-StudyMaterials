/*
该函数是OLED函数库
根据SSD1306略有修改
单片机：stm32f103
作者：鹅鹅鹅
时间：2018.3.6
*/
#include "OLED.h"
#include "delay.h"
#include "codetab.h"

/*IIC延时函数*/
void IIC_Delay(void)
{
	delay_us(1);//实测可以小于5us
}

/*IIC端口初始化*/
void IIC_Init(void)
{					     
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);     //先使能外设IO PORTB时钟 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;     //端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIO 
	
	GPIO_SetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9);			     //PB.8,PB.9输出高	
}

/*IIC起始信号*/
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	IIC_Delay();   //延时>4.7us
 	IIC_SDA=0;     //SCL高时，产生下降沿
	IIC_Delay();   //延时>4us
	IIC_SCL=0;     //SCL为低时，才允许数据变化
}

/*产生IIC停止信号*/
void IIC_Stop(void)
{
	SDA_OUT();     //sda线输出
	IIC_SCL=0;
	IIC_SDA=0;
 	IIC_Delay();   //延时
	IIC_SCL=1;    
	IIC_Delay();   //延时>4us
	IIC_SDA=1;     //SCL高时，产生上升沿
	IIC_Delay();   //延时>4.7us							   	
}

/*产生ACK应答*/
void IIC_Ack(void)
{
    IIC_SCL=0;
    SDA_OUT();
    IIC_SDA=0;     //拉低SDA
    IIC_Delay();   //延时
    IIC_SCL=1;     //SCL为高时，SDA为低，持续>4us，应答/0
    IIC_Delay();   //延时>4us
    IIC_SCL=0;
}

/*产生NACK非应答*/      
void IIC_NAck(void)
{
    IIC_SCL=0;
    SDA_OUT();
    IIC_SDA=1;     //拉高SDA
    IIC_Delay();   //延时
    IIC_SCL=1;     //SCL为高时，SDA为高，持续>4us，非应答/1
    IIC_Delay();   //延时>4us
    IIC_SCL=0;
}

/*等待从机应答信号*/
//返回值：1 接收应答失败
//        0 接收应答成功
unsigned char IIC_Wait_Ack(void)
{
    unsigned char Time=0;
 
    SDA_IN();          //配置为上拉输入。
    IIC_SDA=1;         //主机释放数据总线，等待从机产生应答信号
    IIC_Delay();
    IIC_SCL=1;         //SCL为高时，等待从机应答
    IIC_Delay();       
    while(Read_SDA)    //SDA拉低时，正确应答
    {
        Time++;
        //软件延时
        if(Time>250)   //如果时间超时，没有应答就停止
		{
			IIC_Stop();
			return 1;  //没有响应返回1
		} 
	}
    IIC_SCL=0;        //继续接收数据
    return 0;         //有响应返回0
}

/*IIC发送一个字节*/        
void IIC_Send_Byte(unsigned char txd)
{                        
    unsigned char t;   
    SDA_OUT();      
    IIC_SCL=0;        //拉低时钟开始数据传输
    for(t=0;t<8;t++)  //发送一个字节，从最高位开始发送
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

/*读1个字节，ack=1时，发送Ack，ack=0，发送NAck*/
//返回值：读取到的字节  
unsigned char IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA_IN();        //SDA设置为输入
    for(i=0;i<8;i++) //接收一个字节，从高位开始接收
    {
        IIC_SCL=0; 
        IIC_Delay(); 
        IIC_SCL=1;
        receive<<=1;
        if(Read_SDA) receive++;   
        IIC_Delay(); 
    }                    
    if (!ack)
        IIC_NAck();  //发送nACK
    else
        IIC_Ack();   //发送ACK   
    return receive;
}

/*写命令*/
void WriteCmd(unsigned char IIC_Command)
{
	IIC_Start();
	IIC_Send_Byte(0x78);                   //D/C脚接低时，ssd1306的从属地址为0x78
	IIC_Wait_Ack();                        //等待从机应答
	IIC_Send_Byte(0x00);                   //以下仅传输数据，传输的数据为命令
	IIC_Wait_Ack();
	IIC_Send_Byte(IIC_Command);
	IIC_Wait_Ack();
	IIC_Stop();
}

/*写数据*/
void WriteDat(unsigned char IIC_Data)
{
	IIC_Start();
	IIC_Send_Byte(0x78);                   //D/C脚接低时，ssd1306的从属地址为0x78
	IIC_Wait_Ack();                        //等待从机应答
	IIC_Send_Byte(0x40);                   //以下仅传输数据，传输的数据将被储存到GDDRAM
	IIC_Wait_Ack();
	IIC_Send_Byte(IIC_Data);
	IIC_Wait_Ack();
	IIC_Stop();
}

/*初始化OLED*/
void OLED_Init(void)
{
	IIC_Init();   //I2C初始化 
	delay_ms(100);//给予一定的延时

	WriteCmd(0xA8);//设置驱动路数
	WriteCmd(0x3F);//默认0X3F(1/64)

	WriteCmd(0xD3);//设置显示偏移
	WriteCmd(0x00);//默认为0

	WriteCmd(0x40);//设置显示开始行 [5:0],行数.

	WriteCmd(0xA1);//段重定义设置,bit0:0,0->0;1,0->127;

	WriteCmd(0xC8);//设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数

	WriteCmd(0xDA);//设置COM硬件引脚配置
	WriteCmd(0x12);//[5:4]配置

	WriteCmd(0x81);//对比度设置
	WriteCmd(0x7F);//1~255;默认0X7F (亮度设置,越大越亮)

	WriteCmd(0xA4);//全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)

	WriteCmd(0xA6);//设置显示方式;bit0:1,反相显示;0,正常显示

	WriteCmd(0xD5);//设置时钟分频因子,震荡频率
	WriteCmd(0x80);//[3:0],分频因子;[7:4],震荡频率

	WriteCmd(0x8D);//电荷泵设置
	WriteCmd(0x14); //bit2，开启/关闭

	WriteCmd(0xAF); //开启显示	 
}

/*设置起始点坐标*/
void OLED_SetPos(unsigned char x, unsigned char y)
{ 
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)|0x01);
}

/*全屏填充*/
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

/*清屏*/
void OLED_CLS(void)
{
	OLED_Fill(0x00);
}

/*OLED唤醒*/
void OLED_ON(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X14);  //开启电荷泵
	WriteCmd(0XAF);  //OLED唤醒
}

/*OLED休眠*/
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X10);  //关闭电荷泵
	WriteCmd(0XAE);  
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
// Calls          : 
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); ch[] -- 要显示的字符串; TextSize -- 字符大小(1:6*8 ; 2:8*16)
// Description    : 显示codetab.h中的ASCII字符,有6*8和8*16可选择
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
// Parameters     : x,y -- 起始点坐标(x:0~127, y:0~7); N:汉字在codetab.h中的索引
// Description    : 显示codetab.h中的汉字,16*16点阵
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

/*显示数字*/
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
	
/*显示数字串*/
void OLED_ShowNum(unsigned char x, unsigned char y, u16 num, unsigned char TextSize)
 {
	int i=0;
	int  n;
	int a[4];   //最长为五位数
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
/*自定义UI界面*/
void UI(void)
{
	
		OLED_ShowOneNum(0,0,79,2);   //“601”
		OLED_ShowOneNum(8,0,80,2);
		OLED_ShowOneNum(16,0,81,2);
	  
	  OLED_ShowCN(24,0,1);       //“创新电子”
		OLED_ShowCN(40,0,2);
		OLED_ShowCN(56,0,3);
		OLED_ShowCN(72,0,4);
	 
		OLED_ShowCN(111,0,0);      //16x16 logo
	
	  OLED_ShowStr(6,4,"kp:",1);//pid参数
		OLED_ShowStr(6,5,"ki:",1);
		OLED_ShowStr(6,6,"kd:",1);

		OLED_ShowStr(63,4,"TA:",1);//目标值A为角度，V为速度
	  OLED_ShowStr(63,5,"TV:",1);	
	
		OLED_ShowStr(63,6,"Da:",1);//串口接收的数据
	
 //   OLED_ShowStr(36,7,"QQ:1103298792",1);	

}

