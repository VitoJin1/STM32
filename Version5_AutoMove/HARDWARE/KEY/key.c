#include "key.h"
#include "sys.h" 
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//按键驱动代码	   
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  
								    
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE,ENABLE);//使能PORTA,PORTE时钟
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5|GPIO_Pin_6;//PE2~4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE2,3,4
}
//按键处理函数
//返回按键值

//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY2按下 
//4，KEY3按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY2>KEY3!!

//1---支持连按
//2---不支持连按
u8 KEY_Scan(u8 mode)
{
    static u8 key_up=1;
    if(mode)key_up=1;
	if(key_up&&(Switch1==0||Switch2==0))
	{
		delay_ms(10);//去抖?
        key_up=0;
		if(Switch1==0)return KEY_S1;
		else if(Switch2==0)return KEY_S2;
	}
    else if(Switch1==1&&Switch2==1)
    {
        key_up=1;
    }
 	return 0;// 无按键按下
}
