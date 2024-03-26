#include "spi.h"

void SPI0_Init(void)
{
	GPIO_InitTypeDef GPIOInitStruct;//����һ��������ų�ʼ����Ϣ�Ľṹ��
	
	//ʹ��ʱ��--����AHB1ENR�Ĵ���
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//PA0
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);//PE2 PE3 PE4  PE13 PE14
	
	
	//����GPIOģ��ĺ����� GPIO_Init()
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_14;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_OUT;//���ģʽ
	GPIOInitStruct.GPIO_OType  = GPIO_OType_PP;//�������
	GPIOInitStruct.GPIO_Speed  = GPIO_Medium_Speed;//����
	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_UP;//��������
	GPIO_Init(GPIOB,&GPIOInitStruct);
	
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_4;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_IN;//���ģʽ
	GPIO_Init(GPIOB,&GPIOInitStruct);
	
	SPI_SCK(0);//Mode0 SCKĬ�ϵ͵�ƽ
}



//�ú�����SPIģʽ0���������Է���8��λ��Ҳ���Զ�ȡ���˷���������8��λ--ȫ˫��
unsigned char SPI0_ReadWriteByte(unsigned char WData)
{
	unsigned char RData=0;
	
	for(int i=0;i<8;i++)
	{
		SPI_SCK(0);//�½���  ����ǵ�һ�ν����պþ���Mode0��ʼ���͵�һ��λ
		if(WData & (0x80>>i))
			SPI_MOSI(1);
		else
			SPI_MOSI(0);
		delay_us(1);
		
		
		SPI_SCK(1);//������
		if(SPI_MISO)
			RData |= (0x80>>i);
		delay_us(1);
	}
	return RData;
}



