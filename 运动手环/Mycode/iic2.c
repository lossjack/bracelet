#include "iic2.h"


#define delay_us(n)  RTOS_Delay_us(n)

//��Ĭ��IIC���ų�ʼ��
void IIC2_Init(void)
{
	GPIO_InitTypeDef GPIOInitStruct;//����һ��������ų�ʼ����Ϣ�Ľṹ��
	
	//ʹ��ʱ��--����AHB1ENR�Ĵ���
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);//PE2 PE3 PE4  PE13 PE14

	//����GPIOģ��ĺ����� GPIO_Init()
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_8 | GPIO_Pin_9;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_OUT;//���ģʽ
	GPIOInitStruct.GPIO_OType  = GPIO_OType_PP;//��©���
	GPIOInitStruct.GPIO_Speed  = GPIO_Low_Speed;//����
	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_UP;//��������
	GPIO_Init(GPIOB,&GPIOInitStruct);

}


static void IIC2_SDAModeSelect(GPIOMode_TypeDef Mode)
{
	GPIO_InitTypeDef GPIOInitStruct;//����һ��������ų�ʼ����Ϣ�Ľṹ��
	
	//ʹ��ʱ��--����AHB1ENR�Ĵ���
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//PA0
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);//PE2 PE3 PE4  PE13 PE14
	
	
	//����GPIOģ��ĺ����� GPIO_Init()
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_9;
	GPIOInitStruct.GPIO_Mode   = Mode;//���ģʽ
	GPIOInitStruct.GPIO_OType  = GPIO_OType_PP;//��©���
	GPIOInitStruct.GPIO_Speed  = GPIO_Low_Speed;//����
	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_UP;//��������
	GPIO_Init(GPIOB,&GPIOInitStruct);

}

//�����ź�    ����--->�ӻ�
void IIC2_Start(void)
{
	//��SDA��Ϊ���ģʽ
	IIC2_SDAModeSelect(GPIO_Mode_OUT);
	
	SCL_1(1);
	SDA_W1(1);
	delay_us(20);//IIC���߿���--����һ��ʱ��
	
	//��ʼ����
	SDA_W1(0);//SCL�ߵ�ƽ�ڼ�SDA�����½���
	delay_us(5);
	SCL_1(0);
}

//ֹͣ�ź�    ����--->�ӻ�
void IIC2_Stop(void)
{
	//��SDA��Ϊ���ģʽ
	IIC2_SDAModeSelect(GPIO_Mode_OUT);
	
	SCL_1(0);
	SDA_W1(0);
	delay_us(5);
	
	SCL_1(1);
	delay_us(5);
	SDA_W1(1);
}

//����д8bit  ����--->�ӻ�
void IIC2_WriteByte(unsigned char WData)
{
	//��SDA��Ϊ���ģʽ
	IIC2_SDAModeSelect(GPIO_Mode_OUT);
	
	for(int i=0;i<8;i++)
	{
		SCL_1(0);//SCL�͵�ƽ�ڼ� �����߿��Ըı�SDA����һ��λ
		delay_us(2);
		if(WData & (0x80>>i))
			SDA_W1(1);//��ʾ������1
		else
			SDA_W1(0);//��ʾҪ���͵�ǰλ��0
		delay_us(3);
		
		
		SCL_1(1);//SCL�ߵ�ƽ�ڼ� �Ǹ������߶�ȡSDA��ʱ��
		delay_us(5);
	}
	
	SCL_1(0);//���Ҫ��֤SCL�ǵ͵�ƽ��������
}




//������8bit  �ӻ�--->����
unsigned char IIC2_ReadByte(void)
{
	unsigned char RData=0;
	
	//SDA����Ϊ����ģʽ
	IIC2_SDAModeSelect(GPIO_Mode_IN);
	
	for(int i=0;i<8;i++)
	{
		SCL_1(0);//�ӻ���SCL�͵�ƽ�ڼ�ı�SDA����
		delay_us(5);//�ӻ���������λ���ڼ�
		
		
		SCL_1(1);//�����ڸߵ�ƽ�ж�SDA��ƽ��ȡ
		delay_us(2);
		if(SDA_R1)
			RData |= (0x80>>i);//����1
		delay_us(3);
	}
	SCL_1(0);//���Ҫ��֤SCL�ǵ͵�ƽ��������
	return RData;
}

//Ӧ�����
//ÿ�շ���8bit��ĵ�9��������ġ�
//��������Ҫ�������߷���һ��λ��Ӧ���ź�
//SDA--0 ��Ӧ��  SDA--1 ��Ӧ��

//����дӦ��  ����--->�ӻ�
//ack == 1 ��Ӧ��  
void IIC2_WriteAck(char ack)
{
	//SDA����Ϊ���
	IIC2_SDAModeSelect(GPIO_Mode_OUT);
	
	SCL_1(0);
	delay_us(2);
	SDA_W1(ack);
	delay_us(3);
	
	SCL_1(1);
	delay_us(5);
	
	SCL_1(0);
}


//������Ӧ��  �ӻ�--->����
char IIC2_ReadAck(void)
{
	char ack=1;//Ĭ����Ӧ��
	//SDA����Ϊ����
	IIC2_SDAModeSelect(GPIO_Mode_IN);
	
	SCL_1(0);
	delay_us(5);

	
	SCL_1(1);
	if(SDA_R1==0)
		ack = 0;
	delay_us(5);
	
	SCL_1(0);
	
	return ack;
}

