#include "at24c02.h"

#define AT24C02_Addr  0x50 //0101 0000  <<1 1010 0000  0xA0  0xA1
 

//ʹ��IIC��һ���ֽ�д��24C02��ĳ��λ�ô��
int AT24C02_WriteByte(unsigned char Addr,unsigned char WData)
{
	IIC_Start();
	
	IIC_WriteByte(AT24C02_Addr<<1);//���ȷ����豸��ַ
	if(1 == IIC_ReadAck())
	{
		return -1;//������ǰ������ζ�Ŵӻ���Ӧ��
	}
	
	
	
	IIC_WriteByte(Addr);//���ٷ���Ҫд������оƬ�ĸ�λ��
	if(1 == IIC_ReadAck())
	{
		return -2;//������ǰ������ζ�Ŵӻ���Ӧ��
	}
	
	
	IIC_WriteByte(WData);//�۽�һ����ָ��8bit���ݴ�������ָ��Addr��ַ��
	if(1 == IIC_ReadAck())
	{
		return -3;//������ǰ������ζ�Ŵӻ���Ӧ��
	}
	
	
	IIC_Stop();
	return 0;//����ɹ�
}



int AT24C02_ReadByte(unsigned char Addr,unsigned char *RData)
{
	IIC_Start();
	IIC_WriteByte(AT24C02_Addr<<1);//����дģʽ�豸��ַ
	if(1 == IIC_ReadAck())
	{
		return -1;//������ǰ������ζ�Ŵӻ���Ӧ��
	}
	
	IIC_WriteByte(Addr);//����дģʽ�豸��ַ
	if(1 == IIC_ReadAck())
	{
		return -2;//������ǰ������ζ�Ŵӻ���Ӧ��
	}
	
	IIC_Start();//��������
	
	IIC_WriteByte((AT24C02_Addr<<1) | 1);//���Ͷ�ģʽ�豸��ַ 0xA1
	if(1 == IIC_ReadAck())
	{
		return -3;//������ǰ������ζ�Ŵӻ���Ӧ��
	}
	
	*RData = IIC_ReadByte();//��ȡ8bit����  ���ⲿ����������ס
	IIC_WriteAck(1);//���ӻ�������Ӧ��
	
	IIC_Stop();
	
	return 0;
}


//AT24C02ҳд����
//Addr:�����ҳд����ʼ��ַ 0~255 0x00~0xFF
//WData:Ҫд��Addr���������ֽڵ���ʼ��ַ
//n:ȷ��Ҫ��WData��ʼд�뼸���ֽڵ�оƬ
//write(fd,"hello",3)
//  16~23 Addr=18  n<=6
int AT24C02_PageWrite(unsigned char Addr,unsigned char *WData,int n)
{
	IIC_Start();
	
	IIC_WriteByte(AT24C02_Addr<<1);//���ȷ����豸��ַ
	if(1 == IIC_ReadAck())
	{
		return -1;//������ǰ������ζ�Ŵӻ���Ӧ��
	}
	
	
	
	IIC_WriteByte(Addr);//���ٷ���Ҫд������оƬ�ĸ�λ��
	if(1 == IIC_ReadAck())
	{
		return -2;//������ǰ������ζ�Ŵӻ���Ӧ��
	}
	
	
	//ѭ����WData��ַ��ʼ����n���ֽڵ�оƬ
	while(n--)
	{
		IIC_WriteByte(*WData++);//���ٷ���Ҫд������оƬ�ĸ�λ��
		if(1 == IIC_ReadAck())
		{
			return -3;//������ǰ������ζ�Ŵӻ���Ӧ��
		}
	}
	IIC_Stop();
	return 0;
}



int AT24C02_SeriesRead(unsigned char Addr,unsigned char *RData,int n)
{
	IIC_Start();
	IIC_WriteByte(AT24C02_Addr<<1);//����дģʽ�豸��ַ
	if(1 == IIC_ReadAck())
	{
		return -1;//������ǰ������ζ�Ŵӻ���Ӧ��
	}
	
	IIC_WriteByte(Addr);//����дģʽ�豸��ַ
	if(1 == IIC_ReadAck())
	{
		return -2;//������ǰ������ζ�Ŵӻ���Ӧ��
	}
	
	IIC_Start();//��������
	
	IIC_WriteByte((AT24C02_Addr<<1) | 1);//���Ͷ�ģʽ�豸��ַ 0xA1
	if(1 == IIC_ReadAck())
	{
		return -3;//������ǰ������ζ�Ŵӻ���Ӧ��
	}

	
	n--;
	while(n--) //ѭ����n-1
	{
		*RData++ = IIC_ReadByte();//��ȡ8bit����  ���ⲿ����������ס
		IIC_WriteAck(0);//���ӻ�������Ӧ��
	}
	
	//��ȡ���һ���ֽ�
	*RData++ = IIC_ReadByte();//��ȡ8bit����  ���ⲿ����������ס
	IIC_WriteAck(1);//���ӻ�������Ӧ��
	
	IIC_Stop();
	return 0;//�ɹ�
}

