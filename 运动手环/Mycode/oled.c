#include "oled.h"
#include "oled_font.h"

#define delay_ms(n) RTOS_Delay_ms(n)


int OLED_Write(unsigned char CtrByte,unsigned char WData)
{
	IIC_Start();
	
	IIC_WriteByte(OLED_SlaveAddr<<1);//发送写模式设备地址
	if(1 == IIC_ReadAck())
	{
		return -1;
	}
	
	IIC_WriteByte(CtrByte);//写命令0x00 还是写数据0x40由外部传参决定
	if(1 == IIC_ReadAck())
	{
		return -2;
	}
	
	IIC_WriteByte(WData);//写命令还是写图像数据由上面CtrByte决定
	if(1 == IIC_ReadAck())
	{
		return -3;
	}

	IIC_Stop();
	return 0;//代表当前这轮写成功
}

void OLED_WriteCmd(unsigned char I2C_Command)//写命令
{
	OLED_Write(0x00, I2C_Command);
}

void OLED_WriteDat(unsigned char I2C_Data)//写数据
{
	OLED_Write(0x40, I2C_Data);
}



///////////以下函数会被外部使用/////////////////
void OLED_Init(void)
{
	RTOS_Delay_ms(100); //这里的延时很重要
	
	OLED_WriteCmd(0xAE); //display off
	OLED_WriteCmd(0x20);	//Set Memory Addressing Mode	
	OLED_WriteCmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	OLED_WriteCmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	OLED_WriteCmd(0xc8);	//Set COM Output Scan Direction
	OLED_WriteCmd(0x00); //---set low column address
	OLED_WriteCmd(0x10); //---set high column address
	OLED_WriteCmd(0x40); //--set start line address
	OLED_WriteCmd(0x81); //--set contrast control register
	OLED_WriteCmd(0xff); //对比度调节 255 最大0x00~0xff
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


//屏幕光标定位函数---定显示图像光标在哪
//x：0~127
//y: 0~7   因为OLED的显示方式不是逐行式  --- y是PAGE0~PAGE7
void OLED_SetPos(unsigned char x, unsigned char y) //设置起始点坐标
{ 
	OLED_WriteCmd(0xb0+y);//设定第几页
	
	OLED_WriteCmd(((x&0xf0)>>4)|0x10);//设定是第几列
	OLED_WriteCmd((x&0x0f)|0x01);
}

//全屏填充为指定的数据图像
//参数1:fill_Data  0xFF--全屏白色    0x00--全屏黑色-息屏
void OLED_Fill(unsigned char fill_Data)//全屏填充
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		//每次换页都要重新设置坐标
		OLED_WriteCmd(0xb0+m);		//page0-page1
		OLED_WriteCmd(0x00);		//low column start address
		OLED_WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
		{
			OLED_WriteDat(fill_Data);//当前页内一直写 会自动往后偏移
		}
	}
}



void OLED_ON(void)
{
	OLED_WriteCmd(0X8D);  //设置电荷泵
	OLED_WriteCmd(0X14);  //开启电荷泵
	OLED_WriteCmd(0XAF);  //OLED唤醒
}

void OLED_OFF(void)
{
	OLED_WriteCmd(0X8D);  //设置电荷泵
	OLED_WriteCmd(0X10);  //关闭电荷泵
	OLED_WriteCmd(0XAE);  //OLED休眠
}


void OLED_ShowNum(void)
{
	//先设置位置
	OLED_SetPos(0,0);
	
	OLED_WriteDat(0x00);
	OLED_WriteDat(0x00);
	OLED_WriteDat(0x82);
	OLED_WriteDat(0xFF);
	OLED_WriteDat(0x80);
	OLED_WriteDat(0x00);

}



//可以在屏幕的指定位置显示指定字符
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

