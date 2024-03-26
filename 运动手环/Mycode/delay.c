#include "delay.h"




//us<= 798900
void delay_us(int us)
{
	//����Ҫ��ʱ��ʱ�������Ҫ�Ƶ��������
	//��������Ƶ�� ѡ��ʱ��ԴΪϵͳʱ��/8 =21Mhz
	SysTick->CTRL &= ~(0x01<<2);
	SysTick->LOAD = us*21;//����õ����������������װ�ؼĴ��� -1
	SysTick->VAL  = 0;//����LOAD��ֵ�ͻ���ؽ���  �����־λ
	SysTick->CTRL |= 0x01;//������ʱ��������

	//�ȴ�ʱ�䵽----��־λ��Ϊ1
	while(!(SysTick->CTRL & (0x01<<16)));
	
	SysTick->CTRL &= ~0x01;//�رյδ�ʱ��
}


//ms <= 0xFFFFFF/21000 ms<= 798.9
void delay_ms(int ms)
{
	//����Ҫ��ʱ��ʱ�������Ҫ�Ƶ��������
	//��������Ƶ�� ѡ��ʱ��ԴΪϵͳʱ��/8 =21Mhz
	SysTick->CTRL &= ~(0x01<<2);
	SysTick->LOAD = ms*21000;
	SysTick->VAL  = 0;//����LOAD��ֵ�ͻ���ؽ���  �����־λ
	SysTick->CTRL |= 0x01;//������ʱ��������

	//�ȴ�ʱ�䵽----��־λ��Ϊ1
	while(!(SysTick->CTRL & (0x01<<16)));
	
	SysTick->CTRL &= ~0x01;//�رյδ�ʱ��
}



void delay_s(int s)
{
	while(s--)
	{
		delay_ms(500);
		delay_ms(500);
	}
}



