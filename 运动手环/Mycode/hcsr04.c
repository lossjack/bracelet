#include "hcsr04.h"

//宏定义确定驱动IO引脚
#define TRIG_CLK  RCC_AHB1Periph_GPIOA
#define TRIG_G    GPIOA
#define TRIG_P    GPIO_Pin_2

#define ECHO_CLK  RCC_AHB1Periph_GPIOA
#define ECHO_G    GPIOA
#define ECHO_P    GPIO_Pin_3



void HCSR04_Init(void)
{
	GPIO_InitTypeDef GPIOInitStruct;//定义一个存放引脚初始化信息的结构体
	
	//使能时钟--——AHB1ENR寄存器
	RCC_AHB1PeriphClockCmd(TRIG_CLK,ENABLE);//PE2 PE3 PE4  PE13 PE14
	RCC_AHB1PeriphClockCmd(ECHO_CLK,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);//
	
	//配置GPIO模块的函数： GPIO_Init()
	GPIOInitStruct.GPIO_Pin    = TRIG_P;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_OUT;//输出模式
	GPIOInitStruct.GPIO_OType  = GPIO_OType_PP;//推挽输出
	GPIOInitStruct.GPIO_Speed  = GPIO_Low_Speed;//低速
	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_UP;//上拉输入
	GPIO_Init(TRIG_G,&GPIOInitStruct);

	GPIOInitStruct.GPIO_Pin    = ECHO_P;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_IN;//输入模式
	GPIO_Init(ECHO_G,&GPIOInitStruct);
	
	//添加TIM6 的初始化 用于下面测距的计时
	TIM6->CR1 &= ~0x01;//先关闭TIM6
	TIM6->PSC = 84-1;//84Mhz/84 T=1us
	TIM6->ARR = 65535;//65.535ms 声速 340m/s   0.34m/ms  34cm/ms
	TIM6->CNT = 0;
	

}


//超声波测距函数
float HCSR04_GetDistance(void)
{
	//控制TRIG发出启动信号  一个至少10us的高电平
	GPIO_WriteBit(TRIG_G,TRIG_P,1);
	delay_us(15);
	GPIO_WriteBit(TRIG_G,TRIG_P,0);
	
	//等待ECHO被模块拉高
	while(GPIO_ReadInputDataBit(ECHO_G,ECHO_P) == 0)
	{
		//超时  等待
	}
	
	//高电平刚开始，开始计时
	TIM6->CNT = 0;
	TIM6->CR1 |= 0x01;//开启计数器
	
	//等待ECHO变为低电平
	while(GPIO_ReadInputDataBit(ECHO_G,ECHO_P) == 1);
	
	TIM6->CR1 &= ~0x01;//关闭计数器
	printf("CNT:%d\r\n",TIM6->CNT);
	//          us     变为ms  乘声速  减去一半举例
	//return TIM6->CNT/1000.0 * 34 /2;
	return 0.017*TIM6->CNT; //返回距离  单位：cm
}





