/**********************************
���ߣ���Ƭ�����ֲ�
��վ��https://www.mcuclub.cn/
**********************************/


#ifndef _MAIN_H_
#define _MAIN_H_


/**********************************
����ͷ�ļ�
**********************************/
#include<reg52.h>
#include<intrins.h>
#include<string.h>
#include<stdio.h>


/**********************************
PIN�ڶ���
**********************************/
sbit LED=P2^0;													//LED�ƴ�������
sbit BEEP=P2^1;													//��������������
sbit RELAY_JR=P1^1;											//���ȼ̵�����������P1.1
sbit RELAY_ZL=P1^2;											//����̵�����������P1.2
sbit RELAY_JS=P1^3;						 					//��ˮ�̵�������P1.3
sbit RELAY_CS=P1^4;						 					//��ˮ�̵�������P1.4


/**********************************
�ض���ؼ���
**********************************/
#define uchar unsigned char 
#define uint unsigned int
#define ulong unsigned long


/**********************************
��������
**********************************/
void Delay_function(uint x);						//��ʱx ms����


#endif

