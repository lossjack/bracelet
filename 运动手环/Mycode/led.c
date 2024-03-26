#include "led.h"


/*
函数功能：LED引脚初始化
*/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIOInitStruct;//定义一个存放引脚初始化信息的结构体
	
	//使能时钟--——AHB1ENR寄存器
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//PA0
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);//PE2 PE3 PE4  PE13 PE14
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);//PF8-蜂鸣器 PF9 PF10
	
	//配置GPIO模块的函数： GPIO_Init()
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_9 | GPIO_Pin_10;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_OUT;//输出模式
	GPIOInitStruct.GPIO_OType  = GPIO_OType_PP;//推挽输出
	GPIOInitStruct.GPIO_Speed  = GPIO_Low_Speed;//低速
	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_UP;//上拉输入
	GPIO_Init(GPIOF,&GPIOInitStruct);
	
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init(GPIOE,&GPIOInitStruct);
	
	
	//灭全部灯
	GPIO_WriteBit(GPIOF,GPIO_Pin_9 | GPIO_Pin_10,(BitAction)1);//灭灯
	GPIO_WriteBit(GPIOE,GPIO_Pin_13 | GPIO_Pin_14,(BitAction)1);//灭灯

}

/*
函数功能：控制指定LED的亮灭
参数1：LEDn 指定的第几盏灯 LEDn:1~4
参数2：LED_Flag  指定的状态 亮-0  灭：1
*/
void LED_Control(char LEDn,char LED_Flag)
{
	
	switch(LEDn)
	{
		case 1:
			GPIO_WriteBit(GPIOF,GPIO_Pin_9,(BitAction)LED_Flag);
			break;
		case 2:
			GPIO_WriteBit(GPIOF,GPIO_Pin_10,(BitAction)LED_Flag);
			break;
		case 3:
			GPIO_WriteBit(GPIOE,GPIO_Pin_13,(BitAction)LED_Flag);
			break;
		case 4:
			GPIO_WriteBit(GPIOE,GPIO_Pin_14,(BitAction)LED_Flag);
			break;
		default:
			return ;
	}
}




