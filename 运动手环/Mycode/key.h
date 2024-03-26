#ifndef __KEY_H
#define __KEY_H


#include "stm32f4xx.h"

void KEY_Init(void);
unsigned char KEY_Scan(void);
void KEY_EXTIInit(void);


#endif
