#include "w25q128.h"

void W25Q128_WriteEnable(void)
{
	SPI_CS(0);
	
	SPI0_ReadWriteByte(0x06);//дʹ��ָ��
	
	SPI_CS(1);
}


void W25Q128_WriteDisable(void)
{
	SPI_CS(0);
	
	SPI0_ReadWriteByte(0x04);//д�ر�ָ��
	
	SPI_CS(1);
}


//��״̬�Ĵ���1����
unsigned char W25Q128_ReadSR1(void)
{
	unsigned char SR1Data=0;
	SPI_CS(0);
	
	SPI0_ReadWriteByte(0x05);//дSR1ָ��
	
	SR1Data = SPI0_ReadWriteByte(0xFF);
	
	SPI_CS(1);	
	
	return SR1Data;//���ض�����8bit�Ĵ������
}


//Addr����Ҫ�����ĵ�ַ 24bit
void W25Q128_SectorErase(unsigned int Addr)
{
	W25Q128_WriteEnable();//��дʹ��
	
	SPI_CS(0);
	
	SPI0_ReadWriteByte(0x20);//����8bitָ��
	
	SPI0_ReadWriteByte((Addr>>16) & 0xFF);//23~16
	SPI0_ReadWriteByte((Addr>>8) & 0xFF);//15~8
	SPI0_ReadWriteByte((Addr) & 0xFF);//7~0
	
	SPI_CS(1);
	
	
	
	//��ȡ״̬�Ĵ���1 ���ȴ��������
	//while(�������);
	while(W25Q128_ReadSR1() & 0x01);
	
	W25Q128_WriteDisable();//д�ر�
}




//����д����
//����1��Addr ָ�������￪ʼ�� ��ַ��Χ��0x000000~0xFFFFFF
//����2��WData �û��Լ�����Ĵ�������ݵ��׵�ַ
//����3��n ָ��Ҫ��WData��ַ��ʼ��ȡn���ֽ����ݴ�ŵ�Addr��ַ��
void W25Q128_PageWrite(unsigned int Addr,char *WData,int n)
{
	W25Q128_WriteEnable();//��дʹ��
	
	SPI_CS(0);
	
	SPI0_ReadWriteByte(0x02);//����8bitָ��
	
	SPI0_ReadWriteByte((Addr>>16) & 0xFF);//23~16
	SPI0_ReadWriteByte((Addr>>8) & 0xFF);//15~8
	SPI0_ReadWriteByte((Addr) & 0xFF);//7~0
	
	while(n--)
	{
		SPI0_ReadWriteByte(*WData++);
	}
	
	SPI_CS(1);
	
	
	//��ȡ״̬�Ĵ���1 ���ȴ��������
	//while(д�����);
	while(W25Q128_ReadSR1() & 0x01); //Ҫ��д�ر�ǰ��Flashд��
	
	W25Q128_WriteDisable();//д�ر�
}



//����������
//����1��Addr ָ�������￪ʼ�� ��ַ��Χ��0x000000~0xFFFFFF
//����2��RData �û��Լ�����Ĵ�Ŷ�ȡ�����ݵ��������׵�ַ
//����3��n ָ��Ҫ��Addr��ַ��ʼ��ȡn���ֽ����ݵ�RData�д��
void W25Q128_SerialRead(unsigned int Addr,char *RData,int n)
{
	SPI_CS(0);
	
	SPI0_ReadWriteByte(0x03);//����8bitָ��
	
	SPI0_ReadWriteByte((Addr>>16) & 0xFF);//23~16
	SPI0_ReadWriteByte((Addr>>8) & 0xFF);//15~8
	SPI0_ReadWriteByte((Addr) & 0xFF);//7~0
	
	while(n--)
	{
		*RData++ = SPI0_ReadWriteByte(0xFF);
	}
	
	SPI_CS(1);
		
}


//��ȡ�����̺��豸ID
unsigned short W25Q128_DevoceIDRead(void)
{
	unsigned short Data=0;
	
	SPI_CS(0);
	
	SPI0_ReadWriteByte(0x90);//����8bitָ��
	
	SPI0_ReadWriteByte((0x00));//23~16
	SPI0_ReadWriteByte(0x00);//15~8
	SPI0_ReadWriteByte(0x00);//7~0
	
	
	Data	 = SPI0_ReadWriteByte(0xFF)<<8;//������ID 0xEF
	Data    |= SPI0_ReadWriteByte(0xFF);//�豸ID
	
	SPI_CS(1);
	
	return Data;
}


