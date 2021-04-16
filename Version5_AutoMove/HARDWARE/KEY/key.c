#include "key.h"
#include "sys.h" 
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌÐòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßÐí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ALIENTEKÕ½½¢STM32¿ª·¢°å
//°´¼üÇý¶¯´úÂë	   
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  
								    
//°´¼ü³õÊ¼»¯º¯Êý
void KEY_Init(void) //IO³õÊ¼»¯
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE,ENABLE);//Ê¹ÄÜPORTA,PORTEÊ±ÖÓ
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5|GPIO_Pin_6;//PE2~4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//ÊäÈë
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//ÉÏÀ­
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//³õÊ¼»¯GPIOE2,3,4
}
//°´¼ü´¦Àíº¯Êý
//·µ»Ø°´¼üÖµ

//0£¬Ã»ÓÐÈÎºÎ°´¼ü°´ÏÂ
//1£¬KEY0°´ÏÂ
//2£¬KEY1°´ÏÂ
//3£¬KEY2°´ÏÂ 
//4£¬KEY3°´ÏÂ WK_UP
//×¢Òâ´Ëº¯ÊýÓÐÏìÓ¦ÓÅÏÈ¼¶,KEY0>KEY1>KEY2>KEY3!!

//1---Ö§³ÖÁ¬°´
//2---²»Ö§³ÖÁ¬°´
u8 KEY_Scan(u8 mode)
{
    static u8 key_up=1;
    if(mode)key_up=1;
	if(key_up&&(Switch1==0||Switch2==0))
	{
		delay_ms(10);//È¥¶¶¶
        key_up=0;
		if(Switch1==0)return KEY_S1;
		else if(Switch2==0)return KEY_S2;
	}
    else if(Switch1==1&&Switch2==1)
    {
        key_up=1;
    }
 	return 0;// ÎÞ°´¼ü°´ÏÂ
}
