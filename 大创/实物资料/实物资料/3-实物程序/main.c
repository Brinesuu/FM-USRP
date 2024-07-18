/**********************************
���ߣ���Ƭ�����ֲ�
��վ��https://www.mcuclub.cn/
**********************************/


/**********************************
����ͷ�ļ�
**********************************/
#include "main.h"
#include "lcd1602.h"
#include "key.h"
#include "ds18b20.h"
#include "hc-sr04.h"
#include "timer.h"


/**********************************
��������
**********************************/
uchar key_num = 0;											//����ɨ���־λ			
uchar flag_display = 0;									//��ʾ�����־λ
uint time_num = 0;											//10ms��ʱ����
int temp_value = 0;											//�¶�ֵ
int temp_max = 30;											//�¶����ֵ
int temp_min = 10;											//�¶���Сֵ
uchar temp_buf[8];											//�¶ȱ�����ֵ
uint distance_value = 0;								//����ֵ
uint distance_max = 12;									//ˮλ���ֵ
uint distance_min = 5;									//ˮλ��Сֵ
uint distance_all = 15;									//�ܸ߶�
uchar countdown_hour_old = 0;						//����ʱ�洢ʱ
char countdown_minute_old = 0;					//����ʱ�洢��
char countdown_second_old = 0;					//����ʱ�洢��
uchar countdown_hour = 0;								//����ʱʱ
char countdown_minute = 0;							//����ʱ��
char countdown_second = 0;							//����ʱ��
bit flag_countdown_end = 0;							//����ʱ������־λ
extern bit flag_timer_begin;						//��ʱ��ʼ��־λ
extern bit flag_1s;											//1�뵽���־λ
uint time_count = 0;										//��ʱ����


/**********************************
��������
**********************************/
void Delay_function(uint x);						//��ʱ����(ms)
void Key_function(void);								//��������
void Monitor_function(void);						//��⺯��
void Display_function(void);						//��ʾ����
void Manage_function(void);							//������


/****
*******	������ 
*****/
void main()
{
	Lcd1602_Init();		 										//LCD1602��ʼ��
	Delay_function(50);										//��ʱ50ms
	lcd1602_clean();											//����
	Delay_function(50);										//��ʱ50ms
	Ds18b20_Init();												//DS18B20��ʼ��
	Delay_function(50);										//��ʱ50ms
	Timer0_Init();												//��ʱ��0��ʹ��
	Delay_function(50);										//��ʱ50ms
	
	while(1)
	{
		Key_function();											//��������
		Monitor_function();									//��⺯��
		Display_function();									//��ʾ����
		Manage_function();									//������

		Delay_function(10);									//��ʱ10ms
		time_num++;													//��ʱ����+1
		if(time_num == 5000)
		{
			time_num = 0;
		}
	}
}

/****
*******	��ʱ x ms����
*****/
void Delay_function(uint x)
{
	uint m,n;
	for(m=x;m>0;m--)
	for(n=110;n>0;n--);
}

/****
*******��������
*****/
void Key_function(void)
{
	key_num = Chiclet_Keyboard_Scan(0);					//����ɨ��
	if(key_num != 0)														//�а�������
	{
		switch(key_num)
		{
			case 1:																	//����1���л����ý���
				flag_display++;
				if(flag_display >= 9)
					flag_display = 0;
				
				lcd1602_clean();
			break;

			case 2:																	//����2���Ӽ�
				switch(flag_display)
				{
					case 1:															//����1���¶����ֵ+1
						if(temp_max < 99)
							temp_max++;
					break;

					case 2:															//����2���¶���Сֵ+1
						if(temp_min < temp_max-1)
							temp_min++;
					break;
						
					case 3:															//����3���ܸ߶�ֵ+1
						if(distance_all < 500)
							distance_all++;
					break;	

					case 4:															//����4��ˮλ���ֵ+1
						if(distance_max < (distance_all-1))
							distance_max++;
					break;
						
					case 5:															//����5��ˮλ��Сֵ+1
						if(distance_min < distance_max-1)
							distance_min++;
					break;
						
					case 6:															//����6������ʱʱ+1
						if(countdown_hour_old < 100)
							countdown_hour_old++;
						countdown_hour = countdown_hour_old;
					break;
						
					case 7:															//����7������ʱ��+1
						countdown_minute_old++;
						if(countdown_minute_old >= 60)
							countdown_minute_old = 0;
						countdown_minute = countdown_minute_old;
					break;
						
					case 8:															//����8������ʱ��+1
						countdown_second_old++;
						if(countdown_second_old >= 60)
							countdown_second_old = 0;
						countdown_second = countdown_second_old;
					break;
					
					default:
					break;
				}
			break;

			case 3:																	//����3������
				switch(flag_display)
				{
					case 1:															//����1���¶����ֵ-1
						if(temp_max > temp_min+1)
							temp_max--;
					break;
					
					case 2:															//����2���¶���Сֵ-1
						//if(temp_min > 0)
							temp_min--;
					break;
					
					case 3:															//����3���ܸ߶�ֵ-1
						if(distance_all > (distance_max+1))
							distance_all--;
					break;
					
					case 4:															//����4��ˮλ���ֵ-1
						if(distance_max > distance_min+1)
							distance_max--;
					break;
						
					case 5:															//����5��ˮλ��Сֵ-1
						if(distance_min > 0)
							distance_min--;
					break;
						
					case 6:															//����6������ʱʱ-1
						if(countdown_hour_old > 0)
							countdown_hour_old--;
						countdown_hour = countdown_hour_old;
					break;
						
					case 7:															//����7������ʱ��-1
						countdown_minute_old--;
						if(countdown_minute_old < 0)
							countdown_minute_old = 59;
						countdown_minute = countdown_minute_old;
					break;
						
					case 8:															//����8������ʱ��-1
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
*******��⺯��
*****/
void Monitor_function(void)
{
	if(time_num % 10 == 0)																			//100ms���һ��
	{
		temp_value = Ds18b20_Read_Temp();													//��ȡ�¶�ֵ
		distance_value = Hcsr04_GetDistance(temp_value/10);				//��ȡ����ֵ
		if(distance_value > distance_all)													//����ʣ��ˮλ
			distance_value = 0;
		else
			distance_value = distance_all - distance_value;
	}
}

/****
*******��ʾ����
*****/
void Display_function(void)
{
	switch(flag_display)																				//���ݲ�ͬ����ʾģʽ��־λ����ʾ��ͬ�Ľ���
	{
		case 0:																										//����0
			lcd1602_display_str(1,0,"Temp:");												//��ʾ�¶Ȳ���ֵ
			lcd1602_display_temp(1,5,temp_value);

			lcd1602_display_str(2,0,"D:");													//��ʾʣ��ˮλ
			lcd1602_display_distance(2,2,distance_value);
								
			lcd1602_display_countdown(2,8,countdown_hour,countdown_minute,countdown_second);		//��ʾ����ʱʱ��
		break;

		case 1:																										//����1����ʾ��������¶�
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
			
		case 2:																										//����2����ʾ������С�¶�
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
			
		case 3:																										//����3����ʾ�����ܸ߶�
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
		
		case 4:																										//����4����ʾ����ˮλ���ֵ
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
			
		case 5:																										//����5����ʾ����ˮλ��Сֵ
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
			
		case 6:																										//����6����ʾ���õ���ʱʱ
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
			
		case 7:																										//����7����ʾ���õ���ʱ��
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
			
		case 8:																										//����8����ʾ���õ���ʱ��
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
*******������
*****/
void Manage_function(void)
{
	if(flag_display == 0)																		//��������
	{
		if(temp_value > temp_max*10)													//�¶ȴ����������ֵ������̵����պ�
		{
			RELAY_ZL = 0;
			RELAY_JR = 1;
		}
		else if(temp_value < temp_min*10)											//�¶�С��������Сֵ�����ȼ̵����պ�
		{
			RELAY_ZL = 1;
			RELAY_JR = 0;
		}
		else																									//�¶ȴ������õ�������ֵ֮�䣬�����̵����Ͽ�
		{
			RELAY_ZL = 1;
			RELAY_JR = 1;
		}
		
		if(distance_value < distance_min)											//ˮλֵС��������Сֵ����ˮ�̵����պϣ������ⱨ��
		{
			RELAY_JS = 0;
			RELAY_CS = 1;
			if(time_num % 20 == 0)
			{
				LED = ~LED;
				BEEP = ~BEEP;
			}
		}
		else if(distance_value > distance_max)								//ˮλֵ�����������ֵ����ˮ�̵����պϣ������ⱨ��
		{
			RELAY_JS = 1;
			RELAY_CS = 0;
			if(time_num % 20 == 0)
			{
				LED = ~LED;
				BEEP = ~BEEP;
			}
		}
		else																									//ˮλֵ����������ֵ֮�䣬�����̵����Ͽ�����ȡ�����ⱨ��
		{
			RELAY_JS = 1;
			RELAY_CS = 1;
			LED = 1;
			BEEP = 1;
		}
		
		if(countdown_hour != 0 || countdown_minute != 0 || countdown_second != 0)			//��ˮ����ʱ��Ϊ0
			flag_timer_begin = 1;																												//��ʼ����ʱ�����붨ʱ���ж�
		
		if(flag_1s == 1)																															//1s�������ʱ-1s
		{
			flag_1s = 0;
			if(countdown_second > 0)																										//����ʱ��>0
				countdown_second--;																												//����ʱ��-1
			else																																				//����ʱ��=0
			{
				if(countdown_minute > 0)																									//�������ʱ��>0
				{
					countdown_minute--;																											//����ʱ��-1
					countdown_second = 59;																									//����ʱ��=59
				}
				else																																			//�������ʱ��=0
				{
					if(countdown_hour > 0)																									//�������ʱʱ>0
					{
						countdown_hour--;																											//����ʱʱ-1
						countdown_minute = 59;																								//����ʱ��=59
						countdown_second = 59;																								//����ʱ��=59
					}
					else																																		//�������ʱʱ=0������ʱ����
					{
						flag_timer_begin = 0;
						flag_countdown_end = 1;
					}
				}
			}
		}
		
		if(flag_countdown_end == 1)																										//����ʱ����
		{
			LED = 1;
			BEEP = 1;

			do																																					//��ʼ��ˮ��ֱ��ˮλС����Сֵ
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
			
			do																																					//��ˮ��ɺ󣬿�ʼ��ˮ��ֱ��ˮλ�������ֵ
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
			countdown_hour = countdown_hour_old;																					//���¸�ֵ����ʱʱ��
			countdown_minute = countdown_minute_old;
			countdown_second = countdown_second_old;
		}
	}
	else																																							//���ý��棬���м̵����Ͽ������ⱨ��ȡ��
	{
		RELAY_ZL = 1;
		RELAY_JR = 1;
		RELAY_JS = 1;
		RELAY_CS = 1;
		LED = 1;
		BEEP = 1;
	}
}

