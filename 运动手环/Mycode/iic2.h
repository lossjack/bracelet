#ifndef __IIC2_H
#define __IIC2_H

#include "stm32f4xx.h"
#include "delay.h"

//SCL_1一定是MCU控制

#define SCL_1(n)   GPIO_WriteBit(GPIOB,GPIO_Pin_8,(BitAction)n)
#define SDA_W1(n) GPIO_WriteBit(GPIOB,GPIO_Pin_9,(BitAction)n)
#define SDA_R1    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)

#define delay_us(n) RTOS_Delay_us(n)
#define delay_ms(n) RTOS_Delay_ms(n)

static void IIC2_SDAModeSelect(GPIOMode_TypeDef Mode);

void IIC2_Init(void);


void IIC2_Start(void);
void IIC2_Stop(void);
void IIC2_WriteByte(unsigned char WData);
unsigned char IIC2_ReadByte(void);
void IIC2_WriteAck(char ack);
char IIC2_ReadAck(void);



#endif