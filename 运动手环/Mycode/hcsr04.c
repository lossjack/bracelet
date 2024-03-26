#include "hcsr04.h"

//�궨��ȷ������IO����
#define TRIG_CLK  RCC_AHB1Periph_GPIOA
#define TRIG_G    GPIOA
#define TRIG_P    GPIO_Pin_2

#define ECHO_CLK  RCC_AHB1Periph_GPIOA
#define ECHO_G    GPIOA
#define ECHO_P    GPIO_Pin_3



void HCSR04_Init(void)
{
	GPIO_InitTypeDef GPIOInitStruct;//����һ��������ų�ʼ����Ϣ�Ľṹ��
	
	//ʹ��ʱ��--����AHB1ENR�Ĵ���
	RCC_AHB1PeriphClockCmd(TRIG_CLK,ENABLE);//PE2 PE3 PE4  PE13 PE14
	RCC_AHB1PeriphClockCmd(ECHO_CLK,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);//
	
	//����GPIOģ��ĺ����� GPIO_Init()
	GPIOInitStruct.GPIO_Pin    = TRIG_P;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_OUT;//���ģʽ
	GPIOInitStruct.GPIO_OType  = GPIO_OType_PP;//�������
	GPIOInitStruct.GPIO_Speed  = GPIO_Low_Speed;//����
	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_UP;//��������
	GPIO_Init(TRIG_G,&GPIOInitStruct);

	GPIOInitStruct.GPIO_Pin    = ECHO_P;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_IN;//����ģʽ
	GPIO_Init(ECHO_G,&GPIOInitStruct);
	
	//���TIM6 �ĳ�ʼ�� ����������ļ�ʱ
	TIM6->CR1 &= ~0x01;//�ȹر�TIM6
	TIM6->PSC = 84-1;//84Mhz/84 T=1us
	TIM6->ARR = 65535;//65.535ms ���� 340m/s   0.34m/ms  34cm/ms
	TIM6->CNT = 0;
	

}


//��������ຯ��
float HCSR04_GetDistance(void)
{
	//����TRIG���������ź�  һ������10us�ĸߵ�ƽ
	GPIO_WriteBit(TRIG_G,TRIG_P,1);
	delay_us(15);
	GPIO_WriteBit(TRIG_G,TRIG_P,0);
	
	//�ȴ�ECHO��ģ������
	while(GPIO_ReadInputDataBit(ECHO_G,ECHO_P) == 0)
	{
		//��ʱ  �ȴ�
	}
	
	//�ߵ�ƽ�տ�ʼ����ʼ��ʱ
	TIM6->CNT = 0;
	TIM6->CR1 |= 0x01;//����������
	
	//�ȴ�ECHO��Ϊ�͵�ƽ
	while(GPIO_ReadInputDataBit(ECHO_G,ECHO_P) == 1);
	
	TIM6->CR1 &= ~0x01;//�رռ�����
	printf("CNT:%d\r\n",TIM6->CNT);
	//          us     ��Ϊms  ������  ��ȥһ�����
	//return TIM6->CNT/1000.0 * 34 /2;
	return 0.017*TIM6->CNT; //���ؾ���  ��λ��cm
}





