#include "key.h"

/*
函数功能：粗延时
*/
void delay(int ms)
{
	while(ms--)
		for(int i=38400;i>0;i--);//1ms
}


/*
函数功能：按键引脚初始化
*/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIOInitStruct;//定义一个存放引脚初始化信息的结构体
	
	//使能时钟--——AHB1ENR寄存器
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//PA0
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);//PE2 PE3 PE4  PE13 PE14
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);//PF8-蜂鸣器 PF9 PF10
	
	//配置GPIO模块的函数： GPIO_Init()
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_0 ;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_IN;//输入模式
	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_UP;//上拉输入
	GPIO_Init(GPIOA,&GPIOInitStruct);
	
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Init(GPIOE,&GPIOInitStruct);

}


/*
函数功能：按键扫描 得到刚刚按下的是哪个按键
参数1：LEDn 指定的第几盏灯 LEDn:1~4
参数2：LED_Flag  指定的状态 亮-0  灭：1
*/
unsigned char KEY_Scan(void)
{
	static unsigned char Key_oldvalue=0x0F;//0000 1111 记录上一次读取到的引脚电平
	unsigned char Key_pres = 0;//0000 1000
	
	//按键1 对应引脚为低电平
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 0 )
	{
		//满足上一次记录的该引脚电平是高电平
		if(Key_oldvalue & 0x01)
		{
			
			Key_pres |= 0x01;//记录当前按键按下
			Key_oldvalue &= ~0x01;//更新老的按键状态值
			
			//第一次进来  延时一会
			delay(40);
		}
	}
	else//检测到高电平
	{
		if(!(Key_oldvalue & 0x01))//这里是松开一瞬间 0--->1
		{
			Key_oldvalue |= 0x01;//更新老的按键状态值
			delay(40);
		}
	}
		
	
	//按键2 对应引脚为低电平
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) == 0 )
	{
		//满足上一次记录的该引脚电平是高电平
		if(Key_oldvalue & 0x02)
		{
			Key_pres |= 0x02;//记录当前按键按下
			Key_oldvalue &= ~0x02;//更新老的按键状态值
			
			delay(40);
		}
	}
	else//检测到高电平
	{
		if(!(Key_oldvalue & 0x02))//这里是松开一瞬间 0--->1
		{
			Key_oldvalue |= 0x02;//更新老的按键状态值
			delay(40);
		}
	}
	
	
	//按键1 对应引脚为低电平
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 0 )
	{
		//满足上一次记录的该引脚电平是高电平
		if(Key_oldvalue & 0x04)
		{
			Key_pres |= 0x04;//记录当前按键按下
			Key_oldvalue &= ~0x04;//更新老的按键状态值
			delay(40);
		}
	}
	else//检测到高电平
	{
		if(!(Key_oldvalue & 0x04))//这里是松开一瞬间 0--->1
		{
			
			Key_oldvalue |= 0x04;//更新老的按键状态值
			delay(40);
		}
	}
	
	
	//按键1 对应引脚为低电平
	if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 0 )
	{
		//满足上一次记录的该引脚电平是高电平
		if(Key_oldvalue & 0x08)
		{
			Key_pres |= 0x08;//记录当前按键按下
			Key_oldvalue &= ~0x08;//更新老的按键状态值
			delay(40);
		}
	}
	else//检测到高电平
	{
		if(!(Key_oldvalue & 0x08))//这里是松开一瞬间 0--->1
		{
			Key_oldvalue |= 0x08;//更新老的按键状态值
			delay(40);
		}
	}
	

	return Key_pres;
	
}



// NVIC---misc.c
// EXTI---stm32f4xx_exti.c
// SYSCFG---stm32f4xx_syscfg.c
/*
函数功能：
*/
void KEY_EXTIInit(void)
{
	EXTI_InitTypeDef EXTIInitStruct;
	NVIC_InitTypeDef NVICInitStruct;
	
	//PA0 PE2 PE3 PE4
	//1、使能时钟 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	//2、配置引脚为输入模式
	KEY_Init();
	
	//3、将PA0和EXTI0中断检测线路连接
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

	//4、开始配置EXTI指定的中断线  EXTI0
	EXTIInitStruct.EXTI_Line = EXTI_Line0;
	EXTIInitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式
	EXTIInitStruct.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发---按键按下触发
	EXTIInitStruct.EXTI_LineCmd = ENABLE;//开启
	EXTI_Init(&EXTIInitStruct);
	
	
	//5、NVIC中断优先级配置
	NVICInitStruct.NVIC_IRQChannel = EXTI0_IRQn;//中断号选取
	NVICInitStruct.NVIC_IRQChannelPreemptionPriority = 5;//优先级0~15
	NVICInitStruct.NVIC_IRQChannelSubPriority = 5;//优先级0~15
	NVICInitStruct.NVIC_IRQChannelCmd   =  ENABLE;
	NVIC_Init(&NVICInitStruct);
}




