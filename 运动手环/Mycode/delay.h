#ifndef __DELAY_H
#define __DELAY_H


#include "stm32f4xx.h"

void delay_us(int us);
void delay_ms(int ms);
void delay_s(int s);

//使用了RTOS要用下面的精准延时
void RTOS_Delay_us(int us);
void RTOS_Delay_ms(int ms);
#endif
