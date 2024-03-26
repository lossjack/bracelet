#ifndef __OLED_H
#define __OLED_H

#include "stm32f4xx.h"
#include "delay.h"
#include "iic.h"

#define OLED_SlaveAddr 0x3C  //0011 1100 <<1 = 0111 1000

void OLED_Init(void);

void OLED_Fill(unsigned char fill_Data);
void OLED_ON(void);
void OLED_OFF(void);


void OLED_ShowNum(void);
void OLED_ShowChar(int x,int y,char Chr,int fontsize);
void OLED_ShowLChar(int x,int y,char *Chr);

#endif
