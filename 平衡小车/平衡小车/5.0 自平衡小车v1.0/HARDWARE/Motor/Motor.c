/*
�ú����ǵ���ĺ�����
��Ƭ����stm32f103
���ߣ�����
ʱ�䣺2018.3.21
*/
#include "Motor.h"

/*��ʱ��1��ʼ��*/
//��ʱ��1�������PWM�Կ��Ƶ��
//�˴�������Ƕ�ʱ��1ͨ��1��ͨ��4
void TIM1_Init(u16 arr,u16 psc)
{  
		/*����ṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		
	  /*ʹ��TIM1ʱ�Ӻ�GPIOA�˿�ʱ��*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);	//ʹ�ܶ�ʱ��1ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE); //ʹ��GPIOA/Cʱ��
		
	  /*��������*/
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_8; //TIM1_CH4&CH1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO���ٶ�Ϊ50MHz
		GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
		GPIO_ResetBits(GPIOA,GPIO_Pin_11|GPIO_Pin_8);//��0
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				       //�˿�����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		   //�������
		GPIO_Init(GPIOA, &GPIO_InitStructure);					         //�����趨������ʼ��GPIOD
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);						           //PA3��0
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;	 //�˿�����
		GPIO_Init(GPIOC, &GPIO_InitStructure);					         //�����趨������ʼ��GPIOD
		GPIO_ResetBits(GPIOC,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);					 //PC10��PC11��0
	 
		/*��ʼ��TIM1*/
		TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
		TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
		TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
		
	  /*����˿�����*/
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
		TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 OC4
		TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 OC1
		
		/*���ʹ��*/
		TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR2�ϵ�Ԥװ�ؼĴ���
		TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR2�ϵ�Ԥװ�ؼĴ���
		TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
		TIM_CtrlPWMOutputs(TIM1, ENABLE);//�����ʹ�ܣ��߼���ʱ�����еģ����pwm�����
}

/*TB6612ʹ��*/
void Motor_En(void)
{
		MOTOR_EN=1;
}

/*TB6612ʧ��*/
void Motor_Stop(void)
{
		MOTOR_EN=0;
}

/*�ҵ����ת*/
void RMotor_TP(u16 duty)
{
		PCout(10)=1;
		PCout(11)=0;
		TIM_SetCompare4(TIM1,duty);//��ʱ��1ͨ��4���duty/1000ռ�ձ�
}

/*�ҵ����ת*/
void RMotor_TN(u16 duty)
{
		PCout(10)=0;
		PCout(11)=1;
		TIM_SetCompare4(TIM1,duty);//��ʱ��1ͨ��4���duty/1000ռ�ձ�
}

/*������ת*/
void LMotor_TP(u16 duty)
{
		PCout(8)=1;
		PCout(9)=0;
		TIM_SetCompare1(TIM1,duty);//��ʱ��1ͨ��1���duty/1000ռ�ձ�
}

/*������ת*/
void LMotor_TN(u16 duty)
{
		PCout(8)=0;
		PCout(9)=1;
		TIM_SetCompare1(TIM1,duty);//��ʱ��1ͨ��1���duty/1000ռ�ձ�
}

/*�ҵ������*/
void RMotor_Ctr(s16 duty)
{
		MOTOR_EN=1;
		if(duty>=0)
			RMotor_TP(duty);
		else
			RMotor_TN(-duty);
}

/*��������*/
void LMotor_Ctr(s16 duty)
{
		MOTOR_EN=1;
		if(duty>=0)
			LMotor_TP(duty);
		else
			LMotor_TN(-duty);
}
