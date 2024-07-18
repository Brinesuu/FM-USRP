/**********************************
作者：单片机俱乐部
网站：https://www.mcuclub.cn/
**********************************/


#ifndef _LCD1602_H_
#define _LCD1602_H_


/**********************************
包含头文件
**********************************/
#include "main.h"


/**********************************
PIN口定义
**********************************/
#define LCD1602_PORT P0      		 //数据总线引脚
sbit LCD1602_RS=P2^5;						 //数据/命令选择引脚
sbit LCD1602_RW=P2^6;						 //读/写选择引脚
sbit LCD1602_E=P2^7;						 //使能信号引脚
sbit LCD1602_BUSY=P0^7;					 //检测状态引脚


/**********************************
函数声明
**********************************/
void lcd1602_display_str(uchar hang, uchar add, uchar *dat);					//LCD1602显示字符串函数
void lcd1602_display_num(uchar hang, uchar add,uint num);							//LCD1602显示数字函数(最大4位)
void lcd1602_display_temp(uchar hang, uchar add,int temp);						//LCD1602显示温度函数(显示格式：**.*℃)
void lcd1602_display_distance(uchar hang, uchar add,uint distance);		//LCD1602显示距离函数(显示格式：***cm)
void lcd1602_display_countdown(uchar hang, uchar add,uint hour,uint minute,uint second);	//LCD1602显示倒计时函数(显示格式：**:**:**)
void lcd1602_clean();																									//LCD1602清屏函数
void Lcd1602_Init();																									//LCD1602初始化函数


#endif

