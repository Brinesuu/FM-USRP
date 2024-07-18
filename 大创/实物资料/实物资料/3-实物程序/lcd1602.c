/**********************************
���ߣ���Ƭ�����ֲ�
��վ��https://www.mcuclub.cn/
**********************************/


/**********************************
����ͷ�ļ�
**********************************/
#include "lcd1602.h"


/**********************************
��������
**********************************/
/****
*******1602���æ���� 
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
*******1602д����æ���� 
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
*******1602д����� 
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
*******LCD1602��ʾ�ַ������� 
*******�������壺	hang��������ʾ����  1 ��һ��  2 �ڶ���
*******           add��ƫ������0-15
*******           dat����Ҫ��ʾ���ַ����������ʽ " "
*****/
void lcd1602_display_str(uchar hang, uchar add, uchar *dat)
{
	if(hang==1)   
		lcd1602_write_com(0x80+add);			//��һ��
	else
		lcd1602_write_com(0x80+0x40+add);	//�ڶ���

	while(*dat != '\0')
	{
		lcd1602_write_date(*dat++);
	
	}
}

/****
*******LCD1602�������� 
*****/
void lcd1602_clean()
{
	lcd1602_display_str(1,0,"                        ");
	lcd1602_display_str(2,0,"                        ");
}

/****
*******LCD1602��ʼ������ 
*****/
void Lcd1602_Init()
{
	lcd1602_write_com(0x38);
	lcd1602_write_com(0x0c);
	lcd1602_write_com(0x06);
	lcd1602_write_com(0x01);
}

/****
*******LCD1602��ʾ���ֺ���(���4λ)
*******�������壺	hang��������ʾ����  1 ��һ��  2 �ڶ���
*******           add��ƫ������0-15
*******           num����Ҫ��ʾ�����ݣ����4λ
*****/
void lcd1602_display_num(uchar hang, uchar add, uint num)
{
	if(hang==1)   
		lcd1602_write_com(0x80+add);				//��һ��
	else
		lcd1602_write_com(0x80+0x40+add);		//�ڶ���

	
	if(num >= 1000)
	{
		lcd1602_write_date(num/1000%10+48);	//ǧ
		lcd1602_write_date(num/100%10+48);	//��
		lcd1602_write_date(num/10%10+48);		//ʮ
		lcd1602_write_date(num/1%10+48);		//��
	}
	else if(num >= 100)
	{
		lcd1602_write_date(num/100%10+48);	//��
		lcd1602_write_date(num/10%10+48);		//ʮ
		lcd1602_write_date(num/1%10+48);		//��
		lcd1602_write_date(' ');						//���ո�
	}
	else if(num >= 10)
	{
		lcd1602_write_date(num/10+48);			//ʮ
		lcd1602_write_date(num%10+48);			//��
		lcd1602_write_date(' ');						//���ո�
		lcd1602_write_date(' ');						//���ո�
	}
	else
	{
		lcd1602_write_date(num+48);					//��
		lcd1602_write_date(' ');						//���ո�
		lcd1602_write_date(' ');						//���ո�
		lcd1602_write_date(' ');						//���ո�
	}
}

/****
*******LCD1602��ʾ�¶Ⱥ���(��ʾ��ʽ��**.*��)
*******�������壺	hang��������ʾ����  1 ��һ��  2 �ڶ���
*******           add��ƫ������0-15
*******           temp����Ҫ��ʾ���¶�
*****/
void lcd1602_display_temp(uchar hang, uchar add,int temp)
{
	uchar display_temp_buf[8];

	if(hang==1)   
		lcd1602_write_com(0x80+add);			//��һ��
	else
		lcd1602_write_com(0x80+0x40+add);	//�ڶ���

	if(temp >= 0)
		sprintf(display_temp_buf,"%d.%d",temp/10,temp%10);
	else
		sprintf(display_temp_buf,"-%d.%d",temp*(-1)/10,temp*(-1)%10);
	lcd1602_display_str(hang,add,display_temp_buf);
	lcd1602_write_date(0xdf);				//��ʾ�¶ȵ�СԲȦ���ţ�0xdf��Һ�����ַ���ĸ÷��ŵ�ַ��
  lcd1602_write_date(0x43);				//��ʾ"C"���ţ�0x43��Һ�����ַ������дC�ĵ�ַ��
	lcd1602_write_date(' ');
	lcd1602_write_date(' ');
}

/****
*******LCD1602��ʾ���뺯��(��ʾ��ʽ��***cm)
*******�������壺	hang��������ʾ����  1 ��һ��  2 �ڶ���
*******           add��ƫ������0-15
*******           distance����Ҫ��ʾ�ľ���
*****/
void lcd1602_display_distance(uchar hang, uchar add,uint distance)
{
	if(hang==1)   
		lcd1602_write_com(0x80+add);			//��һ��
	else
		lcd1602_write_com(0x80+0x40+add);	//�ڶ���

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
*******LCD1602��ʾ����ʱ����(��ʾ��ʽ��**:**:**)
*******�������壺	hang��������ʾ����  1 ��һ��  2 �ڶ���
*******           add��ƫ������0-15
*******           hour����Ҫ��ʾ�ĵ���ʱʱ	minute����Ҫ��ʾ�ĵ���ʱ��	second����Ҫ��ʾ�ĵ���ʱ��
*****/
void lcd1602_display_countdown(uchar hang, uchar add,uint hour,uint minute,uint second)
{
	if(hang==1)   
		lcd1602_write_com(0x80+add);			//��һ��
	else
		lcd1602_write_com(0x80+0x40+add);	//�ڶ���

	lcd1602_write_date(hour/10%10+48);
	lcd1602_write_date(hour/1%10+48);
	lcd1602_write_date(':');
	lcd1602_write_date(minute/10%10+48);
	lcd1602_write_date(minute/1%10+48);
	lcd1602_write_date(':');
	lcd1602_write_date(second/10%10+48);
	lcd1602_write_date(second/1%10+48);
}

