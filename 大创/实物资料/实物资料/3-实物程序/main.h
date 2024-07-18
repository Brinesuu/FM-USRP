/**********************************
作者：单片机俱乐部
网站：https://www.mcuclub.cn/
**********************************/


#ifndef _MAIN_H_
#define _MAIN_H_


/**********************************
包含头文件
**********************************/
#include<reg52.h>
#include<intrins.h>
#include<string.h>
#include<stdio.h>


/**********************************
PIN口定义
**********************************/
sbit LED=P2^0;													//LED灯触发引脚
sbit BEEP=P2^1;													//蜂鸣器触发引脚
sbit RELAY_JR=P1^1;											//加热继电器触发引脚P1.1
sbit RELAY_ZL=P1^2;											//制冷继电器触发引脚P1.2
sbit RELAY_JS=P1^3;						 					//加水继电器引脚P1.3
sbit RELAY_CS=P1^4;						 					//抽水继电器引脚P1.4


/**********************************
重定义关键词
**********************************/
#define uchar unsigned char 
#define uint unsigned int
#define ulong unsigned long


/**********************************
函数声明
**********************************/
void Delay_function(uint x);						//延时x ms函数


#endif

