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
	//使能时钟 GPIOA USART1
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	//配置PA9  PA10为复用功能
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_9 | GPIO_Pin_10;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_AF;//输出模式
	GPIOInitStruct.GPIO_OType  = GPIO_OType_PP;//推挽输出
	GPIOInitStruct.GPIO_Speed  = GPIO_Low_Speed;//低速
	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_UP;//上拉输入
	GPIO_Init(GPIOA,&GPIOInitStruct);
	//PB10  PB11
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOB,&GPIOInitStruct);
	
	//配置GPIO模块的函数： GPIO_Init()
	GPIOInitStruct.GPIO_Pin    = TRIG_P ;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_AF;//输出模式
	GPIOInitStruct.GPIO_OType  = GPIO_OType_PP;//推挽输出
	GPIOInitStruct.GPIO_Speed  = GPIO_Low_Speed;//低速
	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_UP;//上拉输入
	GPIO_Init(TRIG_G,&GPIOInitStruct);

	GPIOInitStruct.GPIO_Pin    = ECHO_P;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_AF;//输入模式 
	GPIO_Init(ECHO_G,&GPIOInitStruct);

	
	//指定PA9 PA10复用为USART1功能
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);
	
	//配置USART1模块  波特率  通信协议（数据位 停止位 校验位  流控）
	USARTInitStruct.USART_BaudRate   = 9600;
	USARTInitStruct.USART_WordLength = USART_WordLength_8b;//8bit字长 一帧数据有效数据1byte
	USARTInitStruct.USART_StopBits   = USART_StopBits_1;//停止位为1
	USARTInitStruct.USART_Parity     = USART_Parity_No;//不需要奇偶校验位
	USARTInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//不需要硬件流控
	USARTInitStruct.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;//收发模式
	USART_Init(USART2,&USARTInitStruct);
	
	USARTInitStruct.USART_BaudRate   = 115200;
	USART_Init(USART1,&USARTInitStruct);
	
	USARTInitStruct.USART_BaudRate   = 115200;
	USART_Init(USART3,&USARTInitStruct);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
	
	NVICInitStruct.NVIC_IRQChannel = USART2_IRQn;//中断号选取
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 5;//优先级0~15
	NVICInitStruct.NVIC_IRQChannelSubPriority = 5;//优先级0~15
	NVICInitStruct.NVIC_IRQChannelCmd   =  ENABLE;
	NVIC_Init(&NVICInitStruct);
	
	NVICInitStruct.NVIC_IRQChannel = USART1_IRQn;//中断号选取
	NVIC_Init(&NVICInitStruct);
	
	NVICInitStruct.NVIC_IRQChannel = USART3_IRQn;//中断号选取
	NVIC_Init(&NVICInitStruct);
	
	
	//开启串口
	USART_Cmd(USART1,ENABLE);
	USART_Cmd(USART2,ENABLE);
	USART_Cmd(USART3,ENABLE);
	
	
}


void USART1_SendString(char *str)
{
	//1处理各类数据转换为字符串
	
	
	//2发送字符串到指定串口
	while(*str != '\0')
	{
		USART1->DR = *str;//写入DR会将标志位TXE清零
		str++;
		while(!(USART1->SR & (0x01<<7)));//等待TXE标志位置1 表示TDR数据已经传输到移位寄存器了
	}
}


void USART2_SendString(char *str)
{
	//1处理各类数据转换为字符串
	
	
	//2发送字符串到指定串口
	while(*str != '\0')
	{
		USART2->DR = *str;//写入DR会将标志位TXE清零
		str++;
		while(!(USART2->SR & (0x01<<7)));//等待TXE标志位置1 表示TDR数据已经传输到移位寄存器了
	}
}

void USART3_SendString(char *str)
{
	//1处理各类数据转换为字符串
	
	
	//2发送字符串到指定串口
	while(*str != '\0')
	{
		USART2->DR = *str;//写入DR会将标志位TXE清零
		str++;
		while(!(USART2->SR & (0x01<<7)));//等待TXE标志位置1 表示TDR数据已经传输到移位寄存器了
	}
}


void USART3_IRQHandler(void)
{
	
	unsigned char RData;
	if(USART2->SR & (0x01<<5))
	{
		
		//读取DR
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
		//读取DR
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
//	while(!(USART1->SR & (0x01<<7)));//等TXE标志位置1  表示数据刚从TDR出去完
//	return ch;
//}

//int fgetc(FILE * fp)
//{
//	while(!(USART1->SR & (0x01<<5)));//等待RXNE标志位置位
//	return USART1->DR;
//}



//重写printf函数会调用的底层输出字符函数
//这个函数会被printf底层根据字符串长度重复调用
//这个函数调用一次就发送1个字符
int fputc(int ch, FILE * fp)
{
	USART1->DR = (unsigned char)ch;
	while(!(USART1->SR & (0x01<<7)));//等TXE标志位置1  表示数据刚从TDR出去完
	return ch;
}

int fgetc(FILE * fp)
{
	while(!(USART2->SR & (0x01<<5)));//等待RXNE标志位置位
	return USART2->DR;
}

