/**********************************
作者：单片机俱乐部
网站：https://www.mcuclub.cn/
**********************************/


/**********************************
包含头文件
**********************************/
#include "main.h"
#include "lcd1602.h"
#include "key.h"
#include "ds18b20.h"
#include "hc-sr04.h"
#include "timer.h"


/**********************************
变量定义
**********************************/
uchar key_num = 0;											//按键扫描标志位			
uchar flag_display = 0;									//显示界面标志位
uint time_num = 0;											//10ms计时变量
int temp_value = 0;											//温度值
int temp_max = 30;											//温度最大值
int temp_min = 10;											//温度最小值
uchar temp_buf[8];											//温度保存数值
uint distance_value = 0;								//距离值
uint distance_max = 12;									//水位最大值
uint distance_min = 5;									//水位最小值
uint distance_all = 15;									//总高度
uchar countdown_hour_old = 0;						//倒计时存储时
char countdown_minute_old = 0;					//倒计时存储分
char countdown_second_old = 0;					//倒计时存储秒
uchar countdown_hour = 0;								//倒计时时
char countdown_minute = 0;							//倒计时分
char countdown_second = 0;							//倒计时秒
bit flag_countdown_end = 0;							//倒计时结束标志位
extern bit flag_timer_begin;						//计时开始标志位
extern bit flag_1s;											//1秒到达标志位
uint time_count = 0;										//计时变量


/**********************************
函数声明
**********************************/
void Delay_function(uint x);						//延时函数(ms)
void Key_function(void);								//按键函数
void Monitor_function(void);						//监测函数
void Display_function(void);						//显示函数
void Manage_function(void);							//处理函数


/****
*******	主函数 
*****/
void main()
{
	Lcd1602_Init();		 										//LCD1602初始化
	Delay_function(50);										//延时50ms
	lcd1602_clean();											//清屏
	Delay_function(50);										//延时50ms
	Ds18b20_Init();												//DS18B20初始化
	Delay_function(50);										//延时50ms
	Timer0_Init();												//定时器0初使化
	Delay_function(50);										//延时50ms
	
	while(1)
	{
		Key_function();											//按键函数
		Monitor_function();									//监测函数
		Display_function();									//显示函数
		Manage_function();									//处理函数

		Delay_function(10);									//延时10ms
		time_num++;													//计时变量+1
		if(time_num == 5000)
		{
			time_num = 0;
		}
	}
}

/****
*******	延时 x ms函数
*****/
void Delay_function(uint x)
{
	uint m,n;
	for(m=x;m>0;m--)
	for(n=110;n>0;n--);
}

/****
*******按键函数
*****/
void Key_function(void)
{
	key_num = Chiclet_Keyboard_Scan(0);					//按键扫描
	if(key_num != 0)														//有按键按下
	{
		switch(key_num)
		{
			case 1:																	//按键1，切换设置界面
				flag_display++;
				if(flag_display >= 9)
					flag_display = 0;
				
				lcd1602_clean();
			break;

			case 2:																	//按键2，加键
				switch(flag_display)
				{
					case 1:															//界面1：温度最大值+1
						if(temp_max < 99)
							temp_max++;
					break;

					case 2:															//界面2：温度最小值+1
						if(temp_min < temp_max-1)
							temp_min++;
					break;
						
					case 3:															//界面3：总高度值+1
						if(distance_all < 500)
							distance_all++;
					break;	

					case 4:															//界面4：水位最大值+1
						if(distance_max < (distance_all-1))
							distance_max++;
					break;
						
					case 5:															//界面5：水位最小值+1
						if(distance_min < distance_max-1)
							distance_min++;
					break;
						
					case 6:															//界面6：倒计时时+1
						if(countdown_hour_old < 100)
							countdown_hour_old++;
						countdown_hour = countdown_hour_old;
					break;
						
					case 7:															//界面7：倒计时分+1
						countdown_minute_old++;
						if(countdown_minute_old >= 60)
							countdown_minute_old = 0;
						countdown_minute = countdown_minute_old;
					break;
						
					case 8:															//界面8：倒计时秒+1
						countdown_second_old++;
						if(countdown_second_old >= 60)
							countdown_second_old = 0;
						countdown_second = countdown_second_old;
					break;
					
					default:
					break;
				}
			break;

			case 3:																	//按键3，减键
				switch(flag_display)
				{
					case 1:															//界面1：温度最大值-1
						if(temp_max > temp_min+1)
							temp_max--;
					break;
					
					case 2:															//界面2：温度最小值-1
						//if(temp_min > 0)
							temp_min--;
					break;
					
					case 3:															//界面3：总高度值-1
						if(distance_all > (distance_max+1))
							distance_all--;
					break;
					
					case 4:															//界面4：水位最大值-1
						if(distance_max > distance_min+1)
							distance_max--;
					break;
						
					case 5:															//界面5：水位最小值-1
						if(distance_min > 0)
							distance_min--;
					break;
						
					case 6:															//界面6：倒计时时-1
						if(countdown_hour_old > 0)
							countdown_hour_old--;
						countdown_hour = countdown_hour_old;
					break;
						
					case 7:															//界面7：倒计时分-1
						countdown_minute_old--;
						if(countdown_minute_old < 0)
							countdown_minute_old = 59;
						countdown_minute = countdown_minute_old;
					break;
						
					case 8:															//界面8：倒计时秒-1
						countdown_second_old--;
						if(countdown_second_old < 0)
							countdown_second_old = 59;
						countdown_second = countdown_second_old;
					break;
					
					default:
					break;
				}
			break;

			default:
			break;
		}
	}
}

/****
*******监测函数
*****/
void Monitor_function(void)
{
	if(time_num % 10 == 0)																			//100ms检测一次
	{
		temp_value = Ds18b20_Read_Temp();													//获取温度值
		distance_value = Hcsr04_GetDistance(temp_value/10);				//获取距离值
		if(distance_value > distance_all)													//计算剩余水位
			distance_value = 0;
		else
			distance_value = distance_all - distance_value;
	}
}

/****
*******显示函数
*****/
void Display_function(void)
{
	switch(flag_display)																				//根据不同的显示模式标志位，显示不同的界面
	{
		case 0:																										//界面0
			lcd1602_display_str(1,0,"Temp:");												//显示温度测量值
			lcd1602_display_temp(1,5,temp_value);

			lcd1602_display_str(2,0,"D:");													//显示剩余水位
			lcd1602_display_distance(2,2,distance_value);
								
			lcd1602_display_countdown(2,8,countdown_hour,countdown_minute,countdown_second);		//显示倒计时时间
		break;

		case 1:																										//界面1，显示设置最大温度
			lcd1602_display_str(1,2,"Set Temp Max");
			if(time_num % 20 == 0)
			{
				sprintf(temp_buf,"%d",temp_max);
				lcd1602_display_str(2,7,temp_buf);
			}
			if(time_num % 40 == 0)
			{
				lcd1602_display_str(2,7,"    ");
			}
		break;
			
		case 2:																										//界面2，显示设置最小温度
			lcd1602_display_str(1,2,"Set Temp Min");
			if(time_num % 20 == 0)
			{
				sprintf(temp_buf,"%d",temp_min);
				lcd1602_display_str(2,7,temp_buf);
			}
			if(time_num % 40 == 0)
			{
				lcd1602_display_str(2,7,"    ");
			}
		break;
			
		case 3:																										//界面3：显示设置总高度
			lcd1602_display_str(1,2,"Set Dist All");
			if(time_num % 20 == 0)
			{
				lcd1602_display_num(2,7,distance_all);
			}
			if(time_num % 40 == 0)
			{
				lcd1602_display_str(2,7,"    ");
			}
		break;
		
		case 4:																										//界面4：显示设置水位最大值
			lcd1602_display_str(1,2,"Set Dist Max");
			if(time_num % 20 == 0)
			{
				lcd1602_display_num(2,7,distance_max);
			}
			if(time_num % 40 == 0)
			{
				lcd1602_display_str(2,7,"    ");
			}
		break;
			
		case 5:																										//界面5：显示设置水位最小值
			lcd1602_display_str(1,2,"Set Dist Min");
			if(time_num % 20 == 0)
			{
				lcd1602_display_num(2,7,distance_min);
			}
			if(time_num % 40 == 0)
			{
				lcd1602_display_str(2,7,"    ");
			}
		break;
			
		case 6:																										//界面6：显示设置倒计时时
			lcd1602_display_str(1,4,"Set Time");
			if(time_num % 20 == 0)
			{
				lcd1602_display_countdown(2,4,countdown_hour_old,countdown_minute_old,countdown_second_old);
			}
			if(time_num % 40 == 0)
			{
				lcd1602_display_str(2,4,"  ");
			}
		break;
			
		case 7:																										//界面7：显示设置倒计时分
			lcd1602_display_str(1,4,"Set Time");
			if(time_num % 20 == 0)
			{
				lcd1602_display_countdown(2,4,countdown_hour_old,countdown_minute_old,countdown_second_old);
			}
			if(time_num % 40 == 0)
			{
				lcd1602_display_str(2,7,"  ");
			}
		break;
			
		case 8:																										//界面8：显示设置倒计时秒
			lcd1602_display_str(1,4,"Set Time");
			if(time_num % 20 == 0)
			{
				lcd1602_display_countdown(2,4,countdown_hour_old,countdown_minute_old,countdown_second_old);
			}
			if(time_num % 40 == 0)
			{
				lcd1602_display_str(2,10,"  ");
			}
		break;

		default:
		break;
	}
}

/****
*******处理函数
*****/
void Manage_function(void)
{
	if(flag_display == 0)																		//测量界面
	{
		if(temp_value > temp_max*10)													//温度大于设置最大值，制冷继电器闭合
		{
			RELAY_ZL = 0;
			RELAY_JR = 1;
		}
		else if(temp_value < temp_min*10)											//温度小于设置最小值，加热继电器闭合
		{
			RELAY_ZL = 1;
			RELAY_JR = 0;
		}
		else																									//温度处于设置的上下限值之间，两个继电器断开
		{
			RELAY_ZL = 1;
			RELAY_JR = 1;
		}
		
		if(distance_value < distance_min)											//水位值小于设置最小值，加水继电器闭合，并声光报警
		{
			RELAY_JS = 0;
			RELAY_CS = 1;
			if(time_num % 20 == 0)
			{
				LED = ~LED;
				BEEP = ~BEEP;
			}
		}
		else if(distance_value > distance_max)								//水位值大于设置最大值，抽水继电器闭合，并声光报警
		{
			RELAY_JS = 1;
			RELAY_CS = 0;
			if(time_num % 20 == 0)
			{
				LED = ~LED;
				BEEP = ~BEEP;
			}
		}
		else																									//水位值处于上下限值之间，两个继电器断开，并取消声光报警
		{
			RELAY_JS = 1;
			RELAY_CS = 1;
			LED = 1;
			BEEP = 1;
		}
		
		if(countdown_hour != 0 || countdown_minute != 0 || countdown_second != 0)			//换水倒计时不为0
			flag_timer_begin = 1;																												//开始倒计时，进入定时器中断
		
		if(flag_1s == 1)																															//1s到达，倒计时-1s
		{
			flag_1s = 0;
			if(countdown_second > 0)																										//倒计时秒>0
				countdown_second--;																												//倒计时秒-1
			else																																				//倒计时秒=0
			{
				if(countdown_minute > 0)																									//如果倒计时分>0
				{
					countdown_minute--;																											//倒计时分-1
					countdown_second = 59;																									//倒计时秒=59
				}
				else																																			//如果倒计时分=0
				{
					if(countdown_hour > 0)																									//如果倒计时时>0
					{
						countdown_hour--;																											//倒计时时-1
						countdown_minute = 59;																								//倒计时分=59
						countdown_second = 59;																								//倒计时秒=59
					}
					else																																		//如果倒计时时=0，倒计时结束
					{
						flag_timer_begin = 0;
						flag_countdown_end = 1;
					}
				}
			}
		}
		
		if(flag_countdown_end == 1)																										//倒计时结束
		{
			LED = 1;
			BEEP = 1;

			do																																					//开始抽水，直到水位小于最小值
			{
				RELAY_JS = 1;
				RELAY_CS = 0;
				
				if(time_count % 10 == 0)
				{
					distance_value = Hcsr04_GetDistance(temp_value/10);
					if(distance_value > distance_all)
						distance_value = 0;
					else
						distance_value = distance_all - distance_value;
					
					lcd1602_display_str(2,0,"D:");
					lcd1602_display_distance(2,2,distance_value);
				}

				Delay_function(10);
				time_count++;
				if(time_count > 5000)
					time_count = 0;
				
			}while(distance_value >= distance_min);
			
			time_count = 0;
			
			do																																					//抽水完成后，开始加水，直到水位大于最大值
			{
				RELAY_JS = 0;
				RELAY_CS = 1;
				
				if(time_count % 10 == 0)
				{
					distance_value = Hcsr04_GetDistance(temp_value/10);
					if(distance_value > distance_all)
						distance_value = 0;
					else
						distance_value = distance_all - distance_value;
					
					lcd1602_display_str(2,0,"D:");
					lcd1602_display_distance(2,2,distance_value);
				}

				Delay_function(10);
				time_count++;
				if(time_count > 5000)
					time_count = 0;

			}while(distance_value < distance_max);
			
			time_count = 0;
			RELAY_JS = 1;
			RELAY_CS = 1;
			
			flag_countdown_end = 0;
			countdown_hour = countdown_hour_old;																					//重新赋值倒计时时间
			countdown_minute = countdown_minute_old;
			countdown_second = countdown_second_old;
		}
	}
	else																																							//设置界面，所有继电器断开，声光报警取消
	{
		RELAY_ZL = 1;
		RELAY_JR = 1;
		RELAY_JS = 1;
		RELAY_CS = 1;
		LED = 1;
		BEEP = 1;
	}
}

