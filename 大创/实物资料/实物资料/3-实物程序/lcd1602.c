/**********************************
作者：单片机俱乐部
网站：https://www.mcuclub.cn/
**********************************/


/**********************************
包含头文件
**********************************/
#include "lcd1602.h"


/**********************************
函数定义
**********************************/
/****
*******1602检测忙函数 
*****/
void lcd1602_check_busy()
{  
	P0=0xff;
	do
	{
		LCD1602_RS=0;
		LCD1602_RW=1;
		LCD1602_E=0;
		LCD1602_E=1;
	} while(LCD1602_BUSY==1);
	LCD1602_E=0;
}

/****
*******1602写数据忙函数 
*****/
void lcd1602_write_date(uchar date)
{
	lcd1602_check_busy();
	LCD1602_E=0;
	LCD1602_PORT=date;
	LCD1602_RS=1;
	LCD1602_RW=0;
	LCD1602_E=1;
	LCD1602_E=0;
}

/****
*******1602写命令函数 
*****/
void lcd1602_write_com(uchar com)
{
	lcd1602_check_busy();
	LCD1602_E=0;
	LCD1602_PORT=com;
	LCD1602_RS=0;
	LCD1602_RW=0;
	LCD1602_E=1;
	LCD1602_E=0;
}

/****
*******LCD1602显示字符串函数 
*******参量定义：	hang：输入显示行数  1 第一行  2 第二行
*******           add：偏移量，0-15
*******           dat：所要显示的字符串，输入格式 " "
*****/
void lcd1602_display_str(uchar hang, uchar add, uchar *dat)
{
	if(hang==1)   
		lcd1602_write_com(0x80+add);			//第一行
	else
		lcd1602_write_com(0x80+0x40+add);	//第二行

	while(*dat != '\0')
	{
		lcd1602_write_date(*dat++);
	
	}
}

/****
*******LCD1602清屏函数 
*****/
void lcd1602_clean()
{
	lcd1602_display_str(1,0,"                        ");
	lcd1602_display_str(2,0,"                        ");
}

/****
*******LCD1602初始化函数 
*****/
void Lcd1602_Init()
{
	lcd1602_write_com(0x38);
	lcd1602_write_com(0x0c);
	lcd1602_write_com(0x06);
	lcd1602_write_com(0x01);
}

/****
*******LCD1602显示数字函数(最大4位)
*******参量定义：	hang：输入显示行数  1 第一行  2 第二行
*******           add：偏移量，0-15
*******           num：所要显示的数据，最大4位
*****/
void lcd1602_display_num(uchar hang, uchar add, uint num)
{
	if(hang==1)   
		lcd1602_write_com(0x80+add);				//第一行
	else
		lcd1602_write_com(0x80+0x40+add);		//第二行

	
	if(num >= 1000)
	{
		lcd1602_write_date(num/1000%10+48);	//千
		lcd1602_write_date(num/100%10+48);	//百
		lcd1602_write_date(num/10%10+48);		//十
		lcd1602_write_date(num/1%10+48);		//个
	}
	else if(num >= 100)
	{
		lcd1602_write_date(num/100%10+48);	//百
		lcd1602_write_date(num/10%10+48);		//十
		lcd1602_write_date(num/1%10+48);		//个
		lcd1602_write_date(' ');						//补空格
	}
	else if(num >= 10)
	{
		lcd1602_write_date(num/10+48);			//十
		lcd1602_write_date(num%10+48);			//个
		lcd1602_write_date(' ');						//补空格
		lcd1602_write_date(' ');						//补空格
	}
	else
	{
		lcd1602_write_date(num+48);					//个
		lcd1602_write_date(' ');						//补空格
		lcd1602_write_date(' ');						//补空格
		lcd1602_write_date(' ');						//补空格
	}
}

/****
*******LCD1602显示温度函数(显示格式：**.*℃)
*******参量定义：	hang：输入显示行数  1 第一行  2 第二行
*******           add：偏移量，0-15
*******           temp：所要显示的温度
*****/
void lcd1602_display_temp(uchar hang, uchar add,int temp)
{
	uchar display_temp_buf[8];

	if(hang==1)   
		lcd1602_write_com(0x80+add);			//第一行
	else
		lcd1602_write_com(0x80+0x40+add);	//第二行

	if(temp >= 0)
		sprintf(display_temp_buf,"%d.%d",temp/10,temp%10);
	else
		sprintf(display_temp_buf,"-%d.%d",temp*(-1)/10,temp*(-1)%10);
	lcd1602_display_str(hang,add,display_temp_buf);
	lcd1602_write_date(0xdf);				//显示温度的小圆圈符号，0xdf是液晶屏字符库的该符号地址码
  lcd1602_write_date(0x43);				//显示"C"符号，0x43是液晶屏字符库里大写C的地址码
	lcd1602_write_date(' ');
	lcd1602_write_date(' ');
}

/****
*******LCD1602显示距离函数(显示格式：***cm)
*******参量定义：	hang：输入显示行数  1 第一行  2 第二行
*******           add：偏移量，0-15
*******           distance：所要显示的距离
*****/
void lcd1602_display_distance(uchar hang, uchar add,uint distance)
{
	if(hang==1)   
		lcd1602_write_com(0x80+add);			//第一行
	else
		lcd1602_write_com(0x80+0x40+add);	//第二行

	if(distance > 99)
		lcd1602_write_date(distance/100%10+48);
	if(distance > 9)
		lcd1602_write_date(distance/10%10+48);
	lcd1602_write_date(distance/1%10+48);
	lcd1602_write_date('c');
	lcd1602_write_date('m');
	lcd1602_write_date(' ');
	lcd1602_write_date(' ');
}

/****
*******LCD1602显示倒计时函数(显示格式：**:**:**)
*******参量定义：	hang：输入显示行数  1 第一行  2 第二行
*******           add：偏移量，0-15
*******           hour：所要显示的倒计时时	minute：所要显示的倒计时分	second：所要显示的倒计时秒
*****/
void lcd1602_display_countdown(uchar hang, uchar add,uint hour,uint minute,uint second)
{
	if(hang==1)   
		lcd1602_write_com(0x80+add);			//第一行
	else
		lcd1602_write_com(0x80+0x40+add);	//第二行

	lcd1602_write_date(hour/10%10+48);
	lcd1602_write_date(hour/1%10+48);
	lcd1602_write_date(':');
	lcd1602_write_date(minute/10%10+48);
	lcd1602_write_date(minute/1%10+48);
	lcd1602_write_date(':');
	lcd1602_write_date(second/10%10+48);
	lcd1602_write_date(second/1%10+48);
}

