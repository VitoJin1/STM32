#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F103ZE���İ�
//LED��������	   
							  
////////////////////////////////////////////////////////////////////////////////// 
#define LED_B PBout(12)
#define LED_G PBout(13)	
#define LED_R PBout(14)
#define LED_Y PBout(15)
#define Input_Signal GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)
void LED_Init(void);//��ʼ��
void Signal_Init(void);
		 				    
#endif
