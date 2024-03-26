#include "at24c02.h"

#define AT24C02_Addr  0x50 //0101 0000  <<1 1010 0000  0xA0  0xA1
 

//使用IIC将一个字节写入24C02的某个位置存放
int AT24C02_WriteByte(unsigned char Addr,unsigned char WData)
{
	IIC_Start();
	
	IIC_WriteByte(AT24C02_Addr<<1);//①先发送设备地址
	if(1 == IIC_ReadAck())
	{
		return -1;//这里提前结束意味着从机无应答
	}
	
	
	
	IIC_WriteByte(Addr);//②再发送要写到这款储存芯片哪个位置
	if(1 == IIC_ReadAck())
	{
		return -2;//这里提前结束意味着从机无应答
	}
	
	
	IIC_WriteByte(WData);//③将一个你指定8bit数据存入上面指定Addr地址处
	if(1 == IIC_ReadAck())
	{
		return -3;//这里提前结束意味着从机无应答
	}
	
	
	IIC_Stop();
	return 0;//代表成功
}



int AT24C02_ReadByte(unsigned char Addr,unsigned char *RData)
{
	IIC_Start();
	IIC_WriteByte(AT24C02_Addr<<1);//发送写模式设备地址
	if(1 == IIC_ReadAck())
	{
		return -1;//这里提前结束意味着从机无应答
	}
	
	IIC_WriteByte(Addr);//发送写模式设备地址
	if(1 == IIC_ReadAck())
	{
		return -2;//这里提前结束意味着从机无应答
	}
	
	IIC_Start();//重新启动
	
	IIC_WriteByte((AT24C02_Addr<<1) | 1);//发送读模式设备地址 0xA1
	if(1 == IIC_ReadAck())
	{
		return -3;//这里提前结束意味着从机无应答
	}
	
	*RData = IIC_ReadByte();//读取8bit数据  用外部传入容器接住
	IIC_WriteAck(1);//给从机发送无应答
	
	IIC_Stop();
	
	return 0;
}


//AT24C02页写函数
//Addr:传入的页写的起始地址 0~255 0x00~0xFF
//WData:要写入Addr处的若干字节的起始地址
//n:确定要从WData开始写入几个字节到芯片
//write(fd,"hello",3)
//  16~23 Addr=18  n<=6
int AT24C02_PageWrite(unsigned char Addr,unsigned char *WData,int n)
{
	IIC_Start();
	
	IIC_WriteByte(AT24C02_Addr<<1);//①先发送设备地址
	if(1 == IIC_ReadAck())
	{
		return -1;//这里提前结束意味着从机无应答
	}
	
	
	
	IIC_WriteByte(Addr);//②再发送要写到这款储存芯片哪个位置
	if(1 == IIC_ReadAck())
	{
		return -2;//这里提前结束意味着从机无应答
	}
	
	
	//循环从WData地址开始发送n个字节到芯片
	while(n--)
	{
		IIC_WriteByte(*WData++);//②再发送要写到这款储存芯片哪个位置
		if(1 == IIC_ReadAck())
		{
			return -3;//这里提前结束意味着从机无应答
		}
	}
	IIC_Stop();
	return 0;
}



int AT24C02_SeriesRead(unsigned char Addr,unsigned char *RData,int n)
{
	IIC_Start();
	IIC_WriteByte(AT24C02_Addr<<1);//发送写模式设备地址
	if(1 == IIC_ReadAck())
	{
		return -1;//这里提前结束意味着从机无应答
	}
	
	IIC_WriteByte(Addr);//发送写模式设备地址
	if(1 == IIC_ReadAck())
	{
		return -2;//这里提前结束意味着从机无应答
	}
	
	IIC_Start();//重新启动
	
	IIC_WriteByte((AT24C02_Addr<<1) | 1);//发送读模式设备地址 0xA1
	if(1 == IIC_ReadAck())
	{
		return -3;//这里提前结束意味着从机无应答
	}

	
	n--;
	while(n--) //循环了n-1
	{
		*RData++ = IIC_ReadByte();//读取8bit数据  用外部传入容器接住
		IIC_WriteAck(0);//给从机发送有应答
	}
	
	//读取最后一个字节
	*RData++ = IIC_ReadByte();//读取8bit数据  用外部传入容器接住
	IIC_WriteAck(1);//给从机发送无应答
	
	IIC_Stop();
	return 0;//成功
}

