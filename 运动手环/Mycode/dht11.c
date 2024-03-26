#include "dht11.h"

#define DHT11Data_G GPIOG
#define DHT11Data_P GPIO_Pin_9


#define Data(n)  GPIO_WriteBit(GPIOG,GPIO_Pin_9,(BitAction)n)

//����DHT11����ģʽ  ģʽ��ѡ---���ݴ��ξ���
static void DHT11_DataMode(GPIOMode_TypeDef Mode)
{
	GPIO_InitTypeDef GPIOInitStruct;//����һ��������ų�ʼ����Ϣ�Ľṹ��
	
	//ʹ��ʱ��--����AHB1ENR�Ĵ���
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//PA0
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);//PE2 PE3 PE4  PE13 PE14
	
	
	//����GPIOģ��ĺ����� GPIO_Init()
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_9;
	GPIOInitStruct.GPIO_Mode   = Mode;//���ģʽ
	GPIOInitStruct.GPIO_OType  = GPIO_OType_PP;//�������
	GPIOInitStruct.GPIO_Speed  = GPIO_Low_Speed;//����
	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_UP;//��������
	GPIO_Init(GPIOG,&GPIOInitStruct);

}

//DHT11�����ź�  MCU--->DHT11
void DHT11_Start(void)
{
	//�Ƚ���������Ϊ ���ģʽ
	DHT11_DataMode(GPIO_Mode_OUT);

	//������һ��ʱ���ʾ����
	Data(1);
	delay_ms(5);
	
	//��ʽ��ʼ���������ź�
	Data(0);
	delay_ms(20);//��������18ms
	Data(1);
}


int DHT11_SlaveAck(void)
{
	//�Ƚ���������Ϊ ���ģʽ
	DHT11_DataMode(GPIO_Mode_IN);
	
//	int t=0;
	//�ȴ��ӻ�Ӧ��
	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9) == 1);
//	{
//		delay_us(1);
//		t++;
//		if(t>50000)
//			return -1;
//	}
	
	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9) == 0);
	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9) == 1);
	
	//�ܵ������ʾ�ӻ���Ӧ��
	return 1;//��ʾ��Ӧ��
}


//�ú����ܹ���ȡDHT11���͹�����8bit���ݡ��������ض�����8bit���
unsigned char DHT11_ReadByte(void)
{
	unsigned char RData=0;
	//����������Ϊ����ģʽ--��Ϊ���������������Ӧ����֮��ִ�С������ѽ���������Ϊ������ ������Բ���
	//����
	DHT11_DataMode(GPIO_Mode_IN);
	
	for(int i=0;i<8;i++)
	{
		//�ȴ�54us�͵�ƽ��϶��ȥ
		while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9) == 0);
		
		//�ոյ��ߵ�ƽ  ��ʱ35�� ���Ը��ݴ�ʱ���ŵ�ƽ�жϵ�ǰ������λ����1		
		delay_us(35);
		if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9) == 1)
		{
			//��ǰλ������1
			RData |= 0x01<<(7-i);
			while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9) == 1);//�ȴ�ʣ�µĸߵ�ƽ��ȥ
		}
//		else
//		{
//			//��Ϊ����RDataĬ��Ϊ0  ���Ե���ǰλΪ0ʱ����������
//			//RData &= ~(0x01<<(7-i));
//			//��ǰλ������0
//		}
	}
	return RData;//���ض�����8bit���
}


void DHT11_Stop(void)
{
	DHT11_DataMode(GPIO_Mode_OUT);
	Data(1);
}

//������4��DHT11ʱ���Ӧ��ȡ���� ���---�ܺ���
int DHT11_ReadData(unsigned char *Buff)
{
	
	DHT11_Start();
	
	if(-1 == DHT11_SlaveAck())
	{
		//DHT11��Ӧ��
		return -1;//��ȡʧ��
	}
	
	Buff[0] = DHT11_ReadByte();//ʪ������
	Buff[1] = DHT11_ReadByte();//ʪ��С��---���� �̶�Ϊ0
	Buff[2] = DHT11_ReadByte();//�¶�����
	Buff[3] = DHT11_ReadByte();//�¶�С��
	Buff[4] = DHT11_ReadByte();//У���ֽ�
	
	DHT11_Stop();
	
	
	//У�������Ƿ�׼ȷ
	if((unsigned char)(Buff[0] + Buff[1] + Buff[2] + Buff[3])  == Buff[4])
	{
		//����У����ȷ
		return 0;
		
	}
	
	return -2;
}




