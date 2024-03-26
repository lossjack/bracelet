#include "delay.h"




//us<= 798900
void delay_us(int us)
{
	//根据要延时的时间算出需要计的脉冲个数
	//定下脉冲频率 选择时钟源为系统时钟/8 =21Mhz
	SysTick->CTRL &= ~(0x01<<2);
	SysTick->LOAD = us*21;//计算得到的脉冲个数存入重装载寄存器 -1
	SysTick->VAL  = 0;//这样LOAD的值就会加载进来  清除标志位
	SysTick->CTRL |= 0x01;//开启定时器计数器

	//等待时间到----标志位置为1
	while(!(SysTick->CTRL & (0x01<<16)));
	
	SysTick->CTRL &= ~0x01;//关闭滴答定时器
}


//ms <= 0xFFFFFF/21000 ms<= 798.9
void delay_ms(int ms)
{
	//根据要延时的时间算出需要计的脉冲个数
	//定下脉冲频率 选择时钟源为系统时钟/8 =21Mhz
	SysTick->CTRL &= ~(0x01<<2);
	SysTick->LOAD = ms*21000;
	SysTick->VAL  = 0;//这样LOAD的值就会加载进来  清除标志位
	SysTick->CTRL |= 0x01;//开启定时器计数器

	//等待时间到----标志位置为1
	while(!(SysTick->CTRL & (0x01<<16)));
	
	SysTick->CTRL &= ~0x01;//关闭滴答定时器
}



void delay_s(int s)
{
	while(s--)
	{
		delay_ms(500);
		delay_ms(500);
	}
}



