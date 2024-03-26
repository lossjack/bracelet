#include "light.h"


unsigned short Light_sensor;//定义一个内存中变量

void ADC3IN5_Init(void)
{
	GPIO_InitTypeDef      GPIOInitStruct;
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	
	
	//时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//使能DMA2时钟---ADC123
	
	//DMA配置
	DMA_InitStructure.DMA_Channel = DMA_Channel_2;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC3->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&Light_sensor;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//方向
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//不需要地址递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;//不需要地址递增
	
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//数据宽度指定
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	
	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//循环模式
	
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//优先级---高
	
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; //关闭FIFO        
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;//无用
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	DMA_Init(DMA2_Stream0,&DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);//使能DMA2 数据流0

	
	
	//配置引脚--模拟输入 浮空
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_7;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_AN;//模拟模式
	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_NOPULL;//无上拉下拉输入
	GPIO_Init(GPIOF,&GPIOInitStruct);
	
	//配置3个ADC的公共部分
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;//2分频 42Mhz/2 = 21Mhz T=1/21us
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	//独立ADC3配置
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//AD转换精度-分辨率 6 8 10 12bit
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//是否扫描--根据单通道--不开 还是多通道-必须要开
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//是否连续转换  连续：启动转换一次就不需要了  非连续：转换启动一次只会转换一次
	//ADC_InitStructure.ADC_ExternalTrigConv     = ;//外部触发源选择--不选
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//外部触发边沿选择--也可以选择不需要
	ADC_InitStructure.ADC_DataAlign            = ADC_DataAlign_Right;//数据对齐16位寄存器 12bit最高精度  靠左 靠右--常用
	ADC_InitStructure.ADC_NbrOfConversion      = 1;//转换通道数量 
	ADC_Init(ADC3,&ADC_InitStructure);

	//ADC通道转换规则（顺序）配置              转换的顺序  采样时间 84个 
	ADC_RegularChannelConfig(ADC3,ADC_Channel_5,1,ADC_SampleTime_84Cycles);
	
	
	//设置ADC的什么动作会触发DMA请求
	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);

	/* Enable ADC3 DMA */
	ADC_DMACmd(ADC3, ENABLE);

	
	
	//使能ADC3
	ADC_Cmd(ADC3,ENABLE);
}



