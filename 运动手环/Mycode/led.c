#include "led.h"


/*
�������ܣ�LED���ų�ʼ��
*/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIOInitStruct;//����һ��������ų�ʼ����Ϣ�Ľṹ��
	
	//ʹ��ʱ��--����AHB1ENR�Ĵ���
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//PA0
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);//PE2 PE3 PE4  PE13 PE14
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);//PF8-������ PF9 PF10
	
	//����GPIOģ��ĺ����� GPIO_Init()
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_9 | GPIO_Pin_10;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_OUT;//���ģʽ
	GPIOInitStruct.GPIO_OType  = GPIO_OType_PP;//�������
	GPIOInitStruct.GPIO_Speed  = GPIO_Low_Speed;//����
	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_UP;//��������
	GPIO_Init(GPIOF,&GPIOInitStruct);
	
	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Init(GPIOE,&GPIOInitStruct);
	
	
	//��ȫ����
	GPIO_WriteBit(GPIOF,GPIO_Pin_9 | GPIO_Pin_10,(BitAction)1);//���
	GPIO_WriteBit(GPIOE,GPIO_Pin_13 | GPIO_Pin_14,(BitAction)1);//���

}

/*
�������ܣ�����ָ��LED������
����1��LEDn ָ���ĵڼ�յ�� LEDn:1~4
����2��LED_Flag  ָ����״̬ ��-0  ��1
*/
void LED_Control(char LEDn,char LED_Flag)
{
	
	switch(LEDn)
	{
		case 1:
			GPIO_WriteBit(GPIOF,GPIO_Pin_9,(BitAction)LED_Flag);
			break;
		case 2:
			GPIO_WriteBit(GPIOF,GPIO_Pin_10,(BitAction)LED_Flag);
			break;
		case 3:
			GPIO_WriteBit(GPIOE,GPIO_Pin_13,(BitAction)LED_Flag);
			break;
		case 4:
			GPIO_WriteBit(GPIOE,GPIO_Pin_14,(BitAction)LED_Flag);
			break;
		default:
			return ;
	}
}




