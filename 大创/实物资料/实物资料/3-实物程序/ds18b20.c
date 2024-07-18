/**********************************
作者：单片机俱乐部
网站：https://www.mcuclub.cn/
**********************************/


/**********************************
包含头文件
**********************************/
#include "ds18b20.h"


/****
*******延时函数
*****/
void ds18b20_delay_uint(uint z)
{
 while(z--);
}

/****
*******DS18B20初始化函数
*****/
void Ds18b20_Init(void)
{
	bit q;
	DS18B20_DQ=1;											//把总线拿高
	ds18b20_delay_uint(1);	    			//15us
	DS18B20_DQ=0;											//给复位脉冲
	ds18b20_delay_uint(80);						//750us
	DS18B20_DQ=1;											//把总线拿高 等待
	ds18b20_delay_uint(10);						//110us
	q=DS18B20_DQ;											//读取18b20初始化信号
	ds18b20_delay_uint(20);						//200us
	DS18B20_DQ=1;											//把总线拿高 释放总线
}

/****
*******写ds18b20内的数据函数
*****/
void write_18b20(uchar dat)
{
	uchar i;
	for(i=0;i<8;i++)
	{												 					//写数据是低位开始
		DS18B20_DQ=0;										//把总线拉低写时间隙开始 
		DS18B20_DQ=dat&0x01;						//向18b20总线写数据了
		ds18b20_delay_uint(5);	 				// 60us
		DS18B20_DQ=1;										//释放总线
		dat>>=1;
	}	
}

/****
*******读取ds18b20内的数据函数
*****/
uchar read_18b20(void)
{
	uchar i,value;
	for(i=0;i<8;i++)
	{
		DS18B20_DQ=0;										//把总线拿低读时间隙开始 
		value>>= 1;							 				//读数据是低位开始
		DS18B20_DQ=1;										//释放总线
		if(DS18B20_DQ==1)								//开始读写数据 
			value|=0x80;
		ds18b20_delay_uint(5);	 				//60us	读一个时间隙最少要保持60us的时间
	}
	return value;						 					//返回数据
}

/****
*******读取温度值函数
*******返回值：温度值(扩大10倍)
*****/
int Ds18b20_Read_Temp(void)
{
	int temp = 0;
	uchar tmh, tml;

	EA = 0;
	Ds18b20_Init();		  							//初始化18b20
	write_18b20(0xcc);	  						//跳过64位ROM
	write_18b20(0x44);	  						//启动一次温度转换命令
	ds18b20_delay_uint(50);						//500us

	Ds18b20_Init();		 			  				//初始化18b20	
	write_18b20(0xcc);	  						//跳过64位ROM
	write_18b20(0xbe);	 			  			//发出读取暂存器命令

	
	tml=read_18b20();				  				//读温度低字节
	tmh=read_18b20();			  					//读温度高字节
	
	TR0=1;														//打开定时器0中断
	TR1=1;														//打开定时器1中断

	temp = tmh;												//整合数值
	temp <<= 8;
	temp |= tml;

	temp=temp*0.625;									//换算成温度
	EA = 1;
	return temp;						  				//返回读出的温度
}

