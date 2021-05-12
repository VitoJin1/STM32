#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F103ZE核心板
//LED驱动代码	   
							  
////////////////////////////////////////////////////////////////////////////////// 
#define LED_B PBout(12)
#define LED_G PBout(13)	
#define LED_R PBout(14)
#define LED_Y PBout(15)
#define Input_Signal GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)
void LED_Init(void);//初始化
void Signal_Init(void);
		 				    
#endif
