#include "McuDataSend.h"
#include "usart.h"

/*1：发送基本信息（姿态、锁定状态）*/
void Data_Send_Status(short roll,short pitch,short yaw)
{
	u8 data_to_send[16];
	u8 _cnt=0;
	u8 sum = 0;
	u8 i;	vs16 _temp;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x01;
	data_to_send[_cnt++]=0;

	_temp = (int)(roll*100);
	data_to_send[_cnt++]=(_temp>>8)&0XFF;
	data_to_send[_cnt++]=_temp&0XFF;
	_temp = (int)(pitch*100);
	data_to_send[_cnt++]=(_temp>>8)&0XFF;
	data_to_send[_cnt++]=_temp&0XFF;
	_temp = (int)(yaw*100);
	data_to_send[_cnt++]=(_temp>>8)&0XFF;
	data_to_send[_cnt++]=_temp&0XFF;
//	vs32 _temp2 = 0X00;
	data_to_send[_cnt++]=0X00;
	data_to_send[_cnt++]=0X00;
	data_to_send[_cnt++]=0X00;
	data_to_send[_cnt++]=0X00;
	
  data_to_send[_cnt++]=0X00; //FLY MODEL	
	//if(Rc_C.ARMED==0)		
	data_to_send[_cnt++]=0xA0;	
	//else if(Rc_C.ARMED==1)		data_to_send[_cnt++]=0xA1;
	
	data_to_send[3] = _cnt-4;
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;

  for(i=0;i<_cnt;i++)
//	printc(data_to_send[i]);
		{
				USART_SendData(USART1,data_to_send[i]);				
				while((USART1->SR&0X40)==0);
		}
//	printf("%c",data_to_send[i]);
}

//2：发送传感器数据
void Data_Send_Senser(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz)
{
	u8 data_to_send[24];
	u8 _cnt=0;	
	u8 sum = 0;
	u8 i;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x02;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=(aacx>>8)&0XFF;  //高8位
	data_to_send[_cnt++]=aacx&0XFF;  //低8位
	data_to_send[_cnt++]=(aacy>>8)&0XFF;  //高8位
	data_to_send[_cnt++]=aacy&0XFF;  //低8位
	data_to_send[_cnt++]=(aacz>>8)&0XFF;  //高8位
	data_to_send[_cnt++]=aacz&0XFF;  //低8位
	data_to_send[_cnt++]=(gyrox>>8)&0XFF;  //高8位
	data_to_send[_cnt++]=gyrox&0XFF;  //低8位
	data_to_send[_cnt++]=(gyroy>>8)&0XFF;  //高8位
	data_to_send[_cnt++]=gyroy&0XFF;  //低8位
	data_to_send[_cnt++]=(gyroz>>8)&0XFF;  //高8位
	data_to_send[_cnt++]=gyroz&0XFF;  //低8位
	
	data_to_send[3] = _cnt-4;
	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;

	for(i=0;i<_cnt;i++)
//	printc(data_to_send[i]);
	printf("%c",data_to_send[i]);
}

 	