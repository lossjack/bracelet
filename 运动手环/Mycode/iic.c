#include "iic.h"


#define delay_us(n)  RTOS_Delay_us(n)

//��Ĭ��IIC���ų�ʼ��
void IIC_Init(void)
{
	GPIO_InitTypeDef GPIOInitStruct;//����һ��������ų�ʼ����Ϣ�Ľṹ��
	
	//ʹ��ʱ��--����AHB1ENR�Ĵ���
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);//PE2 PE3 PE4  PE13 PE14

	//����GPIOģ��ĺ����� GPIO_Init()
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_6 | GPIO_Pin_7;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_OUT;//���ģʽ
	GPIOInitStruct.GPIO_OType  = GPIO_OType_PP;//��©���
	GPIOInitStruct.GPIO_Speed  = GPIO_Low_Speed;//����
	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_UP;//��������
	GPIO_Init(GPIOD,&GPIOInitStruct);

}


static void IIC_SDAModeSelect(GPIOMode_TypeDef Mode)
{
	GPIO_InitTypeDef GPIOInitStruct;//����һ��������ų�ʼ����Ϣ�Ľṹ��
	
	//ʹ��ʱ��--����AHB1ENR�Ĵ���
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//PA0
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);//PE2 PE3 PE4  PE13 PE14
	
	
	//����GPIOģ��ĺ����� GPIO_Init()
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_7;
	GPIOInitStruct.GPIO_Mode   = Mode;//���ģʽ
	GPIOInitStruct.GPIO_OType  = GPIO_OType_PP;//��©���
	GPIOInitStruct.GPIO_Speed  = GPIO_Low_Speed;//����
	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_UP;//��������
	GPIO_Init(GPIOD,&GPIOInitStruct);

}

//�����ź�    ����--->�ӻ�
void IIC_Start(void)
{
	//��SDA��Ϊ���ģʽ
	IIC_SDAModeSelect(GPIO_Mode_OUT);
	
	SCL(1);
	SDA_W(1);
	delay_us(20);//IIC���߿���--����һ��ʱ��
	
	//��ʼ����
	SDA_W(0);//SCL�ߵ�ƽ�ڼ�SDA�����½���
	delay_us(5);
	SCL(0);
}

//ֹͣ�ź�    ����--->�ӻ�
void IIC_Stop(void)
{
	//��SDA��Ϊ���ģʽ
	IIC_SDAModeSelect(GPIO_Mode_OUT);
	
	SCL(0);
	SDA_W(0);
	delay_us(5);
	
	SCL(1);
	delay_us(5);
	SDA_W(1);
}

//����д8bit  ����--->�ӻ�
void IIC_WriteByte(unsigned char WData)
{
	//��SDA��Ϊ���ģʽ
	IIC_SDAModeSelect(GPIO_Mode_OUT);
	
	for(int i=0;i<8;i++)
	{
		SCL(0);//SCL�͵�ƽ�ڼ� �����߿��Ըı�SDA����һ��λ
		delay_us(2);
		if(WData & (0x80>>i))
			SDA_W(1);//��ʾ������1
		else
			SDA_W(0);//��ʾҪ���͵�ǰλ��0
		delay_us(3);
		
		
		SCL(1);//SCL�ߵ�ƽ�ڼ� �Ǹ������߶�ȡSDA��ʱ��
		delay_us(5);
	}
	
	SCL(0);//���Ҫ��֤SCL�ǵ͵�ƽ��������
}




//������8bit  �ӻ�--->����
unsigned char IIC_ReadByte(void)
{
	unsigned char RData=0;
	
	//SDA����Ϊ����ģʽ
	IIC_SDAModeSelect(GPIO_Mode_IN);
	
	for(int i=0;i<8;i++)
	{
		SCL(0);//�ӻ���SCL�͵�ƽ�ڼ�ı�SDA����
		delay_us(5);//�ӻ���������λ���ڼ�
		
		
		SCL(1);//�����ڸߵ�ƽ�ж�SDA��ƽ��ȡ
		delay_us(2);
		if(SDA_R)
			RData |= (0x80>>i);//����1
		delay_us(3);
	}
	SCL(0);//���Ҫ��֤SCL�ǵ͵�ƽ��������
	return RData;
}

//Ӧ�����
//ÿ�շ���8bit��ĵ�9��������ġ�
//��������Ҫ�������߷���һ��λ��Ӧ���ź�
//SDA--0 ��Ӧ��  SDA--1 ��Ӧ��

//����дӦ��  ����--->�ӻ�
//ack == 1 ��Ӧ��  
void IIC_WriteAck(char ack)
{
	//SDA����Ϊ���
	IIC_SDAModeSelect(GPIO_Mode_OUT);
	
	SCL(0);
	delay_us(2);
	SDA_W(ack);
	delay_us(3);
	
	SCL(1);
	delay_us(5);
	
	SCL(0);
}


//������Ӧ��  �ӻ�--->����
char IIC_ReadAck(void)
{
	char ack=1;//Ĭ����Ӧ��
	//SDA����Ϊ����
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





