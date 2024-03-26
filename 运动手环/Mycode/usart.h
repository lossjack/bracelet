#ifndef __USART_H
#define __USART_H

#include "stm32f4xx.h"
#include <stdio.h>

#include "oled.h"

#include "rtc.h"

void USART1_Config(void);
void USART1_SendString(char *str);

void USART2_SendString(char *str);



#endif
