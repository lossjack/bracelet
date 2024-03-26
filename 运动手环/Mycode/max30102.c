#include "max30102.h"

//#define SCL(n)   GPIO_WriteBit(GPIOA,GPIO_Pin_2,(BitAction)n)
//#define SDA_W(n) GPIO_WriteBit(GPIOA,GPIO_Pin_3,(BitAction)n)
//#define SDA_R    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)
//#include "iic.h"

////��Ĭ��IIC���ų�ʼ��
//void Max30102IIC_Init(void)
//{
//	GPIO_InitTypeDef GPIOInitStruct;//����һ��������ų�ʼ����Ϣ�Ľṹ��
//	
//	//ʹ��ʱ��--����AHB1ENR�Ĵ���
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//PE2 PE3 PE4  PE13 PE14

//	//����GPIOģ��ĺ����� GPIO_Init()
//	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_2 | GPIO_Pin_3;
//	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_OUT;//���ģʽ
//	GPIOInitStruct.GPIO_OType  = GPIO_OType_PP;//��©���
//	GPIOInitStruct.GPIO_Speed  = GPIO_Low_Speed;//����
//	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_UP;//��������
//	GPIO_Init(GPIOA,&GPIOInitStruct);
//}

//static void IIC_SDAModeSelect(GPIOMode_TypeDef Mode)
//{
//	GPIO_InitTypeDef GPIOInitStruct;//����һ��������ų�ʼ����Ϣ�Ľṹ��
//	
//	//ʹ��ʱ��--����AHB1ENR�Ĵ���
//	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//PA0
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//PE2 PE3 PE4  PE13 PE14
//	
//	
//	//����GPIOģ��ĺ����� GPIO_Init()
//	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_3;
//	GPIOInitStruct.GPIO_Mode   = Mode;//���ģʽ
//	GPIOInitStruct.GPIO_OType  = GPIO_OType_PP;//��©���
//	GPIOInitStruct.GPIO_Speed  = GPIO_Low_Speed;//����
//	GPIOInitStruct.GPIO_PuPd   = GPIO_PuPd_UP;//��������
//	GPIO_Init(GPIOA,&GPIOInitStruct);


#include "iic2.h"



u8 max30102_Bus_Write(u8 Register_Address, u8 Word_Data)
{
 
	/* ���ô���EEPROM�漴��ȡָ�����У�������ȡ�����ֽ� */
 
	/* ��1��������I2C���������ź� */
	IIC2_Start();
 
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	IIC2_WriteByte(max30102_WR_address | I2C_WR);	/* �˴���дָ�� */
 
	/* ��3��������ACK */
	if (IIC2_ReadAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
 
	/* ��4���������ֽڵ�ַ */
	IIC2_WriteByte(Register_Address);
	if (IIC2_ReadAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	
	/* ��5������ʼд������ */
	IIC2_WriteByte(Word_Data);
 
	/* ��6��������ACK */
	if (IIC2_ReadAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
 
	/* ����I2C����ֹͣ�ź� */
	IIC2_Stop();
	return 1;	/* ִ�гɹ� */
 
cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	IIC2_Stop();
	return 0;
}
 
 
 
u8 max30102_Bus_Read(u8 Register_Address)
{
	u8  data;
 
 
	/* ��1��������I2C���������ź� */
	IIC2_Start();
 
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	IIC2_WriteByte(max30102_WR_address | I2C_WR);	/* �˴���дָ�� */
 
	/* ��3��������ACK */
	if (IIC2_ReadAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
 
	/* ��4���������ֽڵ�ַ�� */
	IIC2_WriteByte((uint8_t)Register_Address);
	if (IIC2_ReadAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	
 
	/* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
	IIC2_Start();
 
	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	IIC2_WriteByte(max30102_WR_address | I2C_RD);	/* �˴��Ƕ�ָ�� */
 
	/* ��8��������ACK */
	if (IIC2_ReadAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
 
	/* ��9������ȡ���� */
	{
		data = IIC2_ReadByte();	/* ��1���ֽ� */
 
		IIC2_WriteAck(1);	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
	}
	/* ����I2C����ֹͣ�ź� */
	IIC2_Stop();
	return data;	/* ִ�гɹ� ����dataֵ */
 
cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	IIC2_Stop();
	return 0;
}
 
 
void max30102_FIFO_ReadWords(u8 Register_Address,u16 Word_Data[][2],u8 count)
{
	u8 i=0;
	u8 no = count;
	u8 data1, data2;
	/* ��1��������I2C���������ź� */
	IIC2_Start();
 
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	IIC2_WriteByte(max30102_WR_address | I2C_WR);	/* �˴���дָ�� */
 
	/* ��3��������ACK */
	if (IIC2_ReadAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
 
	/* ��4���������ֽڵ�ַ�� */
	IIC2_WriteByte((uint8_t)Register_Address);
	if (IIC2_ReadAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	
 
	/* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
	IIC2_Start();
 
	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	IIC2_WriteByte(max30102_WR_address | I2C_RD);	/* �˴��Ƕ�ָ�� */
 
	/* ��8��������ACK */
	if (IIC2_ReadAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
 
	/* ��9������ȡ���� */
	while (no)
	{
		data1 = IIC2_ReadByte();	
		IIC2_WriteAck(0);
		data2 = IIC2_ReadByte();
		IIC2_WriteAck(0);
		Word_Data[i][0] = (((u16)data1 << 8) | data2);  //
 
		
		data1 = IIC2_ReadByte();	
		IIC2_WriteAck(0);
		data2 = IIC2_ReadByte();
		if(1==no)
			IIC2_WriteAck(1);	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
		else
			IIC2_WriteAck(0);
		Word_Data[i][1] = (((u16)data1 << 8) | data2); 
 
		no--;	
		i++;
	}
	/* ����I2C����ֹͣ�ź� */
	IIC2_Stop();
 
cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	IIC2_Stop();
}
 
void max30102_FIFO_ReadBytes(u8 Register_Address,u8* Data)
{	
	max30102_Bus_Read(REG_INTR_STATUS_1);
	max30102_Bus_Read(REG_INTR_STATUS_2);
	
	/* ��1��������I2C���������ź� */
	IIC2_Start();
 
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	IIC2_WriteByte(max30102_WR_address | I2C_WR);	/* �˴���дָ�� */
 
	/* ��3��������ACK */
	if (IIC2_ReadAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
 
	/* ��4���������ֽڵ�ַ�� */
	IIC2_WriteByte((uint8_t)Register_Address);
	if (IIC2_ReadAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	
 
	/* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
	IIC2_Start();
 
	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	IIC2_WriteByte(max30102_WR_address | I2C_RD);	/* �˴��Ƕ�ָ�� */
 
	/* ��8��������ACK */
	if (IIC2_ReadAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
 
	/* ��9������ȡ���� */
	Data[0] = IIC2_ReadByte();
	IIC2_WriteAck(0);	
	Data[1] = IIC2_ReadByte();
	IIC2_WriteAck(0);	
	Data[2] = IIC2_ReadByte();
	IIC2_WriteAck(0);	
	Data[3] = IIC2_ReadByte();
	IIC2_WriteAck(0);
	Data[4] = IIC2_ReadByte();
	IIC2_WriteAck(0);	
	Data[5] = IIC2_ReadByte();
	IIC2_WriteAck(1);
	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
	/* ����I2C����ֹͣ�ź� */
	IIC2_Stop();
 
cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	IIC2_Stop();

}
 
void max30102_Bus_WriteBytes(u8 WriteAddr,u8* data,u8 dataLength)
{		
	u8 i;	
    IIC2_Start();  
 
	IIC2_WriteByte(max30102_WR_address);	    //����д����
	IIC2_ReadAck();
	
	IIC2_WriteByte(WriteAddr);	    //����д����
	IIC2_ReadAck();
	
	for(i=0;i<dataLength;i++)
	{
		IIC2_WriteByte(data[i]);
		IIC2_ReadAck();
	}				    	   
    IIC2_Stop();//����һ��ֹͣ���� 
	delay_ms(10);	 
}
 
void max30102_Bus_ReadBytes(u8 writeAddr,u8* data,u8 dataLength)
{		
	u8 i;	
    IIC2_Start();  
 
	IIC2_WriteByte(max30102_WR_address);	    //����д����
	IIC2_ReadAck();
	IIC2_WriteByte(writeAddr);
	IIC2_ReadAck();
	
	IIC2_Start(); 
	IIC2_WriteByte(max30102_WR_address|0X01);//�������ģʽ			   
	IIC2_ReadAck();
	
	for(i=0;i<dataLength-1;i++)
	{
		data[i] = IIC2_ReadByte();
		IIC2_WriteAck(0);
	}		
	data[dataLength-1] = IIC2_ReadByte();
	IIC2_WriteAck(1);
    IIC2_Stop();//����һ��ֹͣ���� 
	delay_ms(10);	 
}

void max30102_init(void)
{
	GPIO_InitTypeDef GPIOInitStruct;//����һ��������ų�ʼ����Ϣ�Ľṹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);//PE2 PE3 PE4  PE13 PE14

	GPIOInitStruct.GPIO_Pin    = GPIO_Pin_10;
	GPIOInitStruct.GPIO_Mode   = GPIO_Mode_IN;//���ģʽ
	GPIO_Init(GPIOB,&GPIOInitStruct);
	
	max30102_reset();
	
	max30102_Bus_Write(REG_INTR_ENABLE_1,0xc0);	// INTR setting
	max30102_Bus_Write(REG_INTR_ENABLE_2,0x00);
	max30102_Bus_Write(REG_FIFO_WR_PTR,0x00);  	//FIFO_WR_PTR[4:0]
	max30102_Bus_Write(REG_OVF_COUNTER,0x00);  	//OVF_COUNTER[4:0]
	max30102_Bus_Write(REG_FIFO_RD_PTR,0x00);  	//FIFO_RD_PTR[4:0]
	max30102_Bus_Write(REG_FIFO_CONFIG,0x0f);  	//sample avg = 1, fifo rollover=false, fifo almost full = 17
	max30102_Bus_Write(REG_MODE_CONFIG,0x03);  	//0x02 for Red only, 0x03 for SpO2 mode 0x07 multimode LED
	max30102_Bus_Write(REG_SPO2_CONFIG,0x27);  	// SPO2_ADC range = 4096nA, SPO2 sample rate (100 Hz), LED pulseWidth (400uS)  
	max30102_Bus_Write(REG_LED1_PA,0x24);   	//Choose value for ~ 7mA for LED1
	max30102_Bus_Write(REG_LED2_PA,0x24);   	// Choose value for ~ 7mA for LED2
	max30102_Bus_Write(REG_PILOT_PA,0x7f);   	// Choose value for ~ 25mA for Pilot LED
 
 
}
 
void max30102_reset(void)
{
	max30102_Bus_Write(REG_MODE_CONFIG,0x40);
	max30102_Bus_Write(REG_MODE_CONFIG,0x40);
}
 
 
 
 
 
 
void maxim_max30102_write_reg(uint8_t uch_addr, uint8_t uch_data)
{
	max30102_Bus_Write(uch_addr,uch_data);
}
 
void maxim_max30102_read_reg(uint8_t uch_addr, uint8_t *puch_data)
{
	*puch_data = max30102_Bus_Read(uch_addr);
}
 
void maxim_max30102_read_fifo(uint32_t *pun_red_led, uint32_t *pun_ir_led)
{
	uint32_t un_temp;
	unsigned char uch_temp;
	char ach_i2c_data[6];
	*pun_red_led=0;
	*pun_ir_led=0;
 
  
  //read and clear status register
  maxim_max30102_read_reg(REG_INTR_STATUS_1, &uch_temp);
  maxim_max30102_read_reg(REG_INTR_STATUS_2, &uch_temp);
  
  max30102_Bus_ReadBytes(REG_FIFO_DATA,(u8 *)ach_i2c_data,6);
  
  un_temp=(unsigned char) ach_i2c_data[0];
  un_temp<<=16;
  *pun_red_led+=un_temp;
  un_temp=(unsigned char) ach_i2c_data[1];
  un_temp<<=8;
  *pun_red_led+=un_temp;
  un_temp=(unsigned char) ach_i2c_data[2];
  *pun_red_led+=un_temp;
  
  un_temp=(unsigned char) ach_i2c_data[3];
  un_temp<<=16;
  *pun_ir_led+=un_temp;
  un_temp=(unsigned char) ach_i2c_data[4];
  un_temp<<=8;
  *pun_ir_led+=un_temp;
  un_temp=(unsigned char) ach_i2c_data[5];
  *pun_ir_led+=un_temp;
  *pun_red_led&=0x03FFFF;  //Mask MSB [23:18]
  *pun_ir_led&=0x03FFFF;  //Mask MSB [23:18]
}













const uint16_t auw_hamm[31]={ 41,    276,    512,    276,     41 }; //Hamm=  long16(512* hamming(5)');
//uch_spo2_table is computed as  -45.060*ratioAverage* ratioAverage + 30.354 *ratioAverage + 94.845 ;
const uint8_t uch_spo2_table[184]={ 95, 95, 95, 96, 96, 96, 97, 97, 97, 97, 97, 98, 98, 98, 98, 98, 99, 99, 99, 99, 
                            99, 99, 99, 99, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 
                            100, 100, 100, 100, 99, 99, 99, 99, 99, 99, 99, 99, 98, 98, 98, 98, 98, 98, 97, 97, 
                            97, 97, 96, 96, 96, 96, 95, 95, 95, 94, 94, 94, 93, 93, 93, 92, 92, 92, 91, 91, 
                            90, 90, 89, 89, 89, 88, 88, 87, 87, 86, 86, 85, 85, 84, 84, 83, 82, 82, 81, 81, 
                            80, 80, 79, 78, 78, 77, 76, 76, 75, 74, 74, 73, 72, 72, 71, 70, 69, 69, 68, 67, 
                            66, 66, 65, 64, 63, 62, 62, 61, 60, 59, 58, 57, 56, 56, 55, 54, 53, 52, 51, 50, 
                            49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 31, 30, 29, 
                            28, 27, 26, 25, 23, 22, 21, 20, 19, 17, 16, 15, 14, 12, 11, 10, 9, 7, 6, 5, 
                            3, 2, 1 } ;
static  int32_t an_dx[ BUFFER_SIZE-MA4_SIZE]; // delta
static  int32_t an_x[ BUFFER_SIZE]; //ir
static  int32_t an_y[ BUFFER_SIZE]; //red
 
void maxim_heart_rate_and_oxygen_saturation(uint32_t *pun_ir_buffer,  int32_t n_ir_buffer_length, uint32_t *pun_red_buffer, int32_t *pn_spo2, int8_t *pch_spo2_valid, 
                              int32_t *pn_heart_rate, int8_t  *pch_hr_valid)
/**
* \brief        Calculate the heart rate and SpO2 level
* \par          Details
*               By detecting  peaks of PPG cycle and corresponding AC/DC of red/infra-red signal, the ratio for the SPO2 is computed.
*               Since this algorithm is aiming for Arm M0/M3. formaula for SPO2 did not achieve the accuracy due to register overflow.
*               Thus, accurate SPO2 is precalculated and save longo uch_spo2_table[] per each ratio.
*
* \param[in]    *pun_ir_buffer           - IR sensor data buffer
* \param[in]    n_ir_buffer_length      - IR sensor data buffer length
* \param[in]    *pun_red_buffer          - Red sensor data buffer
* \param[out]    *pn_spo2                - Calculated SpO2 value
* \param[out]    *pch_spo2_valid         - 1 if the calculated SpO2 value is valid
* \param[out]    *pn_heart_rate          - Calculated heart rate value
* \param[out]    *pch_hr_valid           - 1 if the calculated heart rate value is valid
*
* \retval       None
*/
{
    uint32_t un_ir_mean ,un_only_once ;
    int32_t k ,n_i_ratio_count;
    int32_t i, s, m, n_exact_ir_valley_locs_count ,n_middle_idx;
    int32_t n_th1, n_npks,n_c_min;      
    int32_t an_ir_valley_locs[15] ;
    int32_t an_exact_ir_valley_locs[15] ;
    int32_t an_dx_peak_locs[15] ;
    int32_t n_peak_interval_sum;
    
    int32_t n_y_ac, n_x_ac;
    int32_t n_spo2_calc; 
    int32_t n_y_dc_max, n_x_dc_max; 
    int32_t n_y_dc_max_idx, n_x_dc_max_idx; 
    int32_t an_ratio[5],n_ratio_average; 
    int32_t n_nume,  n_denom ;
    // remove DC of ir signal    
    un_ir_mean =0; 
    for (k=0 ; k<n_ir_buffer_length ; k++ ) un_ir_mean += pun_ir_buffer[k] ;
    un_ir_mean =un_ir_mean/n_ir_buffer_length ;
    for (k=0 ; k<n_ir_buffer_length ; k++ )  an_x[k] =  pun_ir_buffer[k] - un_ir_mean ; 
    
    // 4 pt Moving Average
    for(k=0; k< BUFFER_SIZE-MA4_SIZE; k++){
        n_denom= ( an_x[k]+an_x[k+1]+ an_x[k+2]+ an_x[k+3]);
        an_x[k]=  n_denom/(int32_t)4; 
    }
 
    // get difference of smoothed IR signal
    
    for( k=0; k<BUFFER_SIZE-MA4_SIZE-1;  k++)
        an_dx[k]= (an_x[k+1]- an_x[k]);
 
    // 2-pt Moving Average to an_dx
    for(k=0; k< BUFFER_SIZE-MA4_SIZE-2; k++){
        an_dx[k] =  ( an_dx[k]+an_dx[k+1])/2 ;
    }
    
    // hamming window
    // flip wave form so that we can detect valley with peak detector
    for ( i=0 ; i<BUFFER_SIZE-HAMMING_SIZE-MA4_SIZE-2 ;i++){
        s= 0;
        for( k=i; k<i+ HAMMING_SIZE ;k++){
            s -= an_dx[k] *auw_hamm[k-i] ; 
                     }
        an_dx[i]= s/ (int32_t)1146; // divide by sum of auw_hamm 
    }
 
 
    n_th1=0; // threshold calculation
    for ( k=0 ; k<BUFFER_SIZE-HAMMING_SIZE ;k++){
        n_th1 += ((an_dx[k]>0)? an_dx[k] : ((int32_t)0-an_dx[k])) ;
    }
    n_th1= n_th1/ ( BUFFER_SIZE-HAMMING_SIZE);
    // peak location is acutally index for sharpest location of raw signal since we flipped the signal         
    maxim_find_peaks( an_dx_peak_locs, &n_npks, an_dx, BUFFER_SIZE-HAMMING_SIZE, n_th1, 8, 5 );//peak_height, peak_distance, max_num_peaks 
 
    n_peak_interval_sum =0;
    if (n_npks>=2){
        for (k=1; k<n_npks; k++)
            n_peak_interval_sum += (an_dx_peak_locs[k]-an_dx_peak_locs[k -1]);
        n_peak_interval_sum=n_peak_interval_sum/(n_npks-1);
        *pn_heart_rate=(int32_t)(6000/n_peak_interval_sum);// beats per minutes
        *pch_hr_valid  = 1;
    }
    else  {
        *pn_heart_rate = -999;
        *pch_hr_valid  = 0;
    }
            
    for ( k=0 ; k<n_npks ;k++)
        an_ir_valley_locs[k]=an_dx_peak_locs[k]+HAMMING_SIZE/2; 
 
 
    // raw value : RED(=y) and IR(=X)
    // we need to assess DC and AC value of ir and red PPG. 
    for (k=0 ; k<n_ir_buffer_length ; k++ )  {
        an_x[k] =  pun_ir_buffer[k] ; 
        an_y[k] =  pun_red_buffer[k] ; 
    }
 
    // find precise min near an_ir_valley_locs
    n_exact_ir_valley_locs_count =0; 
    for(k=0 ; k<n_npks ;k++){
        un_only_once =1;
        m=an_ir_valley_locs[k];
        n_c_min= 16777216;//2^24;
        if (m+5 <  BUFFER_SIZE-HAMMING_SIZE  && m-5 >0){
            for(i= m-5;i<m+5; i++)
                if (an_x[i]<n_c_min){
                    if (un_only_once >0){
                       un_only_once =0;
                   } 
                   n_c_min= an_x[i] ;
                   an_exact_ir_valley_locs[k]=i;
                }
            if (un_only_once ==0)
                n_exact_ir_valley_locs_count ++ ;
        }
    }
    if (n_exact_ir_valley_locs_count <2 ){
       *pn_spo2 =  -999 ; // do not use SPO2 since signal ratio is out of range
       *pch_spo2_valid  = 0; 
       return;
    }
    // 4 pt MA
    for(k=0; k< BUFFER_SIZE-MA4_SIZE; k++){
        an_x[k]=( an_x[k]+an_x[k+1]+ an_x[k+2]+ an_x[k+3])/(int32_t)4;
        an_y[k]=( an_y[k]+an_y[k+1]+ an_y[k+2]+ an_y[k+3])/(int32_t)4;
    }
 
    //using an_exact_ir_valley_locs , find ir-red DC andir-red AC for SPO2 calibration ratio
    //finding AC/DC maximum of raw ir * red between two valley locations
    n_ratio_average =0; 
    n_i_ratio_count =0; 
    
    for(k=0; k< 5; k++) an_ratio[k]=0;
    for (k=0; k< n_exact_ir_valley_locs_count; k++){
        if (an_exact_ir_valley_locs[k] > BUFFER_SIZE ){             
            *pn_spo2 =  -999 ; // do not use SPO2 since valley loc is out of range
            *pch_spo2_valid  = 0; 
            return;
        }
    }
    // find max between two valley locations 
    // and use ratio betwen AC compoent of Ir & Red and DC compoent of Ir & Red for SPO2 
 
    for (k=0; k< n_exact_ir_valley_locs_count-1; k++){
        n_y_dc_max= -16777216 ; 
        n_x_dc_max= - 16777216; 
        if (an_exact_ir_valley_locs[k+1]-an_exact_ir_valley_locs[k] >10){
            for (i=an_exact_ir_valley_locs[k]; i< an_exact_ir_valley_locs[k+1]; i++){
                if (an_x[i]> n_x_dc_max) {n_x_dc_max =an_x[i];n_x_dc_max_idx =i; }
                if (an_y[i]> n_y_dc_max) {n_y_dc_max =an_y[i];n_y_dc_max_idx=i;}
            }
            n_y_ac= (an_y[an_exact_ir_valley_locs[k+1]] - an_y[an_exact_ir_valley_locs[k] ] )*(n_y_dc_max_idx -an_exact_ir_valley_locs[k]); //red
            n_y_ac=  an_y[an_exact_ir_valley_locs[k]] + n_y_ac/ (an_exact_ir_valley_locs[k+1] - an_exact_ir_valley_locs[k])  ; 
        
        
            n_y_ac=  an_y[n_y_dc_max_idx] - n_y_ac;    // subracting linear DC compoenents from raw 
            n_x_ac= (an_x[an_exact_ir_valley_locs[k+1]] - an_x[an_exact_ir_valley_locs[k] ] )*(n_x_dc_max_idx -an_exact_ir_valley_locs[k]); // ir
            n_x_ac=  an_x[an_exact_ir_valley_locs[k]] + n_x_ac/ (an_exact_ir_valley_locs[k+1] - an_exact_ir_valley_locs[k]); 
            n_x_ac=  an_x[n_y_dc_max_idx] - n_x_ac;      // subracting linear DC compoenents from raw 
            n_nume=( n_y_ac *n_x_dc_max)>>7 ; //prepare X100 to preserve floating value
            n_denom= ( n_x_ac *n_y_dc_max)>>7;
            if (n_denom>0  && n_i_ratio_count <5 &&  n_nume != 0)
            {   
                an_ratio[n_i_ratio_count]= (n_nume*20)/n_denom ; //formular is ( n_y_ac *n_x_dc_max) / ( n_x_ac *n_y_dc_max) ;  ///*************************n_numeԭ����*100************************//
                n_i_ratio_count++;
            }
        }
    }
 
    maxim_sort_ascend(an_ratio, n_i_ratio_count);
    n_middle_idx= n_i_ratio_count/2;
 
    if (n_middle_idx >1)
        n_ratio_average =( an_ratio[n_middle_idx-1] +an_ratio[n_middle_idx])/2; // use median
    else
        n_ratio_average = an_ratio[n_middle_idx ];
 
    if( n_ratio_average>2 && n_ratio_average <184){
        n_spo2_calc= uch_spo2_table[n_ratio_average] ;
        *pn_spo2 = n_spo2_calc ;
        *pch_spo2_valid  = 1;//  float_SPO2 =  -45.060*n_ratio_average* n_ratio_average/10000 + 30.354 *n_ratio_average/100 + 94.845 ;  // for comparison with table
    }
    else{
        *pn_spo2 =  -999 ; // do not use SPO2 since signal ratio is out of range
        *pch_spo2_valid  = 0; 
    }
}
 
 
void maxim_find_peaks(int32_t *pn_locs, int32_t *pn_npks, int32_t *pn_x, int32_t n_size, int32_t n_min_height, int32_t n_min_distance, int32_t n_max_num)
/**
* \brief        Find peaks
* \par          Details
*               Find at most MAX_NUM peaks above MIN_HEIGHT separated by at least MIN_DISTANCE
*
* \retval       None
*/
{
    maxim_peaks_above_min_height( pn_locs, pn_npks, pn_x, n_size, n_min_height );
    maxim_remove_close_peaks( pn_locs, pn_npks, pn_x, n_min_distance );
    *pn_npks = min( *pn_npks, n_max_num );
}
 
void maxim_peaks_above_min_height(int32_t *pn_locs, int32_t *pn_npks, int32_t  *pn_x, int32_t n_size, int32_t n_min_height)
/**
* \brief        Find peaks above n_min_height
* \par          Details
*               Find all peaks above MIN_HEIGHT
*
* \retval       None
*/
{
    int32_t i = 1, n_width;
    *pn_npks = 0;
    
    while (i < n_size-1){
        if (pn_x[i] > n_min_height && pn_x[i] > pn_x[i-1]){            // find left edge of potential peaks
            n_width = 1;
            while (i+n_width < n_size && pn_x[i] == pn_x[i+n_width])    // find flat peaks
                n_width++;
            if (pn_x[i] > pn_x[i+n_width] && (*pn_npks) < 15 ){                            // find right edge of peaks
                pn_locs[(*pn_npks)++] = i;        
                // for flat peaks, peak location is left edge
                i += n_width+1;
            }
            else
                i += n_width;
        }
        else
            i++;
    }
}
 
 
void maxim_remove_close_peaks(int32_t *pn_locs, int32_t *pn_npks, int32_t *pn_x, int32_t n_min_distance)
/**
* \brief        Remove peaks
* \par          Details
*               Remove peaks separated by less than MIN_DISTANCE
*
* \retval       None
*/
{
    
    int32_t i, j, n_old_npks, n_dist;
    
    /* Order peaks from large to small */
    maxim_sort_indices_descend( pn_x, pn_locs, *pn_npks );
 
    for ( i = -1; i < *pn_npks; i++ ){
        n_old_npks = *pn_npks;
        *pn_npks = i+1;
        for ( j = i+1; j < n_old_npks; j++ ){
            n_dist =  pn_locs[j] - ( i == -1 ? -1 : pn_locs[i] ); // lag-zero peak of autocorr is at index -1
            if ( n_dist > n_min_distance || n_dist < -n_min_distance )
                pn_locs[(*pn_npks)++] = pn_locs[j];
        }
    }
 
    // Resort indices longo ascending order
    maxim_sort_ascend( pn_locs, *pn_npks );
}
 
void maxim_sort_ascend(int32_t *pn_x,int32_t n_size) 
/**
* \brief        Sort array
* \par          Details
*               Sort array in ascending order (insertion sort algorithm)
*
* \retval       None
*/
{
    int32_t i, j, n_temp;
    for (i = 1; i < n_size; i++) {
        n_temp = pn_x[i];
        for (j = i; j > 0 && n_temp < pn_x[j-1]; j--)
            pn_x[j] = pn_x[j-1];
        pn_x[j] = n_temp;
    }
}
 
void maxim_sort_indices_descend(int32_t *pn_x, int32_t *pn_indx, int32_t n_size)
/**
* \brief        Sort indices
* \par          Details
*               Sort indices according to descending order (insertion sort algorithm)
*
* \retval       None
*/ 
{
    int32_t i, j, n_temp;
    for (i = 1; i < n_size; i++) {
        n_temp = pn_indx[i];
        for (j = i; j > 0 && pn_x[n_temp] > pn_x[pn_indx[j-1]]; j--)
            pn_indx[j] = pn_indx[j-1];
        pn_indx[j] = n_temp;
    }
}



