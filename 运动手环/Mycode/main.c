/*�ɴ�Ѽ���˶��ֻ�*/
#include "stm32f4xx.h"//����ٷ��Ѿ������Ƕ�������е�����Ĵ�����
#include "led.h"
#include "key.h"
#include "delay.h"
#include "timer.h"
#include "usart.h"
#include <stdio.h>
#include "hcsr04.h"
#include "dht11.h"
#include "light.h"
#include "iic.h"
#include "at24c02.h"
#include "oled.h"
#include "spi.h"
#include "w25q128.h"

#include "FreeRTOS.h"
#include "Task.h"

#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"


#include "max30102.h"

#include "rtc.h"
#include "iic2.h"
#include "semphr.h"
#include "sys.h"


uint32_t aun_ir_buffer[500]; 
int32_t n_ir_buffer_length;
uint32_t aun_red_buffer[500];
int32_t n_sp02; 
int8_t ch_spo2_valid;
int32_t n_heart_rate;
int8_t  ch_hr_valid;
uint8_t uch_dummy;
 
#define MAX_BRIGHTNESS 255
 
void dis_DrawCurve(u32* data,u8 x);


static TaskHandle_t AppTask_Handle = NULL;//�����¼������Ϣ�Ľṹ��  ע�ⲻҪ�����ں����� ��Ϊ̫��
static TaskHandle_t AppTask_Handle1 = NULL;
static TaskHandle_t AppTask_Handle2 = NULL;
static TaskHandle_t AppTask_Handle3 = NULL;
static TaskHandle_t AppTask_Handle4 = NULL;/*����*/


SemaphoreHandle_t xMutex;

char setclock2[7]={0};
int flagi=0;

u32 hour=19;
u32 minute=7;
u32 second=0;


//�����Ӻ���
static void AppTask(void* parameter)
{    
		RTC_Config();
	
		RTC_TimeTypeDef RTCTime;
		RTC_DateTypeDef RTCDate;
		RTC_AlarmTypeDef RTC_AlarmStructure;
		char buf[128]={0};
		OLED_Init();
		OLED_Fill(0x00);//������
		//����A
		//RTC_SetAlarm_A(16,52,50);//ʱ  ��  ��
		
		
		
    while (1)
    {
		
		xSemaphoreTake(xMutex,portMAX_DELAY);//����
		
		OLED_ShowChar(24,2,'h',0);
		OLED_ShowChar(32,2,'e',0);
		OLED_ShowChar(40,2,'l',0);
		OLED_ShowChar(48,2,'l',0);
		OLED_ShowChar(56,2,'o',0);
		
		RTC_GetTime(RTC_Format_BIN,&RTCTime);
		RTC_GetDate(RTC_Format_BIN,&RTCDate);
		printf("��ʾ����:\r\n");
		sprintf(buf,"20%.2d-%.2d-%.2d %.2d:%.2d:%.2d",RTCDate.RTC_Year,
		RTCDate.RTC_Month,RTCDate.RTC_Date,RTCTime.RTC_Hours,
		RTCTime.RTC_Minutes,RTCTime.RTC_Seconds);
		
		OLED_ShowLChar(0,0,buf);
		
		 RTC_GetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
		 printf("%0.2d:%0.2d:%0.2d\r\n", RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours,
         RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes, 
         RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds);
			
		printf("%s\r\n",buf);
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)==0)
		{
		
		OLED_ON();
	
		GPIO_ResetBits(GPIOE,GPIO_Pin_13);
		
		}
		else
		{
		GPIO_SetBits(GPIOE,GPIO_Pin_13);
		}
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2)==0)
		{
		OLED_Fill(0x00);//������
     	OLED_OFF();
		GPIO_ResetBits(GPIOE,GPIO_Pin_14);
		}
		else
		{
		GPIO_SetBits(GPIOE,GPIO_Pin_14);
		}
		xSemaphoreGive(xMutex);
		vTaskDelay(500);
		//����
		
    }
}

void Oled_show_clock(){
	RTC_TimeTypeDef RTC_TimeStruct; // ���ڴ洢RTC��ʱ��
	RTC_DateTypeDef RTC_DateStruct; // ���ڴ洢������Ϣ
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct); // ��ȡ��ǰ��������Ϣ
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct); // ��ȡ��ǰ��ʱ����Ϣ

	u8 t = 0;
	u8 tbuf_clock[80]={0}; // ���ڴ洢��ʽ�����ʱ��������ַ���
	
	OLED_Fill(0x00);//������;
	sprintf((char *)tbuf_clock, "20%02d-%02d-%02d %02d:%02d:%02d",RTC_DateStruct.RTC_Year, RTC_DateStruct.RTC_Month, RTC_DateStruct.RTC_Date, hour%24, minute%60, second%60);
	
	OLED_ShowLChar(34,0,"set-clock"); 
	OLED_ShowLChar(0,3,(char*)tbuf_clock);
	vTaskDelay(100);
}

//�����Ӻ���
static void AppTask1(void* parameter)
{    
	//OLED_Init();
	//OLED_Fill(0x00);//������
	short x,y,z;
	float Gx,Gy,Gz;
	unsigned long step=0;
	vTaskDelay(20);
	while(0 != mpu_dmp_init()); // ֱ����ʼ���ɹ�Ϊֹ
	
	vTaskDelay(10);
	
	
	//���MPU6050�Ƿ� ����
	if((0x01<<6) & MPU_Read_Byte(MPU_PWR_MGMT1_REG))
	printf("����\r\n");
	char bufff[32] = {0};
	//dmp_set_pedometer_step_count(100);
	
    while (1)
    {
	xSemaphoreTake(xMutex,portMAX_DELAY);//����
		
		//MPU_Get_Accelerometer(&x,&y,&z);//////
		
		dmp_get_pedometer_step_count(&step);
        printf("STEP:%ld\r\n",step);
		
		sprintf(bufff,"step:%ld",step);
      
		USART2_SendString(bufff);
		
		OLED_ShowLChar(72,6,bufff);  
		
		xSemaphoreGive(xMutex);
	   	vTaskDelay(500);
    }
	
}


//�����Ӻ���
static void AppTask2(void* parameter)
{    
	
	
    uint32_t un_min, un_max, un_prev_data;  
	int i;
	int32_t n_brightness;
	float f_temp;
	u8 temp_num=0;
	u8 temp[6];
	u8 str[100];
	u8 dis_hr=0,dis_spo2=0;
	char string[100];
 
	max30102_init();
 
	printf("\r\n MAX30102  init  \r\n");
 
	un_min=0x3FFFF;
	un_max=0;
	
	n_ir_buffer_length=500; //buffer length of 100 stores 5 seconds of samples running at 100sps
	//read the first 500 samples, and determine the signal range
	
	
	
    for(i=0;i<n_ir_buffer_length;i++)
    {
        while(MAX30102_INT==1);   //wait until the interrupt pin asserts
        
		max30102_FIFO_ReadBytes(REG_FIFO_DATA,temp);
		aun_red_buffer[i] =  (long)((long)((long)temp[0]&0x03)<<16) | (long)temp[1]<<8 | (long)temp[2];    // Combine values to get the actual number
		aun_ir_buffer[i] = (long)((long)((long)temp[3] & 0x03)<<16) |(long)temp[4]<<8 | (long)temp[5];   // Combine values to get the actual number
            
        if(un_min>aun_red_buffer[i])
            un_min=aun_red_buffer[i];    //update signal min
        if(un_max<aun_red_buffer[i])
            un_max=aun_red_buffer[i];    //update signal max
    }
	un_prev_data=aun_red_buffer[i];
	//calculate heart rate and SpO2 after first 500 samples (first 5 seconds of samples)
    maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid); 
	
	char data_buf[32]={0};
	
	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)==0)
		{
		//GPIO_ResetBits(GPIOF,GPIO_Pin_9);	
		
		vTaskDelay(100);
		vTaskSuspend(AppTask_Handle);
			
		vTaskDelay(100);
		vTaskSuspend(AppTask_Handle1);
			
		vTaskDelay(100);
		OLED_Fill(0x00);//������
			
		
		
		
		//xSemaphoreTake(xMutex,portMAX_DELAY);//����
		
		i=0;
        un_min=0x3FFFF;
        un_max=0;
		
		//dumping the first 100 sets of samples in the memory and shift the last 400 sets of samples to the top
        for(i=100;i<500;i++)
        {
            aun_red_buffer[i-100]=aun_red_buffer[i];
            aun_ir_buffer[i-100]=aun_ir_buffer[i];
            
            //update the signal min and max
            if(un_min>aun_red_buffer[i])
            un_min=aun_red_buffer[i];
            if(un_max<aun_red_buffer[i])
            un_max=aun_red_buffer[i];
        }
		//take 100 sets of samples before calculating the heart rate.
        for(i=400;i<500;i++)
        {
            un_prev_data=aun_red_buffer[i-1];
            while(MAX30102_INT==1);
            max30102_FIFO_ReadBytes(REG_FIFO_DATA,temp);
			aun_red_buffer[i] =  (long)((long)((long)temp[0]&0x03)<<16) | (long)temp[1]<<8 | (long)temp[2];    // Combine values to get the actual number
			aun_ir_buffer[i] = (long)((long)((long)temp[3] & 0x03)<<16) |(long)temp[4]<<8 | (long)temp[5];   // Combine values to get the actual number
        
            if(aun_red_buffer[i]>un_prev_data)
            {
                f_temp=aun_red_buffer[i]-un_prev_data;
                f_temp/=(un_max-un_min);
                f_temp*=MAX_BRIGHTNESS;
                n_brightness-=(int)f_temp;
                if(n_brightness<0)
                    n_brightness=0;
            }
            else
            {
                f_temp=un_prev_data-aun_red_buffer[i];
                f_temp/=(un_max-un_min);
                f_temp*=MAX_BRIGHTNESS;
                n_brightness+=(int)f_temp;
                if(n_brightness>MAX_BRIGHTNESS)
                    n_brightness=MAX_BRIGHTNESS;
            }
			//send samples and calculation result to terminal program through UART
			if(ch_hr_valid == 1 && n_heart_rate<120)//**/ ch_hr_valid == 1 && ch_spo2_valid ==1 && n_heart_rate<120 && n_sp02<101
			{
				dis_hr = n_heart_rate;
				dis_spo2 = n_sp02;
			}
			else
			{
				dis_hr = 0;
				dis_spo2 = 0;
			}
			printf("HR=%i,", n_heart_rate); 
			printf("HRvalid=%i,", ch_hr_valid);
			printf("SpO2=%i,", n_sp02);
			printf("SPO2Valid=%i\r\n", ch_spo2_valid);
			sprintf(data_buf,"HR:%i", n_heart_rate);
			
			
			
			USART2_SendString(data_buf);
			
			OLED_ShowLChar(5,6,data_buf);  
			
		}
        maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
		
		
		
	
		}
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4)==0)
		{
		
		OLED_Fill(0x00);//������
		vTaskResume(AppTask_Handle);
			
		vTaskDelay(100);
			
		vTaskResume(AppTask_Handle1);
		}
		
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)==0)
		{
		
		OLED_ON();
		OLED_Fill(0x00);//������
		vTaskResume(AppTask_Handle);
			
		vTaskDelay(100);
			
		vTaskResume(AppTask_Handle1);	
		
	
		GPIO_ResetBits(GPIOE,GPIO_Pin_13);
		
		}
		else
		{
		GPIO_SetBits(GPIOE,GPIO_Pin_13);
		}
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2)==0)
		{
		OLED_Fill(0x00);//������
     	OLED_OFF();
		GPIO_ResetBits(GPIOE,GPIO_Pin_14);
		}
		else
		{
		GPIO_SetBits(GPIOE,GPIO_Pin_14);
		}
		
		
		
		vTaskDelay(500);
	}
	
	

}


static void AppTask3(void* parameter)
{
	
	int ledflag =0;  //0��  1��
	short x,y,z;
	float Gx,Gy,Gz;
	unsigned long step=0;
	vTaskDelay(20);
	while(0 != mpu_dmp_init()); // ֱ����ʼ���ɹ�Ϊֹ
	
	vTaskDelay(10);
	
	
	//���MPU6050�Ƿ� ����
	if((0x01<<6) & MPU_Read_Byte(MPU_PWR_MGMT1_REG))
		printf("����\r\n");
	
	//dmp_set_pedometer_step_count(100);
    while (1)
    {
		
		xSemaphoreTake(xMutex,portMAX_DELAY);//����
		
		
		MPU_Get_Accelerometer(&x,&y,&z);
		//dmp_get_pedometer_step_count(&step);
       // printf("STEP:%ld\r\n",step);
		
		
		mpu_dmp_get_data(&Gx,&Gy,&Gz);

		printf("%f\t%f\t\%f\r\n",Gx,Gy,Gz);
		
		if(Gx>-10)
		{
			GPIO_ResetBits(GPIOF,GPIO_Pin_10);
			
		}
		else if(Gx<-10)
		{
			GPIO_SetBits(GPIOF,GPIO_Pin_10);
		}
		
		xSemaphoreGive(xMutex);
        vTaskDelay(200);   /* ��ʱ500��tick */   
                
    }

}

uint16_t uart1_recv_data;

static void AppTask4(void* parameter)
{
	while(1)
	{
		if(uart1_recv_data == '0')
        {
			PEout(13) = ~PEout(13);
			uart1_recv_data = 4;
        }
        if(uart1_recv_data == '1')
        {	
			PEout(13) =1;
        }
        if(uart1_recv_data == '5')
        {	
			hour++;
			uart1_recv_data = 4;
			OLED_Fill(0x00);//������
			Oled_show_clock();
        }
		if(uart1_recv_data == '6')
        {	
			minute++;
			uart1_recv_data = 4;
			OLED_Fill(0x00);//������
			Oled_show_clock();
        }
		if(uart1_recv_data == '7')
        {	
			second++;
			uart1_recv_data = 4;
			OLED_Fill(0x00);//������
			Oled_show_clock();
        }
		if(uart1_recv_data == '8')
        {	
			PFout(8) = ~PFout(8) ;
			uart1_recv_data = 4;
        }
		delay_ms(100);
	}
}


void USART2_IRQHandler(void)
{
	
	//unsigned char RData;
	if(USART2->SR & (0x01<<5))
	{
		
		//��ȡDR
		setclock2[flagi] = USART2->DR;
		flagi++;
		if(flagi==7)
		{
				int setnao[3]={0};
				setnao[0] = (setclock2[1]-48)*10+setclock2[2]-48;
				setnao[1] = (setclock2[3]-48)*10+setclock2[4]-48;
				setnao[2] = (setclock2[5]-48)*10+setclock2[6]-48;
			if(setclock2[0] == 's')
				
			{
				RTC_SetAlarm_A(setnao[0],setnao[1],setnao[2]);//ʱ  ��  ��
				
//				if(setclock2[6] == 0)
//				{
//				OLED_Fill(0x00);
//				OLED_OFF();
//				LED_Control(2,1);
//				}
//				else if(setclock2[6] == 1)
//				{
//				OLED_Fill(0x00);
//				OLED_ON();
//				LED_Control(2,0);
//				}
				flagi=0;				
			}
			
			
		}
	}
}




int main(void)
{    
	xMutex = xSemaphoreCreateMutex();
	
	BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS ��ʵ��long���� */

	/* ������Ӳ����ʼ�� */
	USART1_Config();//��ʼ������1
	LED_Init();//LED��ʼ��

	IIC_Init();
	IIC2_Init();
	

	/* ����AppTaskCreate���� */
	xReturn = xTaskCreate((TaskFunction_t )AppTask,  /* ������ں��� */  //ʱ��
					(const char*    )"AppTask",/* �������� */
					(uint16_t       )256,  /* ����ջ��С */
					(void*          )NULL,/* ������ں������� */
					(UBaseType_t    )1, /* ��������ȼ� */
					(TaskHandle_t*  )&AppTask_Handle);/* ������ƿ�ָ�� ���߳���Ϣ��¼�ṹ��һ������˼ */ 
	
	xReturn = xTaskCreate((TaskFunction_t )AppTask1,  /* ������ں��� */  //����
					(const char*    )"AppTask1",/* �������� */
					(uint16_t       )256,  /* ����ջ��С */
					(void*          )NULL,/* ������ں������� */
					(UBaseType_t    )2, /* ��������ȼ� */
					(TaskHandle_t*  )&AppTask_Handle1);/* ������ƿ�ָ�� ���߳���Ϣ��¼�ṹ��һ������˼ */ 
					
					
	xReturn = xTaskCreate((TaskFunction_t )AppTask2,  /* ������ں��� */ //����
				(const char*    )"AppTask2",/* �������� */
				(uint16_t       )256,  /* ����ջ��С */
				(void*          )NULL,/* ������ں������� */
				(UBaseType_t    )0, /* ��������ȼ� */
				(TaskHandle_t*  )&AppTask_Handle2);/* ������ƿ�ָ�� ���߳���Ϣ��¼�ṹ��һ������˼ */ 
	xReturn = xTaskCreate((TaskFunction_t )AppTask3,  /* ������ں��� */ //x y z
				(const char*    )"AppTask3",/* �������� */
				(uint16_t       )256,  /* ����ջ��С */
				(void*          )NULL,/* ������ں������� */
				(UBaseType_t    )5, /* ��������ȼ� */
				(TaskHandle_t*  )&AppTask_Handle3);/* ������ƿ�ָ�� ���߳���Ϣ��¼�ṹ��һ������˼ */ 
				
				
					
	/* ����������� */           
	if(pdPASS == xReturn)
		vTaskStartScheduler();   /* �������񣬿������� */
	else
		return -1;  

	while(1);   /* ��������ִ�е����� */    
}


