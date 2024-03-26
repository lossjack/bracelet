#include "spi.h"

void SPI0_Init(void)
{
	GPIO_InitTypeDef GPIOInitStruct;//定义一个存放引脚初始化信息的结构体
	
	//使能时钟--——AHB1ENR寄存器
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//PA0
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);//PE2 PE3 PE4  PE13 PE14
	
	
	//配置GPIO模块的函数： GPIO_Init()
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_14;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_OUT;//输出模式
	GPIOInitStruct.GPIO_OType  = GPIO_OType_PP;//推挽输出
	GPIOInitStruct.GPIO_Speed  = GPIO_Medium_Speed;//低速
	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_UP;//上拉输入
	GPIO_Init(GPIOB,&GPIOInitStruct);
	
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_4;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_IN;//输出模式
	GPIO_Init(GPIOB,&GPIOInitStruct);
	
	SPI_SCK(0);//Mode0 SCK默认低电平
}



//该函数是SPI模式0函数，可以发送8个位，也可以读取别人发给主机的8个位--全双工
unsigned char SPI0_ReadWriteByte(unsigned char WData)
{
	unsigned char RData=0;
	
	for(int i=0;i<8;i++)
	{
		SPI_SCK(0);//下降沿  如果是第一次进来刚好就是Mode0开始发送第一个位
		if(WData & (0x80>>i))
			SPI_MOSI(1);
		else
			SPI_MOSI(0);
		delay_us(1);
		
		
		SPI_SCK(1);//上升沿
		if(SPI_MISO)
			RData |= (0x80>>i);
		delay_us(1);
	}
	return RData;
}



