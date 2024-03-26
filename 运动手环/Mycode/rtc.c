#include "rtc.h"

/*此函数仅给中断库函数
RTC_Alarm_IRQHandler做一个粗延时使用*/
static void Alarm_delay_s(int s);

/*初始化蜂鸣器*/
static void BEEP_Init(void);

//初始化RTC
void RTC_Config(void)
{
	RTC_InitTypeDef RTC_InitStruct;
	RTC_DateTypeDef RTC_DateStruct;
	RTC_TimeTypeDef RTC_TimeStruct;
	
	//先读取RTC备份寄存器中储存的数据看是否是指定的数值 不是就没配置过
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x1244)
	{
		//①表示没有初始化过 需要初始化RTC和再配置一次时间
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
		
		//开启后备寄存器区域写访问包括RTC寄存器
		PWR_BackupAccessCmd(ENABLE);
		
		//②使能RTC时钟
		RCC_LSEConfig(RCC_LSE_ON);
		
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET);//等待LSE稳定就绪
		//RCC中选择LSE作为RTC时钟
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		RCC_RTCCLKCmd(ENABLE);//使能RTC时钟(LSE\LSI)
		RTC_WaitForSynchro();//等待寄存器同步标志位置位
		
		//③RTC配置 
		RTC_InitStruct.RTC_HourFormat   =RTC_HourFormat_24;
		RTC_InitStruct.RTC_AsynchPrediv =128-1; //异步分频
		RTC_InitStruct.RTC_SynchPrediv	=256-1; //同步分频
		RTC_Init(&RTC_InitStruct);
		
		//④设置日期和时间
		RTC_DateStruct.RTC_Year=22;
		RTC_DateStruct.RTC_Month=11;
		RTC_DateStruct.RTC_Date=14;
		RTC_DateStruct.RTC_WeekDay=01;
		RTC_SetDate(RTC_Format_BIN,&RTC_DateStruct);
		
		RTC_TimeStruct.RTC_Hours=19;//时
		RTC_TimeStruct.RTC_Minutes=16;//分
		RTC_TimeStruct.RTC_Seconds=00;
		RTC_SetTime(RTC_Format_BIN,&RTC_TimeStruct);
		
		//⑤存放一个特殊标志位到RTC后备寄存器
		RTC_WriteBackupRegister(RTC_BKP_DR0,0x1244);
	}
	else
	{
		/* 使能电源管理模块时钟 */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
		/* 允许访问RTC寄存器及其后备寄存器 DBE */
		PWR_BackupAccessCmd(ENABLE);
	}
	
}



//闹钟A
void RTC_SetAlarm_A(int Hours,int Minutes,int Seconds)
{
	BEEP_Init();
	
	//结构体s
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	RTC_AlarmTypeDef RTC_AlarmStruct;//闹钟设置
	
	 //关闭闹钟
    RTC_AlarmCmd(RTC_Alarm_A,DISABLE);
//	RTC_ClearITPendingBit(RTC_IT_ALRA);//闹钟A标志位清除
	
	
	//配置闹钟,时分秒传参决定
	RTC_AlarmStruct.RTC_AlarmTime.RTC_H12=RTC_H12_AM;//24小时
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Hours=Hours;//点
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Minutes=Minutes;//分
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Seconds=Seconds;//秒
	
	RTC_AlarmStruct.RTC_AlarmDateWeekDay=13;
	RTC_AlarmStruct.RTC_AlarmDateWeekDaySel=RTC_AlarmDateWeekDaySel_WeekDay;
	RTC_AlarmStruct.RTC_AlarmMask=RTC_AlarmMask_DateWeekDay;
	RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmStruct);
	//中断
	EXTI_ClearITPendingBit(EXTI_Line17);//EXTI17标志位清除
	EXTI_InitStruct.EXTI_Line=EXTI_Line17;//闹钟中断线17
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;//中断触发
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Rising;//上升沿
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;//使能
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel=RTC_Alarm_IRQn;//全局中断号
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	//开启
	RTC_ITConfig(RTC_IT_ALRA,ENABLE);
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
	
}
//中断库函数
void RTC_Alarm_IRQHandler(void)
{
	//GPIO_WriteBit(GPIOF,GPIO_Pin_8,(BitAction)1);
	GPIO_WriteBit(GPIOF,GPIO_Pin_9,(BitAction)0);
	Alarm_delay_s(500);
	
	GPIO_WriteBit(GPIOF,GPIO_Pin_9,(BitAction)1);
	//GPIO_WriteBit(GPIOF,GPIO_Pin_8,(BitAction)0);
	//EXTI->PR=0x01<<17;
	EXTI_ClearITPendingBit(EXTI_Line17);//EXTI17标志位清除
}

static void BEEP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_25MHz;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOF,&GPIO_InitStruct);
}

static void Alarm_delay_s(int s)
{
	
	
 while(s--)
  for(int i=38400;i>0;i--);//1ms

}

//自定义设置时间
void SetTime(int Hour,int Minutes,int Seconds)
{
		RTC_InitTypeDef RTC_InitStruct;
		RTC_TimeTypeDef RTC_TimeStruct;

		/* 使能电源管理模块时钟 */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
		/* 允许访问RTC寄存器及其后备寄存器 DBE */
		PWR_BackupAccessCmd(ENABLE);

		//②使能RTC时钟
		RCC_LSEConfig(RCC_LSE_ON);
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET);//等待LSE稳定就绪
		//RCC中选择LSE作为RTC时钟
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		RCC_RTCCLKCmd(ENABLE);//使能RTC时钟(LSE\LSI)
		RTC_WaitForSynchro();//等待寄存器同步标志位置位

		//③RTC配置 
		RTC_InitStruct.RTC_HourFormat   =RTC_HourFormat_24;
		RTC_InitStruct.RTC_AsynchPrediv =128-1; //异步分频
		RTC_InitStruct.RTC_SynchPrediv	=256-1; //同步分频
		RTC_Init(&RTC_InitStruct);

		//设置时间
		RTC_TimeStruct.RTC_Hours=Hour;
		RTC_TimeStruct.RTC_Minutes=Minutes;
		RTC_TimeStruct.RTC_Seconds=Seconds;
		RTC_SetTime(RTC_Format_BCD,&RTC_TimeStruct);
}
