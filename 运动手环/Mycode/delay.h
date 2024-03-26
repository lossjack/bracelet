#ifndef __DELAY_H
#define __DELAY_H


#include "stm32f4xx.h"

void delay_us(int us);
void delay_ms(int ms);
void delay_s(int s);

//ʹ����RTOSҪ������ľ�׼��ʱ
void RTOS_Delay_us(int us);
void RTOS_Delay_ms(int ms);
#endif
