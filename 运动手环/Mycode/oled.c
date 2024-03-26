#include "oled.h"
#include "oled_font.h"

#define delay_ms(n) RTOS_Delay_ms(n)


int OLED_Write(unsigned char CtrByte,unsigned char WData)
{
	IIC_Start();
	
	IIC_WriteByte(OLED_SlaveAddr<<1);//����дģʽ�豸��ַ
	if(1 == IIC_ReadAck())
	{
		return -1;
	}
	
	IIC_WriteByte(CtrByte);//д����0x00 ����д����0x40���ⲿ���ξ���
	if(1 == IIC_ReadAck())
	{
		return -2;
	}
	
	IIC_WriteByte(WData);//д�����дͼ������������CtrByte����
	if(1 == IIC_ReadAck())
	{
		return -3;
	}

	IIC_Stop();
	return 0;//����ǰ����д�ɹ�
}

void OLED_WriteCmd(unsigned char I2C_Command)//д����
{
	OLED_Write(0x00, I2C_Command);
}

void OLED_WriteDat(unsigned char I2C_Data)//д����
{
	OLED_Write(0x40, I2C_Data);
}



///////////���º����ᱻ�ⲿʹ��/////////////////
void OLED_Init(void)
{
	RTOS_Delay_ms(100); //�������ʱ����Ҫ
	
	OLED_WriteCmd(0xAE); //display off
	OLED_WriteCmd(0x20);	//Set Memory Addressing Mode	
	OLED_WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	OLED_WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	OLED_WriteCmd(0xc8);	//Set COM Output Scan Direction
	OLED_WriteCmd(0x00); //---set low column address
	OLED_WriteCmd(0x10); //---set high column address
	OLED_WriteCmd(0x40); //--set start line address
	OLED_WriteCmd(0x81); //--set contrast control register
	OLED_WriteCmd(0xff); //�Աȶȵ��� 255 ���0x00~0xff
	OLED_WriteCmd(0xa1); //--set segment re-map 0 to 127
	OLED_WriteCmd(0xa6); //--set normal display
	OLED_WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	OLED_WriteCmd(0x3F); //
	OLED_WriteCmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	OLED_WriteCmd(0xd3); //-set display offset
	OLED_WriteCmd(0x00); //-not offset
	OLED_WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	OLED_WriteCmd(0xf0); //--set divide ratio
	OLED_WriteCmd(0xd9); //--set pre-charge period
	OLED_WriteCmd(0x22); //
	OLED_WriteCmd(0xda); //--set com pins hardware configuration
	OLED_WriteCmd(0x12);
	OLED_WriteCmd(0xdb); //--set vcomh
	OLED_WriteCmd(0x20); //0x20,0.77xVcc
	OLED_WriteCmd(0x8d); //--set DC-DC enable
	OLED_WriteCmd(0x14); //
	OLED_WriteCmd(0xAF); //--turn on oled panel
}


//��Ļ��궨λ����---����ʾͼ��������
//x��0~127
//y: 0~7   ��ΪOLED����ʾ��ʽ��������ʽ  --- y��PAGE0~PAGE7
void OLED_SetPos(unsigned char x, unsigned char y) //������ʼ������
{ 
	OLED_WriteCmd(0xb0+y);//�趨�ڼ�ҳ
	
	OLED_WriteCmd(((x&0xf0)>>4)|0x10);//�趨�ǵڼ���
	OLED_WriteCmd((x&0x0f)|0x01);
}

//ȫ�����Ϊָ��������ͼ��
//����1:fill_Data  0xFF--ȫ����ɫ    0x00--ȫ����ɫ-Ϣ��
void OLED_Fill(unsigned char fill_Data)//ȫ�����
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		//ÿ�λ�ҳ��Ҫ������������
		OLED_WriteCmd(0xb0+m);		//page0-page1
		OLED_WriteCmd(0x00);		//low column start address
		OLED_WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
		{
			OLED_WriteDat(fill_Data);//��ǰҳ��һֱд ���Զ�����ƫ��
		}
	}
}



void OLED_ON(void)
{
	OLED_WriteCmd(0X8D);  //���õ�ɱ�
	OLED_WriteCmd(0X14);  //������ɱ�
	OLED_WriteCmd(0XAF);  //OLED����
}

void OLED_OFF(void)
{
	OLED_WriteCmd(0X8D);  //���õ�ɱ�
	OLED_WriteCmd(0X10);  //�رյ�ɱ�
	OLED_WriteCmd(0XAE);  //OLED����
}


void OLED_ShowNum(void)
{
	//������λ��
	OLED_SetPos(0,0);
	
	OLED_WriteDat(0x00);
	OLED_WriteDat(0x00);
	OLED_WriteDat(0x82);
	OLED_WriteDat(0xFF);
	OLED_WriteDat(0x80);
	OLED_WriteDat(0x00);

}



//��������Ļ��ָ��λ����ʾָ���ַ�
void OLED_ShowChar(int x,int y,char Chr,int fontsize)
{
	OLED_SetPos(x,y);
	for(int i=0;i<6;i++)
	{
		OLED_WriteDat(F6x8[Chr-32][i]);
	}
}
void OLED_ShowLChar(int x,int y,char *Chr)
{
	int i=x;
	for(;*Chr!='\0';Chr++)
	{
		OLED_ShowChar(x,y,*Chr,0);
		x+=6;
		if(x>128)
		{
			x=i;
			y+=1;
		}
		
	}
}

