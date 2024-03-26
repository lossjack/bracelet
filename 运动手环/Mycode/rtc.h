#ifndef __RTC_H_
#define __RTC_H_

#include "stm32f4xx.h"
#include "delay.h"
#include "led.h"
#include "oled.h"

void RTC_Config(void);

void RTC_SetAlarm_A(int Hours,int Minutes,int Seconds);

void SetTime(int Hour,int Minutes,int Seconds);
#endif
