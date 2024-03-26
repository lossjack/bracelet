#include "dht11.h"

#define DHT11Data_G GPIOG
#define DHT11Data_P GPIO_Pin_9


#define Data(n)  GPIO_WriteBit(GPIOG,GPIO_Pin_9,(BitAction)n)

//配置DHT11引脚模式  模式可选---根据传参决定
static void DHT11_DataMode(GPIOMode_TypeDef Mode)
{
	GPIO_InitTypeDef GPIOInitStruct;//定义一个存放引脚初始化信息的结构体
	
	//使能时钟--——AHB1ENR寄存器
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//PA0
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);//PE2 PE3 PE4  PE13 PE14
	
	
	//配置GPIO模块的函数： GPIO_Init()
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_9;
	GPIOInitStruct.GPIO_Mode   = Mode;//输出模式
	GPIOInitStruct.GPIO_OType  = GPIO_OType_PP;//推挽输出
	GPIOInitStruct.GPIO_Speed  = GPIO_Low_Speed;//低速
	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_UP;//上拉输入
	GPIO_Init(GPIOG,&GPIOInitStruct);

}

//DHT11启动信号  MCU--->DHT11
void DHT11_Start(void)
{
	//先将引脚配置为 输出模式
	DHT11_DataMode(GPIO_Mode_OUT);

	//先拉高一段时间表示空闲
	Data(1);
	delay_ms(5);
	
	//正式开始发送启动信号
	Data(0);
	delay_ms(20);//拉低至少18ms
	Data(1);
}


int DHT11_SlaveAck(void)
{
	//先将引脚配置为 输出模式
	DHT11_DataMode(GPIO_Mode_IN);
	
//	int t=0;
	//等待从机应答
	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9) == 1);
//	{
//		delay_us(1);
//		t++;
//		if(t>50000)
//			return -1;
//	}
	
	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9) == 0);
	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9) == 1);
	
	//能到这里表示从机有应答
	return 1;//表示有应答
}


//该函数能够读取DHT11发送过来的8bit数据。函数返回读到的8bit结果
unsigned char DHT11_ReadByte(void)
{
	unsigned char RData=0;
	//将引脚配置为输入模式--因为这个函数会在上面应答函数之后执行。上面已将引脚配置为输入了 这里可以不再
	//配置
	DHT11_DataMode(GPIO_Mode_IN);
	
	for(int i=0;i<8;i++)
	{
		//等待54us低电平间隙过去
		while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9) == 0);
		
		//刚刚到高电平  延时35后 可以根据此时引脚电平判断当前是数字位还是1		
		delay_us(35);
		if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9) == 1)
		{
			//当前位是数字1
			RData |= 0x01<<(7-i);
			while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9) == 1);//等待剩下的高电平过去
		}
//		else
//		{
//			//因为变量RData默认为0  所以当当前位为0时，跳过即可
//			//RData &= ~(0x01<<(7-i));
//			//当前位是数字0
//		}
	}
	return RData;//返回读到的8bit结果
}


void DHT11_Stop(void)
{
	DHT11_DataMode(GPIO_Mode_OUT);
	Data(1);
}

//将上面4个DHT11时序对应读取函数 组合---总函数
int DHT11_ReadData(unsigned char *Buff)
{
	
	DHT11_Start();
	
	if(-1 == DHT11_SlaveAck())
	{
		//DHT11无应答
		return -1;//读取失败
	}
	
	Buff[0] = DHT11_ReadByte();//湿度整数
	Buff[1] = DHT11_ReadByte();//湿度小数---无用 固定为0
	Buff[2] = DHT11_ReadByte();//温度整数
	Buff[3] = DHT11_ReadByte();//温度小数
	Buff[4] = DHT11_ReadByte();//校验字节
	
	DHT11_Stop();
	
	
	//校验数据是否准确
	if((unsigned char)(Buff[0] + Buff[1] + Buff[2] + Buff[3])  == Buff[4])
	{
		//数据校验正确
		return 0;
		
	}
	
	return -2;
}




