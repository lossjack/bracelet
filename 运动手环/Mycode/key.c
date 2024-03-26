#include "key.h"

/*
�������ܣ�����ʱ
*/
void delay(int ms)
{
	while(ms--)
		for(int i=38400;i>0;i--);//1ms
}


/*
�������ܣ��������ų�ʼ��
*/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIOInitStruct;//����һ��������ų�ʼ����Ϣ�Ľṹ��
	
	//ʹ��ʱ��--����AHB1ENR�Ĵ���
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//PA0
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);//PE2 PE3 PE4  PE13 PE14
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);//PF8-������ PF9 PF10
	
	//����GPIOģ��ĺ����� GPIO_Init()
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_0 ;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_IN;//����ģʽ
	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_UP;//��������
	GPIO_Init(GPIOA,&GPIOInitStruct);
	
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Init(GPIOE,&GPIOInitStruct);

}


/*
�������ܣ�����ɨ�� �õ��ոհ��µ����ĸ�����
����1��LEDn ָ���ĵڼ�յ�� LEDn:1~4
����2��LED_Flag  ָ����״̬ ��-0  ��1
*/
unsigned char KEY_Scan(void)
{
	static unsigned char Key_oldvalue=0x0F;//0000 1111 ��¼��һ�ζ�ȡ�������ŵ�ƽ
	unsigned char Key_pres = 0;//0000 1000
	
	//����1 ��Ӧ����Ϊ�͵�ƽ
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 0 )
	{
		//������һ�μ�¼�ĸ����ŵ�ƽ�Ǹߵ�ƽ
		if(Key_oldvalue & 0x01)
		{
			
			Key_pres |= 0x01;//��¼��ǰ��������
			Key_oldvalue &= ~0x01;//�����ϵİ���״ֵ̬
			
			//��һ�ν���  ��ʱһ��
			delay(40);
		}
	}
	else//��⵽�ߵ�ƽ
	{
		if(!(Key_oldvalue & 0x01))//�������ɿ�һ˲�� 0--->1
		{
			Key_oldvalue |= 0x01;//�����ϵİ���״ֵ̬
			delay(40);
		}
	}
		
	
	//����2 ��Ӧ����Ϊ�͵�ƽ
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) == 0 )
	{
		//������һ�μ�¼�ĸ����ŵ�ƽ�Ǹߵ�ƽ
		if(Key_oldvalue & 0x02)
		{
			Key_pres |= 0x02;//��¼��ǰ��������
			Key_oldvalue &= ~0x02;//�����ϵİ���״ֵ̬
			
			delay(40);
		}
	}
	else//��⵽�ߵ�ƽ
	{
		if(!(Key_oldvalue & 0x02))//�������ɿ�һ˲�� 0--->1
		{
			Key_oldvalue |= 0x02;//�����ϵİ���״ֵ̬
			delay(40);
		}
	}
	
	
	//����1 ��Ӧ����Ϊ�͵�ƽ
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0 )
	{
		//������һ�μ�¼�ĸ����ŵ�ƽ�Ǹߵ�ƽ
		if(Key_oldvalue & 0x04)
		{
			Key_pres |= 0x04;//��¼��ǰ��������
			Key_oldvalue &= ~0x04;//�����ϵİ���״ֵ̬
			delay(40);
		}
	}
	else//��⵽�ߵ�ƽ
	{
		if(!(Key_oldvalue & 0x04))//�������ɿ�һ˲�� 0--->1
		{
			
			Key_oldvalue |= 0x04;//�����ϵİ���״ֵ̬
			delay(40);
		}
	}
	
	
	//����1 ��Ӧ����Ϊ�͵�ƽ
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0 )
	{
		//������һ�μ�¼�ĸ����ŵ�ƽ�Ǹߵ�ƽ
		if(Key_oldvalue & 0x08)
		{
			Key_pres |= 0x08;//��¼��ǰ��������
			Key_oldvalue &= ~0x08;//�����ϵİ���״ֵ̬
			delay(40);
		}
	}
	else//��⵽�ߵ�ƽ
	{
		if(!(Key_oldvalue & 0x08))//�������ɿ�һ˲�� 0--->1
		{
			Key_oldvalue |= 0x08;//�����ϵİ���״ֵ̬
			delay(40);
		}
	}
	

	return Key_pres;
	
}



// NVIC---misc.c
// EXTI---stm32f4xx_exti.c
// SYSCFG---stm32f4xx_syscfg.c
/*
�������ܣ�
*/
void KEY_EXTIInit(void)
{
	EXTI_InitTypeDef EXTIInitStruct;
	NVIC_InitTypeDef NVICInitStruct;
	
	//PA0 PE2 PE3 PE4
	//1��ʹ��ʱ�� 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	//2����������Ϊ����ģʽ
	KEY_Init();
	
	//3����PA0��EXTI0�жϼ����·����
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

	//4����ʼ����EXTIָ�����ж���  EXTI0
	EXTIInitStruct.EXTI_Line = EXTI_Line0;
	EXTIInitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//�ж�ģʽ
	EXTIInitStruct.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���---�������´���
	EXTIInitStruct.EXTI_LineCmd = ENABLE;//����
	EXTI_Init(&EXTIInitStruct);
	
	
	//5��NVIC�ж����ȼ�����
	NVICInitStruct.NVIC_IRQChannel = EXTI0_IRQn;//�жϺ�ѡȡ
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 5;//���ȼ�0~15
	NVICInitStruct.NVIC_IRQChannelSubPriority = 5;//���ȼ�0~15
	NVICInitStruct.NVIC_IRQChannelCmd   =  ENABLE;
	NVIC_Init(&NVICInitStruct);
}




