#include "rtc.h"

/*�˺��������жϿ⺯��
RTC_Alarm_IRQHandler��һ������ʱʹ��*/
static void Alarm_delay_s(int s);

/*��ʼ��������*/
static void BEEP_Init(void);

//��ʼ��RTC
void RTC_Config(void)
{
	RTC_InitTypeDef RTC_InitStruct;
	RTC_DateTypeDef RTC_DateStruct;
	RTC_TimeTypeDef RTC_TimeStruct;
	
	//�ȶ�ȡRTC���ݼĴ����д�������ݿ��Ƿ���ָ������ֵ ���Ǿ�û���ù�
	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x1244)
	{
		//�ٱ�ʾû�г�ʼ���� ��Ҫ��ʼ��RTC��������һ��ʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
		
		//�����󱸼Ĵ�������д���ʰ���RTC�Ĵ���
		PWR_BackupAccessCmd(ENABLE);
		
		//��ʹ��RTCʱ��
		RCC_LSEConfig(RCC_LSE_ON);
		
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET);//�ȴ�LSE�ȶ�����
		//RCC��ѡ��LSE��ΪRTCʱ��
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		RCC_RTCCLKCmd(ENABLE);//ʹ��RTCʱ��(LSE\LSI)
		RTC_WaitForSynchro();//�ȴ��Ĵ���ͬ����־λ��λ
		
		//��RTC���� 
		RTC_InitStruct.RTC_HourFormat   =RTC_HourFormat_24;
		RTC_InitStruct.RTC_AsynchPrediv =128-1; //�첽��Ƶ
		RTC_InitStruct.RTC_SynchPrediv	=256-1; //ͬ����Ƶ
		RTC_Init(&RTC_InitStruct);
		
		//���������ں�ʱ��
		RTC_DateStruct.RTC_Year=22;
		RTC_DateStruct.RTC_Month=11;
		RTC_DateStruct.RTC_Date=14;
		RTC_DateStruct.RTC_WeekDay=01;
		RTC_SetDate(RTC_Format_BIN,&RTC_DateStruct);
		
		RTC_TimeStruct.RTC_Hours=19;//ʱ
		RTC_TimeStruct.RTC_Minutes=16;//��
		RTC_TimeStruct.RTC_Seconds=00;
		RTC_SetTime(RTC_Format_BIN,&RTC_TimeStruct);
		
		//�ݴ��һ�������־λ��RTC�󱸼Ĵ���
		RTC_WriteBackupRegister(RTC_BKP_DR0,0x1244);
	}
	else
	{
		/* ʹ�ܵ�Դ����ģ��ʱ�� */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
		/* �������RTC�Ĵ�������󱸼Ĵ��� DBE */
		PWR_BackupAccessCmd(ENABLE);
	}
	
}



//����A
void RTC_SetAlarm_A(int Hours,int Minutes,int Seconds)
{
	BEEP_Init();
	
	//�ṹ��s
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	RTC_AlarmTypeDef RTC_AlarmStruct;//��������
	
	 //�ر�����
    RTC_AlarmCmd(RTC_Alarm_A,DISABLE);
//	RTC_ClearITPendingBit(RTC_IT_ALRA);//����A��־λ���
	
	
	//��������,ʱ���봫�ξ���
	RTC_AlarmStruct.RTC_AlarmTime.RTC_H12=RTC_H12_AM;//24Сʱ
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Hours=Hours;//��
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Minutes=Minutes;//��
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Seconds=Seconds;//��
	
	RTC_AlarmStruct.RTC_AlarmDateWeekDay=13;
	RTC_AlarmStruct.RTC_AlarmDateWeekDaySel=RTC_AlarmDateWeekDaySel_WeekDay;
	RTC_AlarmStruct.RTC_AlarmMask=RTC_AlarmMask_DateWeekDay;
	RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmStruct);
	//�ж�
	EXTI_ClearITPendingBit(EXTI_Line17);//EXTI17��־λ���
	EXTI_InitStruct.EXTI_Line=EXTI_Line17;//�����ж���17
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;//�жϴ���
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Rising;//������
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;//ʹ��
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel=RTC_Alarm_IRQn;//ȫ���жϺ�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	//����
	RTC_ITConfig(RTC_IT_ALRA,ENABLE);
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
	
}
//�жϿ⺯��
void RTC_Alarm_IRQHandler(void)
{
	//GPIO_WriteBit(GPIOF,GPIO_Pin_8,(BitAction)1);
	GPIO_WriteBit(GPIOF,GPIO_Pin_9,(BitAction)0);
	Alarm_delay_s(500);
	
	GPIO_WriteBit(GPIOF,GPIO_Pin_9,(BitAction)1);
	//GPIO_WriteBit(GPIOF,GPIO_Pin_8,(BitAction)0);
	//EXTI->PR=0x01<<17;
	EXTI_ClearITPendingBit(EXTI_Line17);//EXTI17��־λ���
}

static void BEEP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//ʱ��ʹ��
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

//�Զ�������ʱ��
void SetTime(int Hour,int Minutes,int Seconds)
{
		RTC_InitTypeDef RTC_InitStruct;
		RTC_TimeTypeDef RTC_TimeStruct;

		/* ʹ�ܵ�Դ����ģ��ʱ�� */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
		/* �������RTC�Ĵ�������󱸼Ĵ��� DBE */
		PWR_BackupAccessCmd(ENABLE);

		//��ʹ��RTCʱ��
		RCC_LSEConfig(RCC_LSE_ON);
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET);//�ȴ�LSE�ȶ�����
		//RCC��ѡ��LSE��ΪRTCʱ��
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		RCC_RTCCLKCmd(ENABLE);//ʹ��RTCʱ��(LSE\LSI)
		RTC_WaitForSynchro();//�ȴ��Ĵ���ͬ����־λ��λ

		//��RTC���� 
		RTC_InitStruct.RTC_HourFormat   =RTC_HourFormat_24;
		RTC_InitStruct.RTC_AsynchPrediv =128-1; //�첽��Ƶ
		RTC_InitStruct.RTC_SynchPrediv	=256-1; //ͬ����Ƶ
		RTC_Init(&RTC_InitStruct);

		//����ʱ��
		RTC_TimeStruct.RTC_Hours=Hour;
		RTC_TimeStruct.RTC_Minutes=Minutes;
		RTC_TimeStruct.RTC_Seconds=Seconds;
		RTC_SetTime(RTC_Format_BCD,&RTC_TimeStruct);
}
