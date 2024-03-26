#include "timer.h"

void TIM14_Init(void)
{
	NVIC_InitTypeDef NVICInitStruct;
	
	//1��ʹ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);
	
	
	//2��ʱ����Ԫ����
	TIM14->PSC = 8400-1;//84Mhz/8400 = 10000Hz T= 0.1ms
	TIM14->ARR = 1000-1; //��ʱ100ms  ��Ϊ������ʱ���� ��ȵ�1��ʱ�����ں��ٴ�0��ʼ
						  //����CNT==0ʱ����ʵ�Ѿ�����1������
						  //����Ҫ��N������CNT�Ƶ�N-1������
	TIM14->CNT = 0;//CNT��0��ʼ�ۼ�
	
//	SysTick->LOAD
//	SysTick->VAL
	
	//�����ж� UIE  Update Interrupt ENABLE
	//TIM14->CR1 |= 0x01<<2;
	TIM14->DIER |= 0x01;
	//TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE);
	
	NVICInitStruct.NVIC_IRQChannel = TIM8_TRG_COM_TIM14_IRQn;//�жϺ�ѡȡ
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 5;//���ȼ�0~15
	NVICInitStruct.NVIC_IRQChannelSubPriority = 5;//���ȼ�0~15
	NVICInitStruct.NVIC_IRQChannelCmd   =  ENABLE;
	NVIC_Init(&NVICInitStruct);
	
	TIM14->CR1 |= 0x01;//������ʱ��14
	
}

void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
	static unsigned int num=0;
	//�������ж�Դ����һ���жϺź��жϷ���������Ҫ���жϱ�־
	if(TIM14->SR & 0x01)
	{
		//GPIO_ToggleBits(GPIOE,GPIO_Pin_14);
		num++;
		if(num%2 == 0)
		{
			GPIO_ToggleBits(GPIOF,GPIO_Pin_9);
		}
		if(num%5 == 0)
		{
			GPIO_ToggleBits(GPIOF,GPIO_Pin_10);
		}
		if(num%8 == 0)
		{
			GPIO_ToggleBits(GPIOE,GPIO_Pin_13);
		}
		if(num%12 == 0)
		{
			GPIO_ToggleBits(GPIOE,GPIO_Pin_14);
		}
		
		TIM14->SR &= ~0x01;//���־λ  д0����
	}
}



void TIM14_PWMOuteInit(void)
{
	GPIO_InitTypeDef GPIOInitStruct;
	TIM_TimeBaseInitTypeDef TIMInitStruct;
	TIM_OCInitTypeDef  TIMOCInitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);
	
	//1������GPIO
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_9;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_AF;//���ģʽ
	GPIOInitStruct.GPIO_OType  = GPIO_OType_PP;//�������
	GPIOInitStruct.GPIO_Speed  = GPIO_Low_Speed;//���� 2Mhz
	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOF,&GPIOInitStruct);
	
	//��ָ�����Ÿ���Ϊָ������
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);
	
	
	//2������TIM14ʱ����Ԫ
	TIMInitStruct.TIM_Prescaler   = 84-1;//PSC 84Mhz/84   T=1us
	TIMInitStruct.TIM_Period      = 5000-1;//ARR ��ʱ������ 5ms
	TIMInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//�������� ��ʵTIM14����
	TIM_TimeBaseInit(TIM14,&TIMInitStruct);
	
	
	//3������TIM14 ����Ƚ�ģ��
	TIMOCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;//����Ƚ�ģʽ PWM1 �����Ч�ұ���Ч   PWM2�����Ч�ұ���Ч
	TIMOCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;//�������  High low
	TIMOCInitStruct.TIM_Pulse = 0;//CCR ��ʼ��Ϊ0  ����
	TIMOCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//����Ƚ�ʹ�� 
	TIM_OC1Init(TIM14,&TIMOCInitStruct);
	
	
	//4������TIM14
	//TIM14->CR1 |= 0x01;
	TIM_Cmd(TIM14,ENABLE);
}

