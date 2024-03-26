#include "w25q128.h"

void W25Q128_WriteEnable(void)
{
	SPI_CS(0);
	
	SPI0_ReadWriteByte(0x06);//写使能指令
	
	SPI_CS(1);
}


void W25Q128_WriteDisable(void)
{
	SPI_CS(0);
	
	SPI0_ReadWriteByte(0x04);//写关闭指令
	
	SPI_CS(1);
}


//读状态寄存器1函数
unsigned char W25Q128_ReadSR1(void)
{
	unsigned char SR1Data=0;
	SPI_CS(0);
	
	SPI0_ReadWriteByte(0x05);//写SR1指令
	
	SR1Data = SPI0_ReadWriteByte(0xFF);
	
	SPI_CS(1);	
	
	return SR1Data;//返回读到的8bit寄存器结果
}


//Addr是你要擦除的地址 24bit
void W25Q128_SectorErase(unsigned int Addr)
{
	W25Q128_WriteEnable();//先写使能
	
	SPI_CS(0);
	
	SPI0_ReadWriteByte(0x20);//发送8bit指令
	
	SPI0_ReadWriteByte((Addr>>16) & 0xFF);//23~16
	SPI0_ReadWriteByte((Addr>>8) & 0xFF);//15~8
	SPI0_ReadWriteByte((Addr) & 0xFF);//7~0
	
	SPI_CS(1);
	
	
	
	//读取状态寄存器1 来等待擦除完成
	//while(擦除完成);
	while(W25Q128_ReadSR1() & 0x01);
	
	W25Q128_WriteDisable();//写关闭
}




//连续写函数
//参数1：Addr 指定从哪里开始存 地址范围：0x000000~0xFFFFFF
//参数2：WData 用户自己传入的待存放数据的首地址
//参数3：n 指定要从WData地址开始读取n个字节数据存放到Addr地址中
void W25Q128_PageWrite(unsigned int Addr,char *WData,int n)
{
	W25Q128_WriteEnable();//先写使能
	
	SPI_CS(0);
	
	SPI0_ReadWriteByte(0x02);//发送8bit指令
	
	SPI0_ReadWriteByte((Addr>>16) & 0xFF);//23~16
	SPI0_ReadWriteByte((Addr>>8) & 0xFF);//15~8
	SPI0_ReadWriteByte((Addr) & 0xFF);//7~0
	
	while(n--)
	{
		SPI0_ReadWriteByte(*WData++);
	}
	
	SPI_CS(1);
	
	
	//读取状态寄存器1 来等待擦除完成
	//while(写入完成);
	while(W25Q128_ReadSR1() & 0x01); //要在写关闭前等Flash写完
	
	W25Q128_WriteDisable();//写关闭
}



//连续读函数
//参数1：Addr 指定从哪里开始读 地址范围：0x000000~0xFFFFFF
//参数2：RData 用户自己传入的存放读取出数据的容器的首地址
//参数3：n 指定要从Addr地址开始读取n个字节数据到RData中存放
void W25Q128_SerialRead(unsigned int Addr,char *RData,int n)
{
	SPI_CS(0);
	
	SPI0_ReadWriteByte(0x03);//发送8bit指令
	
	SPI0_ReadWriteByte((Addr>>16) & 0xFF);//23~16
	SPI0_ReadWriteByte((Addr>>8) & 0xFF);//15~8
	SPI0_ReadWriteByte((Addr) & 0xFF);//7~0
	
	while(n--)
	{
		*RData++ = SPI0_ReadWriteByte(0xFF);
	}
	
	SPI_CS(1);
		
}


//读取制造商和设备ID
unsigned short W25Q128_DevoceIDRead(void)
{
	unsigned short Data=0;
	
	SPI_CS(0);
	
	SPI0_ReadWriteByte(0x90);//发送8bit指令
	
	SPI0_ReadWriteByte((0x00));//23~16
	SPI0_ReadWriteByte(0x00);//15~8
	SPI0_ReadWriteByte(0x00);//7~0
	
	
	Data	 = SPI0_ReadWriteByte(0xFF)<<8;//制造商ID 0xEF
	Data    |= SPI0_ReadWriteByte(0xFF);//设备ID
	
	SPI_CS(1);
	
	return Data;
}


