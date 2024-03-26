#include "usart.h"

#define TRIG_CLK  RCC_AHB1Periph_GPIOA
#define TRIG_G    GPIOA
#define TRIG_P    GPIO_Pin_2

#define ECHO_CLK  RCC_AHB1Periph_GPIOA
#define ECHO_G    GPIOA
#define ECHO_P    GPIO_Pin_3


void USART1_Config(void)
{
	GPIO_InitTypeDef GPIOInitStruct;
	USART_InitTypeDef USARTInitStruct;
	NVIC_InitTypeDef NVICInitStruct;
	
	
	RCC_AHB1PeriphClockCmd(TRIG_CLK,ENABLE);//PE2 PE3 PE4  PE13 PE14
	RCC_AHB1PeriphClockCmd(ECHO_CLK,ENABLE);
	//ʹ��ʱ�� GPIOA USART1
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	//����PA9  PA10Ϊ���ù���
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_9 | GPIO_Pin_10;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_AF;//���ģʽ
	GPIOInitStruct.GPIO_OType  = GPIO_OType_PP;//�������
	GPIOInitStruct.GPIO_Speed  = GPIO_Low_Speed;//����
	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_UP;//��������
	GPIO_Init(GPIOA,&GPIOInitStruct);
	//PB10  PB11
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOB,&GPIOInitStruct);
	
	//����GPIOģ��ĺ����� GPIO_Init()
	GPIOInitStruct.GPIO_Pin    = TRIG_P ;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_AF;//���ģʽ
	GPIOInitStruct.GPIO_OType  = GPIO_OType_PP;//�������
	GPIOInitStruct.GPIO_Speed  = GPIO_Low_Speed;//����
	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_UP;//��������
	GPIO_Init(TRIG_G,&GPIOInitStruct);

	GPIOInitStruct.GPIO_Pin    = ECHO_P;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_AF;//����ģʽ 
	GPIO_Init(ECHO_G,&GPIOInitStruct);

	
	//ָ��PA9 PA10����ΪUSART1����
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);
	
	//����USART1ģ��  ������  ͨ��Э�飨����λ ֹͣλ У��λ  ���أ�
	USARTInitStruct.USART_BaudRate   = 9600;
	USARTInitStruct.USART_WordLength = USART_WordLength_8b;//8bit�ֳ� һ֡������Ч����1byte
	USARTInitStruct.USART_StopBits   = USART_StopBits_1;//ֹͣλΪ1
	USARTInitStruct.USART_Parity     = USART_Parity_No;//����Ҫ��żУ��λ
	USARTInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//����ҪӲ������
	USARTInitStruct.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;//�շ�ģʽ
	USART_Init(USART2,&USARTInitStruct);
	
	USARTInitStruct.USART_BaudRate   = 115200;
	USART_Init(USART1,&USARTInitStruct);
	
	USARTInitStruct.USART_BaudRate   = 115200;
	USART_Init(USART3,&USARTInitStruct);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
	
	NVICInitStruct.NVIC_IRQChannel = USART2_IRQn;//�жϺ�ѡȡ
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 5;//���ȼ�0~15
	NVICInitStruct.NVIC_IRQChannelSubPriority = 5;//���ȼ�0~15
	NVICInitStruct.NVIC_IRQChannelCmd   =  ENABLE;
	NVIC_Init(&NVICInitStruct);
	
	NVICInitStruct.NVIC_IRQChannel = USART1_IRQn;//�жϺ�ѡȡ
	NVIC_Init(&NVICInitStruct);
	
	NVICInitStruct.NVIC_IRQChannel = USART3_IRQn;//�жϺ�ѡȡ
	NVIC_Init(&NVICInitStruct);
	
	
	//��������
	USART_Cmd(USART1,ENABLE);
	USART_Cmd(USART2,ENABLE);
	USART_Cmd(USART3,ENABLE);
	
	
}


void USART1_SendString(char *str)
{
	//1�����������ת��Ϊ�ַ���
	
	
	//2�����ַ�����ָ������
	while(*str != '\0')
	{
		USART1->DR = *str;//д��DR�Ὣ��־λTXE����
		str++;
		while(!(USART1->SR & (0x01<<7)));//�ȴ�TXE��־λ��1 ��ʾTDR�����Ѿ����䵽��λ�Ĵ�����
	}
}


void USART2_SendString(char *str)
{
	//1�����������ת��Ϊ�ַ���
	
	
	//2�����ַ�����ָ������
	while(*str != '\0')
	{
		USART2->DR = *str;//д��DR�Ὣ��־λTXE����
		str++;
		while(!(USART2->SR & (0x01<<7)));//�ȴ�TXE��־λ��1 ��ʾTDR�����Ѿ����䵽��λ�Ĵ�����
	}
}

void USART3_SendString(char *str)
{
	//1�����������ת��Ϊ�ַ���
	
	
	//2�����ַ�����ָ������
	while(*str != '\0')
	{
		USART2->DR = *str;//д��DR�Ὣ��־λTXE����
		str++;
		while(!(USART2->SR & (0x01<<7)));//�ȴ�TXE��־λ��1 ��ʾTDR�����Ѿ����䵽��λ�Ĵ�����
	}
}


void USART3_IRQHandler(void)
{
	
	unsigned char RData;
	if(USART2->SR & (0x01<<5))
	{
		
		//��ȡDR
		RData = USART2->DR;
	
		if(RData == '1')
		{
			OLED_Fill(0xFF);
			OLED_ON();
			LED_Control(2,1);
		}
		else if(RData == '2')
		{
			OLED_Fill(0x00);
			OLED_OFF();
			LED_Control(2,0);
		}
	
	}
}        






void USART1_IRQHandler(void)
{
	unsigned char RData;
	
	if(USART1->SR & (0x01<<5))
	{
		//��ȡDR
		RData = USART1->DR;
		if(RData == 0)
		{
			LED_Control(1,1);
		}
		else if(RData == 1)
		{
			LED_Control(1,0);
		}
	
	}
}



//int fputc(int ch, FILE * fp)
//{
//	USART1->DR = (unsigned char)ch;
//	while(!(USART1->SR & (0x01<<7)));//��TXE��־λ��1  ��ʾ���ݸմ�TDR��ȥ��
//	return ch;
//}

//int fgetc(FILE * fp)
//{
//	while(!(USART1->SR & (0x01<<5)));//�ȴ�RXNE��־λ��λ
//	return USART1->DR;
//}



//��дprintf��������õĵײ�����ַ�����
//��������ᱻprintf�ײ�����ַ��������ظ�����
//�����������һ�ξͷ���1���ַ�
int fputc(int ch, FILE * fp)
{
	USART1->DR = (unsigned char)ch;
	while(!(USART1->SR & (0x01<<7)));//��TXE��־λ��1  ��ʾ���ݸմ�TDR��ȥ��
	return ch;
}

int fgetc(FILE * fp)
{
	while(!(USART2->SR & (0x01<<5)));//�ȴ�RXNE��־λ��λ
	return USART2->DR;
}

