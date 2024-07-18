/**********************************
作者：单片机俱乐部
网站：https://www.mcuclub.cn/
**********************************/


/**********************************
包含头文件
**********************************/
#include "key.h"


/**********************************
函数定义
**********************************/
/****
*******	按键处理函数
key_mode:0,不支持连续按;1,支持连续按;
返回值：键值
*****/
uchar Chiclet_Keyboard_Scan(uchar key_mode)
{	 
	static uchar key_up=1;										//按键松开标志

	if(key_mode)  														//mode=1,支持连按
		key_up=1;
	
	if(key_up && (KEY1==0 || KEY2==0 || KEY3==0))
	{
		Delay_function(10);											//去抖动 
		key_up=0;

		if(KEY1==0)															//按键1按下，返回1
			return 1;
		else if(KEY2==0)												//按键2按下，返回2
			return 2;
		else if(KEY3==0)												//按键3按下，返回3
			return 3;
	}
	else if(KEY1==1 && KEY2==1 && KEY3==1)		//没有按键按下，松开标志位置1
		key_up=1;
	
	return 0;																	// 无按键按下.返回0
}

