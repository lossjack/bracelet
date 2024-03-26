#ifndef __W25Q128_H
#define __W25Q128_H

#include "stm32f4xx.h"
#include "delay.h"
#include "spi.h"


void W25Q128_SectorErase(unsigned int Addr);
void W25Q128_PageWrite(unsigned int Addr,char *WData,int n);
void W25Q128_SerialRead(unsigned int Addr,char *RData,int n);

unsigned short W25Q128_DevoceIDRead(void);
#endif

