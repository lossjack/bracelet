#ifndef __AT24C02_H
#define __AT24C02_H


#include "stm32f4xx.h"
#include "delay.h"
#include "iic.h"

int AT24C02_WriteByte(unsigned char Addr,unsigned char WData);
int AT24C02_ReadByte(unsigned char Addr,unsigned char *RData);

int AT24C02_PageWrite(unsigned char Addr,unsigned char *WData,int n);
int AT24C02_SeriesRead(unsigned char Addr,unsigned char *RData,int n);
#endif
