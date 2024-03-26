#ifndef __SPI_H
#define __SPI_H

#include "stm32f4xx.h"
#include "delay.h"

#define SPI_SCK(n)   GPIO_WriteBit(GPIOB,GPIO_Pin_3,(BitAction)n) 
#define SPI_MOSI(n)  GPIO_WriteBit(GPIOB,GPIO_Pin_5,(BitAction)n) 
#define SPI_MISO     GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4) 
#define SPI_CS(n)    GPIO_WriteBit(GPIOB,GPIO_Pin_14,(BitAction)n)

void SPI0_Init(void);

unsigned char SPI0_ReadWriteByte(unsigned char WData);


#endif
