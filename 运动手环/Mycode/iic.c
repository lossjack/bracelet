#include "iic.h"


#define delay_us(n)  RTOS_Delay_us(n)

//对默认IIC引脚初始化
void IIC_Init(void)
{
	GPIO_InitTypeDef GPIOInitStruct;//定义一个存放引脚初始化信息的结构体
	
	//使能时钟--——AHB1ENR寄存器
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);//PE2 PE3 PE4  PE13 PE14

	//配置GPIO模块的函数： GPIO_Init()
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_6 | GPIO_Pin_7;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_OUT;//输出模式
	GPIOInitStruct.GPIO_OType  = GPIO_OType_PP;//开漏输出
	GPIOInitStruct.GPIO_Speed  = GPIO_Low_Speed;//低速
	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_UP;//上拉输入
	GPIO_Init(GPIOD,&GPIOInitStruct);

}


static void IIC_SDAModeSelect(GPIOMode_TypeDef Mode)
{
	GPIO_InitTypeDef GPIOInitStruct;//定义一个存放引脚初始化信息的结构体
	
	//使能时钟--——AHB1ENR寄存器
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//PA0
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);//PE2 PE3 PE4  PE13 PE14
	
	
	//配置GPIO模块的函数： GPIO_Init()
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_7;
	GPIOInitStruct.GPIO_Mode   = Mode;//输出模式
	GPIOInitStruct.GPIO_OType  = GPIO_OType_PP;//开漏输出
	GPIOInitStruct.GPIO_Speed  = GPIO_Low_Speed;//低速
	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_UP;//上拉输入
	GPIO_Init(GPIOD,&GPIOInitStruct);

}

//启动信号    主机--->从机
void IIC_Start(void)
{
	//将SDA变为输出模式
	IIC_SDAModeSelect(GPIO_Mode_OUT);
	
	SCL(1);
	SDA_W(1);
	delay_us(20);//IIC总线空闲--保持一段时间
	
	//开始启动
	SDA_W(0);//SCL高电平期间SDA产生下降沿
	delay_us(5);
	SCL(0);
}

//停止信号    主机--->从机
void IIC_Stop(void)
{
	//将SDA变为输出模式
	IIC_SDAModeSelect(GPIO_Mode_OUT);
	
	SCL(0);
	SDA_W(0);
	delay_us(5);
	
	SCL(1);
	delay_us(5);
	SDA_W(1);
}

//主机写8bit  主机--->从机
void IIC_WriteByte(unsigned char WData)
{
	//将SDA变为输出模式
	IIC_SDAModeSelect(GPIO_Mode_OUT);
	
	for(int i=0;i<8;i++)
	{
		SCL(0);//SCL低电平期间 发送者可以改变SDA发送一个位
		delay_us(2);
		if(WData & (0x80>>i))
			SDA_W(1);//表示是数字1
		else
			SDA_W(0);//表示要发送当前位是0
		delay_us(3);
		
		
		SCL(1);//SCL高电平期间 是给接受者读取SDA的时间
		delay_us(5);
	}
	
	SCL(0);//最后要保证SCL是低电平结束本轮
}




//主机读8bit  从机--->主机
unsigned char IIC_ReadByte(void)
{
	unsigned char RData=0;
	
	//SDA配置为输入模式
	IIC_SDAModeSelect(GPIO_Mode_IN);
	
	for(int i=0;i<8;i++)
	{
		SCL(0);//从机在SCL低电平期间改变SDA发送
		delay_us(5);//从机发送数据位的期间
		
		
		SCL(1);//主机在高电平判断SDA电平读取
		delay_us(2);
		if(SDA_R)
			RData |= (0x80>>i);//数字1
		delay_us(3);
	}
	SCL(0);//最后要保证SCL是低电平结束本轮
	return RData;
}

//应答机制
//每收发完8bit后的第9个脉冲节拍。
//接收者需要给发送者反馈一个位的应答信号
//SDA--0 有应答  SDA--1 无应答

//主机写应答  主机--->从机
//ack == 1 无应答  
void IIC_WriteAck(char ack)
{
	//SDA配置为输出
	IIC_SDAModeSelect(GPIO_Mode_OUT);
	
	SCL(0);
	delay_us(2);
	SDA_W(ack);
	delay_us(3);
	
	SCL(1);
	delay_us(5);
	
	SCL(0);
}


//主机读应答  从机--->主机
char IIC_ReadAck(void)
{
	char ack=1;//默认无应答
	//SDA配置为输入
	IIC_SDAModeSelect(GPIO_Mode_IN);
	
	SCL(0);
	delay_us(5);

	
	SCL(1);
	if(SDA_R==0)
		ack = 0;
	delay_us(5);
	
	SCL(0);
	
	return ack;
}





