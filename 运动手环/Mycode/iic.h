#ifndef __IIC_H
#define __IIC_H

#include "stm32f4xx.h"
#include "delay.h"

//SCL一定是MCU控制
#define SCL(n)   GPIO_WriteBit(GPIOD,GPIO_Pin_6,(BitAction)n)
#define SDA_W(n) GPIO_WriteBit(GPIOD,GPIO_Pin_7,(BitAction)n)
#define SDA_R    GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7)

void IIC_Init(void);

void IIC_Start(void);
void IIC_Stop(void);
void IIC_WriteByte(unsigned char WData);
unsigned char IIC_ReadByte(void);
void IIC_WriteAck(char ack);
char IIC_ReadAck(void);





#endif
