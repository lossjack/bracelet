#include "timer.h"

void TIM14_Init(void)
{
	NVIC_InitTypeDef NVICInitStruct;
	
	//1、使能时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);
	
	
	//2、时基单元配置
	TIM14->PSC = 8400-1;//84Mhz/8400 = 10000Hz T= 0.1ms
	TIM14->ARR = 1000-1; //定时100ms  因为开启定时器后 会等到1个时钟周期后，再从0开始
						  //所以CNT==0时，其实已经计了1个脉冲
						  //最终要计N个脉冲CNT计到N-1个即可
	TIM14->CNT = 0;//CNT从0开始累计
	
//	SysTick->LOAD
//	SysTick->VAL
	
	//开启中断 UIE  Update Interrupt ENABLE
	//TIM14->CR1 |= 0x01<<2;
	TIM14->DIER |= 0x01;
	//TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE);
	
	NVICInitStruct.NVIC_IRQChannel = TIM8_TRG_COM_TIM14_IRQn;//中断号选取
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 5;//优先级0~15
	NVICInitStruct.NVIC_IRQChannelSubPriority = 5;//优先级0~15
	NVICInitStruct.NVIC_IRQChannelCmd   =  ENABLE;
	NVIC_Init(&NVICInitStruct);
	
	TIM14->CR1 |= 0x01;//开启定时器14
	
}

void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
	static unsigned int num=0;
	//如果多个中断源公用一个中断号和中断服务函数，需要先判断标志
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
		
		TIM14->SR &= ~0x01;//清标志位  写0清零
	}
}



void TIM14_PWMOuteInit(void)
{
	GPIO_InitTypeDef GPIOInitStruct;
	TIM_TimeBaseInitTypeDef TIMInitStruct;
	TIM_OCInitTypeDef  TIMOCInitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);
	
	//1、配置GPIO
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_9;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_AF;//输出模式
	GPIOInitStruct.GPIO_OType  = GPIO_OType_PP;//推挽输出
	GPIOInitStruct.GPIO_Speed  = GPIO_Low_Speed;//低速 2Mhz
	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOF,&GPIOInitStruct);
	
	//将指定引脚复用为指定功能
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource9, GPIO_AF_TIM14);
	
	
	//2、配置TIM14时基单元
	TIMInitStruct.TIM_Prescaler   = 84-1;//PSC 84Mhz/84   T=1us
	TIMInitStruct.TIM_Period      = 5000-1;//ARR 定时器周期 5ms
	TIMInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//递增计数 其实TIM14不用
	TIM_TimeBaseInit(TIM14,&TIMInitStruct);
	
	
	//3、配置TIM14 输出比较模块
	TIMOCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;//输出比较模式 PWM1 左边有效右边无效   PWM2左边无效右边有效
	TIMOCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;//输出极性  High low
	TIMOCInitStruct.TIM_Pulse = 0;//CCR 初始化为0  灯灭
	TIMOCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//输出比较使能 
	TIM_OC1Init(TIM14,&TIMOCInitStruct);
	
	
	//4、开启TIM14
	//TIM14->CR1 |= 0x01;
	TIM_Cmd(TIM14,ENABLE);
}

