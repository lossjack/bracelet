#include "light.h"


unsigned short Light_sensor;//����һ���ڴ��б���

void ADC3IN5_Init(void)
{
	GPIO_InitTypeDef      GPIOInitStruct;
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	
	
	//ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//ʹ��DMA2ʱ��---ADC123
	
	//DMA����
	DMA_InitStructure.DMA_Channel = DMA_Channel_2;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC3->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&Light_sensor;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//����
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//����Ҫ��ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;//����Ҫ��ַ����
	
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//���ݿ��ָ��
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	
	
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//ѭ��ģʽ
	
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//���ȼ�---��
	
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; //�ر�FIFO        
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;//����
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	DMA_Init(DMA2_Stream0,&DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);//ʹ��DMA2 ������0

	
	
	//��������--ģ������ ����
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_7;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_AN;//ģ��ģʽ
	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_NOPULL;//��������������
	GPIO_Init(GPIOF,&GPIOInitStruct);
	
	//����3��ADC�Ĺ�������
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;//2��Ƶ 42Mhz/2 = 21Mhz T=1/21us
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	//����ADC3����
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//ADת������-�ֱ��� 6 8 10 12bit
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//�Ƿ�ɨ��--���ݵ�ͨ��--���� ���Ƕ�ͨ��-����Ҫ��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//�Ƿ�����ת��  ����������ת��һ�ξͲ���Ҫ��  ��������ת������һ��ֻ��ת��һ��
	//ADC_InitStructure.ADC_ExternalTrigConv     = ;//�ⲿ����Դѡ��--��ѡ
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//�ⲿ��������ѡ��--Ҳ����ѡ����Ҫ
	ADC_InitStructure.ADC_DataAlign            = ADC_DataAlign_Right;//���ݶ���16λ�Ĵ��� 12bit��߾���  ���� ����--����
	ADC_InitStructure.ADC_NbrOfConversion      = 1;//ת��ͨ������ 
	ADC_Init(ADC3,&ADC_InitStructure);

	//ADCͨ��ת������˳������              ת����˳��  ����ʱ�� 84�� 
	ADC_RegularChannelConfig(ADC3,ADC_Channel_5,1,ADC_SampleTime_84Cycles);
	
	
	//����ADC��ʲô�����ᴥ��DMA����
	ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);

	/* Enable ADC3 DMA */
	ADC_DMACmd(ADC3, ENABLE);

	
	
	//ʹ��ADC3
	ADC_Cmd(ADC3,ENABLE);
}



