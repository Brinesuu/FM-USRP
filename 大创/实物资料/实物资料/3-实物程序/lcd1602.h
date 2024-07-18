/**********************************
���ߣ���Ƭ�����ֲ�
��վ��https://www.mcuclub.cn/
**********************************/


#ifndef _LCD1602_H_
#define _LCD1602_H_


/**********************************
����ͷ�ļ�
**********************************/
#include "main.h"


/**********************************
PIN�ڶ���
**********************************/
#define LCD1602_PORT P0      		 //������������
sbit LCD1602_RS=P2^5;						 //����/����ѡ������
sbit LCD1602_RW=P2^6;						 //��/дѡ������
sbit LCD1602_E=P2^7;						 //ʹ���ź�����
sbit LCD1602_BUSY=P0^7;					 //���״̬����


/**********************************
��������
**********************************/
void lcd1602_display_str(uchar hang, uchar add, uchar *dat);					//LCD1602��ʾ�ַ�������
void lcd1602_display_num(uchar hang, uchar add,uint num);							//LCD1602��ʾ���ֺ���(���4λ)
void lcd1602_display_temp(uchar hang, uchar add,int temp);						//LCD1602��ʾ�¶Ⱥ���(��ʾ��ʽ��**.*��)
void lcd1602_display_distance(uchar hang, uchar add,uint distance);		//LCD1602��ʾ���뺯��(��ʾ��ʽ��***cm)
void lcd1602_display_countdown(uchar hang, uchar add,uint hour,uint minute,uint second);	//LCD1602��ʾ����ʱ����(��ʾ��ʽ��**:**:**)
void lcd1602_clean();																									//LCD1602��������
void Lcd1602_Init();																									//LCD1602��ʼ������


#endif

